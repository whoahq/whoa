#include "gx/mtl/CGxDeviceMTL.hpp"
#include "app/mac/View.h"
#include "gx/Window.hpp"
#include "util/Autorelease.hpp"
#include <storm/Memory.hpp>
#include <cmath>
#include <cstring>

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

CGxDeviceMTL::CGxDeviceMTL() : CGxDevice() {
    this->m_api = GxApi_Metal;
    this->m_caps.m_colorFormat = GxCF_rgba;

    this->DeviceCreatePools();
    this->DeviceCreateStreamBufs();
}

void CGxDeviceMTL::ITexMarkAsUpdated(CGxTex* texId) {
    CGxDevice::ITexMarkAsUpdated(texId);
}

void CGxDeviceMTL::IRsSendToHw(EGxRenderState which) {
    (void)which;
}

int32_t CGxDeviceMTL::DeviceCreate(int32_t (*windowProc)(void* window, uint32_t message, uintptr_t wparam, intptr_t lparam), const CGxFormat& format) {
    System_Autorelease::ScopedPool autorelease;

    CGRect rect;
    Rect* bounds;
    Rect* zoomedBounds = GetSavedZoomedWindowBounds();

    if (
        zoomedBounds
        && zoomedBounds->bottom - zoomedBounds->top > 599
        && zoomedBounds->right - zoomedBounds->left > 799
    ) {
        bounds = GetSavedZoomedWindowBounds();
    } else {
        bounds = GetSavedWindowBounds();
    }

    if (
        bounds->bottom - bounds->top > 599
        && bounds->right - bounds->left > 799
    ) {
        rect.origin.x = bounds->left;
        rect.origin.y = bounds->top;
        rect.size.width = bounds->right - bounds->left;
        rect.size.height = bounds->bottom - bounds->top;
    } else {
        Rect newBounds = {
            0,
            0,
            static_cast<int16_t>(std::floor((static_cast<float>(format.size.y) / static_cast<float>(format.size.x)) * 1024.0f)),
            1024,
        };

        SetSavedWindowBounds(newBounds);

        rect.origin.x = newBounds.left;
        rect.origin.y = newBounds.top;
        rect.size.width = newBounds.right;
        rect.size.height = newBounds.bottom;
    }

    this->m_window.SetViewClass(GetEngineViewClass());
    this->m_window.Init(rect, nullptr);
    this->m_window.SetTitle("World of Warcraft");
    this->m_window.CreateView();

    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (!device) {
        return 0;
    }

    id<MTLCommandQueue> commandQueue = [device newCommandQueue];
    if (!commandQueue) {
        return 0;
    }

    NSView* view = this->m_window.GetNSView();
    CAMetalLayer* layer = view ? (CAMetalLayer*)[view layer] : nil;
    if (!layer) {
        return 0;
    }

    layer.device = device;
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    layer.framebufferOnly = YES;

    CGSize drawableSize = [view convertSizeToBacking:view.bounds.size];
    layer.drawableSize = drawableSize;

    this->m_device = device;
    this->m_commandQueue = commandQueue;
    this->m_layer = layer;

    if (CGxDevice::DeviceCreate(windowProc, format)) {
        auto callbacks = new GLWindowCallbacks();
        AssignEngineViewCallbacks(callbacks);
        this->m_window.SetCallbacks(callbacks);
        this->m_window.Show();

        this->ISetCaps(format);

        this->m_context = 1;
        this->ICursorCreate(format);

        return 1;
    }

    return 0;
}

void CGxDeviceMTL::ISetCaps(const CGxFormat& format) {
    (void)format;

    this->m_caps.m_pixelCenterOnEdge = 1;
    this->m_caps.m_texelCenterOnEdge = 1;
    this->m_caps.m_colorFormat = GxCF_rgba;
    this->m_caps.m_generateMipMaps = 1;
    this->m_caps.int10 = 1;

    this->m_caps.m_texFmt[GxTex_Dxt1] = 1;
    this->m_caps.m_texFmt[GxTex_Dxt3] = 1;
    this->m_caps.m_texFmt[GxTex_Dxt5] = 1;

    this->m_caps.m_texFilterAnisotropic = 1;
    this->m_caps.m_maxTexAnisotropy = 16;

    for (int32_t i = 0; i < GxTexTargets_Last; ++i) {
        this->m_caps.m_texTarget[i] = 1;
        this->m_caps.m_texMaxSize[i] = 4096;
    }
}

int32_t CGxDeviceMTL::DeviceSetFormat(const CGxFormat& format) {
    CGxDevice::DeviceSetFormat(format);

    CRect rect = { 0.0f, 0.0f, static_cast<float>(format.size.y), static_cast<float>(format.size.x) };
    this->DeviceSetDefWindow(rect);

    if (this->m_window.m_Window) {
        this->m_window.Resize(format.size.x, format.size.y);
    }

    return 1;
}

void* CGxDeviceMTL::DeviceWindow() {
    return &this->m_window;
}

void CGxDeviceMTL::CapsWindowSize(CRect& rect) {
    CRect windowRect = this->DeviceCurWindow();

    rect.minX = windowRect.minX;
    rect.minY = windowRect.minY;
    rect.maxX = windowRect.maxX;
    rect.maxY = windowRect.maxY;
}

void CGxDeviceMTL::CapsWindowSizeInScreenCoords(CRect& dst) {
    if (this->IDevIsWindowed()) {
        auto windowRect = this->DeviceCurWindow();
        auto deviceRect = this->m_window.GetRect();

        dst.minX = windowRect.minX + deviceRect.origin.x;
        dst.maxX = windowRect.maxX + deviceRect.origin.x;
        dst.minY = windowRect.minY + deviceRect.origin.y;
        dst.maxY = windowRect.maxY + deviceRect.origin.y;
    } else {
        dst = this->DeviceCurWindow();
    }
}

void CGxDeviceMTL::SceneClear(uint32_t mask, CImVector color) {
    CGxDevice::SceneClear(mask, color);

    if (!this->m_context) {
        return;
    }

    this->m_clearMask = mask;
    this->m_clearColor = color.value;
}

void CGxDeviceMTL::ScenePresent() {
    if (!this->m_context) {
        return;
    }

    auto device = (id<MTLDevice>)this->m_device;
    auto commandQueue = (id<MTLCommandQueue>)this->m_commandQueue;
    auto layer = (CAMetalLayer*)this->m_layer;

    if (!device || !commandQueue || !layer) {
        return;
    }

    System_Autorelease::ScopedPool autorelease;

    id<CAMetalDrawable> drawable = [layer nextDrawable];
    if (!drawable) {
        return;
    }

    const uint8_t r = (this->m_clearColor >> 16) & 0xFF;
    const uint8_t g = (this->m_clearColor >> 8) & 0xFF;
    const uint8_t b = this->m_clearColor & 0xFF;
    const uint8_t a = (this->m_clearColor >> 24) & 0xFF;

    auto pass = [MTLRenderPassDescriptor renderPassDescriptor];
    pass.colorAttachments[0].texture = drawable.texture;
    pass.colorAttachments[0].loadAction = (this->m_clearMask & 0x1) ? MTLLoadActionClear : MTLLoadActionLoad;
    pass.colorAttachments[0].storeAction = MTLStoreActionStore;
    pass.colorAttachments[0].clearColor = MTLClearColorMake(
        r / 255.0f,
        g / 255.0f,
        b / 255.0f,
        a / 255.0f
    );

    id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
    id<MTLRenderCommandEncoder> encoder = [commandBuffer renderCommandEncoderWithDescriptor: pass];
    [encoder endEncoding];

    [commandBuffer presentDrawable: drawable];
    [commandBuffer commit];
}

void CGxDeviceMTL::Draw(CGxBatch* batch, int32_t indexed) {
    (void)batch;
    (void)indexed;
}

void CGxDeviceMTL::PoolSizeSet(CGxPool* pool, uint32_t size) {
    if (!pool || pool->m_size >= static_cast<int32_t>(size)) {
        return;
    }

    pool->m_size = static_cast<int32_t>(size);
    pool->unk1C = 0;

    if (pool->m_mem) {
        SMemFree(pool->m_mem, __FILE__, __LINE__, 0x0);
        pool->m_mem = nullptr;
    }
}

char* CGxDeviceMTL::BufLock(CGxBuf* buf) {
    CGxDevice::BufLock(buf);

    if (!this->m_context) {
        return nullptr;
    }

    CGxPool* pool = buf->m_pool;
    if (!pool->m_mem) {
        pool->m_mem = SMemAlloc(pool->m_size, __FILE__, __LINE__, 0x0);
    }

    if (pool->m_usage == GxPoolUsage_Stream) {
        uint32_t v7 = pool->unk1C + buf->m_itemSize - 1;
        uint32_t alignedNext = v7 - v7 % buf->m_itemSize;

        if (alignedNext + buf->m_size > static_cast<uint32_t>(pool->m_size)) {
            pool->Discard();
            alignedNext = 0;
        }

        buf->m_index = alignedNext;
        pool->unk1C = alignedNext + buf->m_size;
    }

    if (!pool->m_mem) {
        return nullptr;
    }

    return static_cast<char*>(pool->m_mem) + buf->m_index;
}

int32_t CGxDeviceMTL::BufUnlock(CGxBuf* buf, uint32_t size) {
    CGxDevice::BufUnlock(buf, size);
    buf->unk1D = 1;
    return 1;
}

void CGxDeviceMTL::BufData(CGxBuf* buf, const void* data, size_t size, uintptr_t offset) {
    CGxDevice::BufData(buf, data, size, offset);

    auto bufData = this->BufLock(buf);
    if (bufData) {
        memcpy(&bufData[offset], data, size);
    }
    this->BufUnlock(buf, static_cast<uint32_t>(size));
}

void CGxDeviceMTL::TexDestroy(CGxTex* texId) {
    CGxDevice::TexDestroy(texId);
}

void CGxDeviceMTL::IShaderCreate(CGxShader* shader) {
    shader->loaded = 1;
    shader->valid = 1;
}

void CGxDeviceMTL::ShaderCreate(CGxShader* shaders[], EGxShTarget target, const char* a4, const char* a5, int32_t permutations) {
    CGxDevice::ShaderCreate(shaders, target, a4, a5, permutations);
}

int32_t CGxDeviceMTL::StereoEnabled() {
    return 0;
}

void CGxDeviceMTL::Resize(uint32_t width, uint32_t height) {
    CRect rect = { 0.0f, 0.0f, static_cast<float>(height), static_cast<float>(width) };
    this->DeviceSetDefWindow(rect);
}
