#include "gx/glsdl/CGxDeviceGLSDL.hpp"
#include "event/Input.hpp"
#include "gx/Blit.hpp"
#include "gx/CGxBatch.hpp"
#include "gx/Shader.hpp"
#include "gx/Window.hpp"
#include "gx/texture/CGxTex.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <bc/Debug.hpp>

GLEnum CGxDeviceGLSDL::s_glCubeMapFaces[] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

GLEnum CGxDeviceGLSDL::s_glDstBlend[] = {
    GL_ZERO,                    // GxBlend_Opaque
    GL_ZERO,                    // GxBlend_AlphaKey
    GL_ONE_MINUS_SRC_ALPHA,     // GxBlend_Alpha
    GL_ONE,                     // GxBlend_Add
    GL_ZERO,                    // GxBlend_Mod
    GL_SRC_COLOR,               // GxBlend_Mod2x
    GL_ONE,                     // GxBlend_ModAdd
    GL_ONE,                     // GxBlend_InvSrcAlphaAdd
    GL_ZERO,                    // GxBlend_InvSrcAlphaOpaque
    GL_ZERO,                    // GxBlend_SrcAlphaOpaque
    GL_ONE,                     // GxBlend_NoAlphaAdd
    GL_ONE_MINUS_CONSTANT_ALPHA // GxBlend_ConstantAlpha
};

GLEnum CGxDeviceGLSDL::s_glSrcBlend[] = {
    GL_ONE,                     // GxBlend_Opaque
    GL_ONE,                     // GxBlend_AlphaKey
    GL_SRC_ALPHA,               // GxBlend_Alpha
    GL_SRC_ALPHA,               // GxBlend_Add
    GL_DST_COLOR,               // GxBlend_Mod
    GL_DST_COLOR,               // GxBlend_Mod2x
    GL_DST_COLOR,               // GxBlend_ModAdd
    GL_ONE_MINUS_SRC_ALPHA,     // GxBlend_InvSrcAlphaAdd
    GL_ONE_MINUS_SRC_ALPHA,     // GxBlend_InvSrcAlphaOpaque
    GL_SRC_ALPHA,               // GxBlend_SrcAlphaOpaque
    GL_ONE,                     // GxBlend_NoAlphaAdd
    GL_CONSTANT_ALPHA           // GxBlend_ConstantAlpha
};

GLTextureFormat CGxDeviceGLSDL::s_gxTexFmtToGLSDLFmt[] = {
    GLTF_INVALID,   // GxTex_Unknown
    GLTF_ABGR8888,  // GxTex_Abgr8888
    GLTF_ARGB8888,  // GxTex_Argb8888
    GLTF_ARGB4444,  // GxTex_Argb4444
    GLTF_ARGB1555,  // GxTex_Argb1555
    GLTF_RGB565,    // GxTex_Rgb565
    GLTF_DXT1,      // GxTex_Dxt1
    GLTF_DXT3,      // GxTex_Dxt3
    GLTF_DXT5,      // GxTex_Dxt5
    GLTF_INVALID,   // GxTex_Uv88
    GLTF_INVALID,   // GxTex_Gr1616F
    GLTF_INVALID,   // GxTex_R32F
    GLTF_D24        // GxTex_D24X8
};

GLEnum CGxDeviceGLSDL::s_poolTarget2BufferFormat[] = {
    GL_ZERO,            // GxPoolTarget_Vertex
    GL_UNSIGNED_SHORT   // GxPoolTarget_Index
};

GLEnum CGxDeviceGLSDL::s_poolTarget2BufferType[] = {
    GL_ARRAY_BUFFER,            // GxPoolTarget_Vertex
    GL_ELEMENT_ARRAY_BUFFER     // GxPoolTarget_Index
};

GLEnum CGxDeviceGLSDL::s_poolUsage2BufferUsage[] = {
    GL_STATIC_DRAW,     // GxPoolUsage_Static
    GL_DYNAMIC_DRAW,    // GxPoolUsage_Dynamic
    GL_DYNAMIC_DRAW     // GxPoolUsage_Stream
};

GLEnum CGxDeviceGLSDL::s_primitiveConversion[] = {
    GL_POINTS,          // GxPrim_Points
    GL_LINES,           // GxPrim_Lines
    GL_LINE_STRIP,      // GxPrim_LineStrip
    GL_TRIANGLES,       // GxPrim_Triangles
    GL_TRIANGLE_STRIP,  // GxPrim_TriangleStrip
    GL_TRIANGLE_FAN,    // GxPrim_TriangleFan
    GL_ZERO             // GxPrims_Last
};

CGxDeviceGLSDL::CGxDeviceGLSDL() : CGxDevice() {
    // TODO

    this->m_api = GxApi_OpenGl;
    this->m_caps.m_colorFormat = GxCF_rgba;

    // TODO

    this->DeviceCreatePools();
    this->DeviceCreateStreamBufs();

    // TODO
}

char* CGxDeviceGLSDL::BufLock(CGxBuf* buf) {
    CGxDevice::BufLock(buf);
    return this->IBufLock(buf);
}

int32_t CGxDeviceGLSDL::BufUnlock(CGxBuf* buf, uint32_t size) {
    CGxDevice::BufUnlock(buf, size);
    return this->IBufUnlock(buf);
}

void CGxDeviceGLSDL::BufData(CGxBuf* buf, const void* data, size_t size, uintptr_t offset) {
    CGxDevice::BufData(buf, data, size, offset);

    auto bufData = this->IBufLock(buf);
    memcpy(&bufData[offset], data, size);
    this->IBufUnlock(buf);
}

void CGxDeviceGLSDL::CapsWindowSize(CRect& rect) {
    CRect windowRect = this->DeviceCurWindow();

    rect.minX = windowRect.minX;
    rect.minY = windowRect.minY;
    rect.maxX = windowRect.maxX;
    rect.maxY = windowRect.maxY;
}

void CGxDeviceGLSDL::CapsWindowSizeInScreenCoords(CRect& dst) {
    if (this->IDevIsWindowed()) {
        auto windowRect = this->DeviceCurWindow();
        auto deviceRect = this->m_GLSDLWindow.GetRect();

        dst.minX = windowRect.minX + deviceRect.origin.x;
        dst.maxX = windowRect.maxX + deviceRect.origin.x;
        dst.minY = windowRect.minY + deviceRect.origin.y;
        dst.maxY = windowRect.maxY + deviceRect.origin.y;
    } else {
        dst = this->DeviceCurWindow();
    }
}

int32_t CGxDeviceGLSDL::DeviceCreate(int32_t (*windowProc)(void* window, uint32_t message, uintptr_t wparam, intptr_t lparam), const CGxFormat& format) {
    GLSDLWindowRect rect;
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

    this->m_GLSDLWindow.Create("World of Warcraft", rect, GLTF_D24, 1);
    this->m_GLSDLDevice.Init(&this->m_GLSDLWindow, "WoW", 0);
    // this->m_GLSDLDevice.Init(&this->m_GLSDLWindow, "WoW", 4);

    GLSDLDevice::SetOption(GLSDLDevice::eShaderConstantBindings, false);
    GLSDLDevice::SetOption(GLSDLDevice::eUseMTGL, true);

    if (CGxDevice::DeviceCreate(windowProc, format)) {
        // this->m_glWindow.Show();

        GLSDLDevice::SetOption(GLSDLDevice::eUseHybridShader, true);

        this->ISetCaps(format);

        this->m_context = 1;

        this->ICursorCreate(format);

        return 1;
    } else {
        return 0;
    }
}

int32_t CGxDeviceGLSDL::DeviceSetFormat(const CGxFormat& format) {
    static GLTextureFormat glSDLTexFormats[] = {
        GLTF_RGB565,
        GLTF_XRGB8888,
        GLTF_ARGB8888,
        GLTF_A2RGB10,
        GLTF_D16,
        GLTF_D24,
        GLTF_D24S8,
        GLTF_D32
    };

    bool v7 = false;
    bool v10 = false;

    Rect v15 = {
        0,
        0,
        static_cast<int16_t>(format.size.y),
        static_cast<int16_t>(format.size.x)
    };

    if (format.window && format.maximize != 1) {
        Rect* zoomedBounds = GetSavedZoomedWindowBounds();
        Rect* bounds;

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
            v15.right = bounds->right - bounds->left;
            v15.bottom = bounds->bottom - bounds->top;
        } else {
            Rect newBounds = {
                0,
                0,
                static_cast<int16_t>(std::floor((static_cast<float>(format.size.y) / static_cast<float>(format.size.x)) * 1024.0f)),
                1024,
            };

            SetSavedWindowBounds(newBounds);

            v15.right = newBounds.right;
            v15.bottom = newBounds.bottom;
        }

        // TODO GxMacPrivate::SetAspectRatio(static_cast<float>(format.size.x) / static_cast<float>(format.size.y));

        // TODO auto v8 = LMGetMainDevice();
        // TODO GxMacPrivate::ConstrainWindowRect(v15, v8);

        v7 = true;
        v10 = true;
    }

    CRect wind = {
        0.0f,
        0.0f,
        static_cast<float>(v15.bottom - v15.top),
        static_cast<float>(v15.right - v15.left)
    };

    this->DeviceSetDefWindow(wind);

    this->m_GLSDLDevice.SetDisplay(
        v15.right - v15.left,
        v15.bottom - v15.top,
        glSDLTexFormats[format.colorFormat],
        glSDLTexFormats[format.depthFormat],
        format.refreshRate,
        v7,
        format.window ^ 1,
        format.sampleCount
    );

    OsInputPostEvent(OS_INPUT_SIZE, v15.right - v15.left, v15.bottom - v15.top, 0, 0);

    // TODO
    // GameFrame::SetSize(v15.right - v15.left, v15.bottom - v15.top);

    // TODO
    // this->m_GLSDLDevice.SetVSync(format.vsync != 0);

    this->IStateSetGLSDLDefaults();

    if (v10) {
        // TODO
        // this->m_GLSDLDevice.RestoreDefaultGamma();
    } else {
        // TODO
        // - unknown vtable call
    }

    // TODO
    // - this seems to belong elsewhere
    CGxDevice::DeviceSetFormat(format);

    return 1;
}

void* CGxDeviceGLSDL::DeviceWindow() {
    return &this->m_GLSDLWindow;
}

void CGxDeviceGLSDL::Draw(CGxBatch* batch, int32_t indexed) {
    if (!this->m_context) {
        return;
    }

    // TODO
    // unk conditional check early return

    this->IStateSync();
    this->ValidateDraw(batch, indexed);

    uint32_t baseIndex = 0;
    if (!this->m_caps.int10) {
        baseIndex = this->m_primVertexFormatBuf[0]->m_index / this->m_primVertexFormatBuf[0]->m_itemSize;
    }

    if (indexed) {
        this->m_GLSDLDevice.DrawIndexed(
            CGxDeviceGLSDL::s_primitiveConversion[batch->m_primType],
            batch->m_minIndex,
            batch->m_maxIndex,
            baseIndex,
            batch->m_start + (this->m_primIndexBuf->m_index / 2),
            batch->m_count
        );
    } else {
        this->m_GLSDLDevice.Draw(
            CGxDeviceGLSDL::s_primitiveConversion[batch->m_primType],
            baseIndex,
            batch->m_count
        );
    }
}

char* CGxDeviceGLSDL::IBufLock(CGxBuf* buf) {
    if (!this->m_context) {
        // TODO
        // EmergencyMem
        return nullptr;
    }

    CGxPool* pool = buf->m_pool;

    GLBuffer::eMapFlag mapFlag = GLBuffer::GLMap_None;

    if (pool->m_usage == GxPoolUsage_Stream) {
        mapFlag = GLBuffer::GLMap_Unk1;

        uint32_t v7 = pool->unk1C + buf->m_itemSize - 1;
        uint32_t alignedNext = v7 - v7 % buf->m_itemSize;

        if (alignedNext + buf->m_size > pool->m_size) {
            pool->Discard();
            alignedNext = 0;
            mapFlag = GLBuffer::GLMap_Unk2;
        }

        buf->m_index = alignedNext;
        pool->unk1C = alignedNext + buf->m_size;
    } else {
        mapFlag = pool->m_usage == GxPoolUsage_Dynamic ? GLBuffer::GLMap_Unk1 : GLBuffer::GLMap_None;
    }

    GLBuffer* glBuf = reinterpret_cast<GLBuffer*>(pool->m_apiSpecific);

    if (!glBuf) {
        glBuf = this->m_GLSDLDevice.CreateBuffer(
            CGxDeviceGLSDL::s_poolTarget2BufferType[pool->m_target],
            pool->m_size,
            nullptr,
            CGxDeviceGLSDL::s_poolUsage2BufferUsage[pool->m_usage],
            CGxDeviceGLSDL::s_poolTarget2BufferFormat[pool->m_target]
        );

        pool->m_apiSpecific = glBuf;
    }

    if (!glBuf) {
        // TODO
        // EmergencyMem
        return nullptr;
    }

    return glBuf->Map(buf->m_index, buf->m_size, mapFlag);
}

int32_t CGxDeviceGLSDL::IBufUnlock(CGxBuf* buf) {
    CGxPool* pool = buf->m_pool;

    // TODO
    // EmergencyMem

    auto glBuf = reinterpret_cast<GLBuffer*>(pool->m_apiSpecific);

    glBuf->Unmap(0);
    buf->unk1D = 1;

    return 1;
}

void CGxDeviceGLSDL::IRsSendToHw(EGxRenderState which) {
    auto state = &this->m_appRenderStates[which];

    switch (which) {
        // TODO
        // - remaining render states

        case GxRs_BlendingMode: {
            int32_t blend = static_cast<int32_t>(state->m_value);

            if (blend <= GxBlend_AlphaKey) {
                this->m_GLSDLDevice.SetAlphaBlendEnable(0);
            } else {
                this->m_GLSDLDevice.SetAlphaBlendEnable(1);
                this->m_GLSDLDevice.SetAlphaBlend(
                    CGxDeviceGLSDL::s_glSrcBlend[blend],
                    CGxDeviceGLSDL::s_glDstBlend[blend],
                    GL_FUNC_ADD
                );
            }

            break;
        }

        case GxRs_AlphaRef: {
            int32_t alphaRef = static_cast<int32_t>(state->m_value);

            if (alphaRef <= 0) {
                this->m_GLSDLDevice.SetAlphaTestEnable(0);
            } else {
                this->m_GLSDLDevice.SetAlphaTest(GL_GEQUAL, static_cast<float>(alphaRef) / static_cast<float>(255));
                this->m_GLSDLDevice.SetAlphaTestEnable(1);
            }

            break;
        }

        case GxRs_FogStart: {
            auto fogStart = static_cast<float>(state->m_value);

            this->m_GLSDLDevice.SetFogParam(GL_FOG_START, fogStart);

            break;
        }

        case GxRs_FogEnd: {
            auto fogEnd = static_cast<float>(state->m_value);

            this->m_GLSDLDevice.SetFogParam(GL_FOG_END, fogEnd);

            break;
        }

        case GxRs_FogColor: {
            auto fogColor = static_cast<CImVector>(state->m_value);

            this->m_GLSDLDevice.SetFogColor(
                fogColor.r / 255.0f,
                fogColor.g / 255.0f,
                fogColor.b / 255.0f,
                fogColor.a / 255.0f
            );

            break;
        }

        case GxRs_Lighting: {
            int32_t lightingEnable = 0;

            if (this->MasterEnable(GxMasterEnable_Lighting)) {
                lightingEnable = static_cast<int32_t>(state->m_value);
            }

            this->m_GLSDLDevice.SetLightingEnable(lightingEnable);

            break;
        }

        case GxRs_Fog: {
            int32_t fogEnable = 0;

            if (this->MasterEnable(GxMasterEnable_Fog)) {
                fogEnable = static_cast<int32_t>(state->m_value);
            }

            this->m_GLSDLDevice.SetFogEnable(fogEnable);

            break;
        }

        case GxRs_DepthTest: {
            int32_t depthTest = 0;

            if (this->MasterEnable(GxMasterEnable_DepthTest)) {
                depthTest = static_cast<int32_t>(state->m_value);
            }

            this->m_GLSDLDevice.SetDepthTestEnable(depthTest != 0);

            break;
        }

        case GxRs_DepthFunc: {
            static GLEnum s_glDepthFunc[] = {
                GL_LEQUAL,
                GL_EQUAL,
                GL_GEQUAL,
                GL_LESS,
                GL_NONE,
                GL_NONE
            };

            GLEnum depthFunc = s_glDepthFunc[static_cast<int32_t>(state->m_value)];
            this->m_GLSDLDevice.SetDepthTestFunc(depthFunc);

            break;
        }

        case GxRs_DepthWrite: {
            int32_t depthWrite = static_cast<int32_t>(state->m_value);
            bool depthWriteMask = this->MasterEnable(GxMasterEnable_DepthWrite) && depthWrite
                ? 1 : 0;

            this->m_GLSDLDevice.SetDepthWriteMask(depthWriteMask);

            break;
        }

        case GxRs_Culling: {
            static GLEnum cullMode[] = {
                GL_ZERO,
                GL_CW,
                GL_CCW
            };

            // TODO
            // - only use state if unk flag 0x20 set, otherwise use 0

            int32_t mode = static_cast<int32_t>(state->m_value);

            // TODO
            // BLIZZARD_ASSERT(mode <= GxCullState_CCW);

            this->m_GLSDLDevice.SetCullMode(cullMode[mode]);

            break;
        }

        case GxRs_Texture0:
        case GxRs_Texture1:
        case GxRs_Texture2:
        case GxRs_Texture3:
        case GxRs_Texture4:
        case GxRs_Texture5:
        case GxRs_Texture6:
        case GxRs_Texture7:
        case GxRs_Texture8:
        case GxRs_Texture9:
        case GxRs_Texture10:
        case GxRs_Texture11:
        case GxRs_Texture12:
        case GxRs_Texture13:
        case GxRs_Texture14:
        case GxRs_Texture15: {
            int32_t tmu = which - GxRs_Texture0;

            if (tmu <= 15) {
                CGxTex* texture = static_cast<CGxTex*>(static_cast<void*>(state->m_value));

                if (texture) {
                    this->ITexBind();
                    this->ITexMarkAsUpdated(texture);

                    GLTexture* glTexture = static_cast<GLTexture*>(texture->m_apiSpecificData);
                    this->m_GLSDLDevice.SetTexture(tmu, glTexture);
                } else {
                    this->m_GLSDLDevice.SetTexture(tmu, nullptr);
                }
            }

            break;
        }

        case GxRs_TexGen0:
        case GxRs_TexGen1:
        case GxRs_TexGen2:
        case GxRs_TexGen3:
        case GxRs_TexGen4:
        case GxRs_TexGen5:
        case GxRs_TexGen6:
        case GxRs_TexGen7: {
            // TODO

            break;
        }

        case GxRs_VertexShader: {
            auto vs = static_cast<CGxShader*>(static_cast<void*>(state->m_value));
            this->IShaderBindVertex(vs);

            break;
        }

        case GxRs_PixelShader: {
            auto ps = static_cast<CGxShader*>(static_cast<void*>(state->m_value));
            this->IShaderBindPixel(ps);

            break;
        }

        default: {
            fprintf(stderr, "Unhandled render state in CGxDeviceGLSDL::IRsSendToHw: %i\n", which);

            break;
        }
    }
}

void CGxDeviceGLSDL::ISceneBegin() {
    if (this->m_context) {
        this->ShaderConstantsClear();
    }

    // TODO GameMovie::ReadFrame(this);
}

void CGxDeviceGLSDL::ISetCaps(const CGxFormat& format) {
    // TODO fill in proper implementation

    this->m_caps.m_pixelCenterOnEdge = 1;
    this->m_caps.m_texelCenterOnEdge = 1;

    this->m_caps.m_colorFormat = GxCF_rgba;

    this->m_caps.m_generateMipMaps = 1;

    this->m_caps.int10 = 1;

    this->m_caps.m_texFmt[GxTex_Dxt1] = 1;
    this->m_caps.m_texFmt[GxTex_Dxt3] = 1;
    this->m_caps.m_texFmt[GxTex_Dxt5] = 1;

    this->m_caps.m_shaderTargets[GxSh_Vertex] = GxShVS_arbvp1;
    this->m_caps.m_shaderTargets[GxSh_Pixel] = GxShPS_arbfp1;

    this->m_caps.m_texFilterAnisotropic = 1;
    this->m_caps.m_maxTexAnisotropy = 16;

    this->m_caps.m_texTarget[GxTex_2d] = 1;
    this->m_caps.m_texTarget[GxTex_CubeMap] = 1;
    this->m_caps.m_texTarget[GxTex_Rectangle] = 1;
    this->m_caps.m_texTarget[GxTex_NonPow2] = 1;

    this->m_caps.m_texMaxSize[GxTex_2d] = 4096;
    this->m_caps.m_texMaxSize[GxTex_CubeMap] = 4096;
    this->m_caps.m_texMaxSize[GxTex_Rectangle] = 4096;
    this->m_caps.m_texMaxSize[GxTex_NonPow2] = 4096;

    // TODO
}

void CGxDeviceGLSDL::IShaderBindPixel(CGxShader* sh) {
    CGxDevice::IShaderBind();

    if (!sh) {
        this->m_GLSDLDevice.SetShader(GLShader::ePixelShader, nullptr);
        return;
    }

    if (!sh->loaded) {
        this->IShaderCreatePixel(sh);
    }

    BLIZZARD_ASSERT(sh->Valid());

    this->m_GLSDLDevice.SetShader(GLShader::ePixelShader, static_cast<GLShader*>(sh->apiSpecific));
}

void CGxDeviceGLSDL::IShaderBindVertex(CGxShader* sh) {
    CGxDevice::IShaderBind();

    if (!sh) {
        this->m_GLSDLDevice.SetShader(GLShader::eVertexShader, nullptr);
        return;
    }

    if (!sh->loaded) {
        this->IShaderCreateVertex(sh);
    }

    BLIZZARD_ASSERT(sh->Valid());

    this->m_GLSDLDevice.SetShader(GLShader::eVertexShader, static_cast<GLShader*>(sh->apiSpecific));
}

void CGxDeviceGLSDL::IShaderConstantsFlush() {
    // Vertex shader constants
    auto vsConst = &CGxDevice::s_shadowConstants[1];
    if (vsConst->unk2 <= vsConst->unk1) {
        this->m_GLSDLDevice.SetShaderConstants(
            GLShader::eVertexShader,
            vsConst->unk2,
            reinterpret_cast<float*>(&vsConst->constants[vsConst->unk2]),
            vsConst->unk1 - vsConst->unk2 + 1
        );
    }
    vsConst->unk2 = 255;
    vsConst->unk1 = 0;

    // Pixel shader constants
    auto psConst = &CGxDevice::s_shadowConstants[0];
    if (psConst->unk2 <= psConst->unk1) {
        this->m_GLSDLDevice.SetShaderConstants(
            GLShader::ePixelShader,
            psConst->unk2,
            reinterpret_cast<float*>(&psConst->constants[psConst->unk2]),
            psConst->unk1 - psConst->unk2 + 1
        );
    }
    psConst->unk2 = 255;
    psConst->unk1 = 0;
}

void CGxDeviceGLSDL::IShaderCreate(CGxShader* sh) {
    if (sh->target == GxSh_Vertex) {
        this->IShaderCreateVertex(sh);
    } else if (sh->target == GxSh_Pixel) {
        this->IShaderCreatePixel(sh);
    }
}

void CGxDeviceGLSDL::IShaderCreatePixel(CGxShader* ps) {
    BLIZZARD_ASSERT(!ps->loaded);

    ps->loaded = 1;
    ps->valid = 0;

    unsigned char* codeStr = ps->code.m_data;
    uint32_t codeLen = ps->code.Count();

    if (codeLen) {
        this->PatchPixelShader(ps);

        GLShader* glShader = this->m_GLSDLDevice.CreateShader(
            GLShader::ShaderType::ePixelShader,
            codeStr,
            codeLen,
            ps->m_key.m_str
        );

        glShader->Compile(nullptr);

        ps->apiSpecific = glShader;
        ps->valid = 1;
    }
}

void CGxDeviceGLSDL::IShaderCreateVertex(CGxShader* vs) {
    BLIZZARD_ASSERT(!vs->loaded);

    vs->loaded = 1;
    vs->valid = 0;

    unsigned char* code = vs->code.m_data;
    uint32_t codeLen = vs->code.Count();

    if (codeLen) {
        this->PatchVertexShader(vs);

        GLShader* glShader = this->m_GLSDLDevice.CreateShader(
            GLShader::ShaderType::eVertexShader,
            code,
            codeLen,
            vs->m_key.m_str
        );

        glShader->Compile(nullptr);

        vs->apiSpecific = glShader;
        vs->valid = 1;
    }
}

void CGxDeviceGLSDL::IStateSetGLSDLDefaults() {
    this->IRsForceUpdate();
    this->IRsSync(0);

    // TODO a1->unk8 = -1;
    // TODO a1->unk13 = 0;

    this->m_GLSDLDevice.SetIndexBuffer(nullptr);

    GLTexture* a2 = nullptr;
    GLTexture* a3 = nullptr;
    // TODO this->m_GLSDLDevice.GetBackBuffer(&a2, &a3, nullptr);
    // TODO this->gllunk2[12] = a2->GetMipmap(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    // TODO this->gllunk2[13] = a3->GetMipmap(0, GL_TEXTURE_CUBE_MAP_POSITIVE_X);

    this->m_GLSDLDevice.SetFogParam(GL_FOG_DENSITY, 0.0f);
    // TODO this->InitLights();

    // TODO this->gllunk2[10] = 0;

    this->ISceneBegin();
}

void CGxDeviceGLSDL::IStateSync() {
    this->IShaderConstantsFlush();
    this->IRsSync(0);
    this->IStateSyncXforms();
    this->IStateSyncLights();
    this->IStateSyncEnables();
    this->IStateSyncMaterial();
    // TODO clip plane
    this->IStateSyncScissorRect();
    this->IStateSyncVertexPtrs();
    this->IStateSyncIndexPtr();
}

void CGxDeviceGLSDL::IStateSyncEnables() {
    // TODO
}

void CGxDeviceGLSDL::IStateSyncIndexPtr() {
    if (this->m_primIndexDirty) {
        this->m_primIndexDirty = 0;
        this->m_GLSDLDevice.SetIndexBuffer(static_cast<GLBuffer*>(this->m_primIndexBuf->m_pool->m_apiSpecific));
    }
}

void CGxDeviceGLSDL::IStateSyncLights() {
    // TODO
}

void CGxDeviceGLSDL::IStateSyncMaterial() {
    // TODO
}

void CGxDeviceGLSDL::IStateSyncScissorRect() {
    // TODO
}

void CGxDeviceGLSDL::IStateSyncVertexPtrs() {
    static int32_t gxAttribSlot[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

    static int32_t gxAttribTypeToGLSDLAttribType[] = {
        GLVT_UBYTE4N,
        GLVT_UBYTE4,
        GLVT_UBYTE4N,
        GLVT_FLOAT2,
        GLVT_FLOAT3,
        GLVT_SHORT2N,
        GLVT_FLOAT1
    };

    if (this->m_primVertexFormat < GxVertexBufferFormats_Last) {
        GLVertexFormat* glFormat = &this->m_glFormats[this->m_primVertexFormat];

        if (glFormat->m_Size) {
            this->m_GLSDLDevice.SetVertexFormat(glFormat);

            this->m_GLSDLDevice.SetVertexBuffer(
                0,
                static_cast<GLBuffer*>(this->m_primVertexBuf->m_pool->m_apiSpecific),
                this->m_primVertexBuf->m_index,
                this->m_primVertexSize
            );

            return;
        }
    }

    GLVertexFormat* glFormat = nullptr;

    if (this->m_primVertexFormat < GxVertexBufferFormats_Last) {
        glFormat = &this->m_glFormats[this->m_primVertexFormat];
    }

    for (int32_t i = 0; i < GxVAs_Last; i++) {
        if ((1 << i) & this->m_primVertexMask) {
            CGxVertexAttrib* attrib = &this->m_primVertexFormatAttrib[i];

            uint32_t j = glFormat->m_Size;

            glFormat->m_Attribs[j].stream = 0;
            glFormat->m_Attribs[j].slot = gxAttribSlot[attrib->attrib];
            glFormat->m_Attribs[j].type = gxAttribTypeToGLSDLAttribType[attrib->type];
            glFormat->m_Attribs[j].offset = attrib->offset;

            glFormat->m_Size++;
        }
    }

    this->m_GLSDLDevice.SetVertexFormat(glFormat);

    this->m_GLSDLDevice.SetVertexBuffer(
        0,
        static_cast<GLBuffer*>(this->m_primVertexBuf->m_pool->m_apiSpecific),
        this->m_primVertexBuf->m_index,
        this->m_primVertexSize
    );
}

void CGxDeviceGLSDL::IStateSyncXforms() {
    // TODO this->IXformSetWorld();
    // TODO this->IXformSetTex();

    this->IXformSetViewport();
}

void CGxDeviceGLSDL::ITexCreate(CGxTex* gxTex) {
    uint32_t width, height, baseMip, mipCount;
    this->ITexWHDStartEnd(gxTex, width, height, baseMip, mipCount);

    uint32_t flags = 0;

    flags |= gxTex->m_flags.m_renderTarget ? GLTFLAG_RENDERTARGET : 0x0;
    flags |= gxTex->m_format == GxTex_D24X8 ? GLTFLAG_DEPTH : 0x0;
    flags |= gxTex->m_flags.m_generateMipMaps ? GLTFLAG_AUTOGEN_MIPMAP : 0x0;

    // TODO
    // 2 bits, appearing at m_maxAnisotropy + 1 bit
    // flags |= gxTex->m_flags & 0xC000 : GLTFLAG_READ_ACCESS : 0x0;

    if (gxTex->m_target == 1) {
        gxTex->m_apiSpecificData = this->m_GLSDLDevice.CreateTextureCubeMap(
            width,
            mipCount - baseMip,
            CGxDeviceGLSDL::s_gxTexFmtToGLSDLFmt[gxTex->m_format],
            flags
        );
    } else {
        gxTex->m_apiSpecificData = this->m_GLSDLDevice.CreateTexture2D(
            width,
            height,
            mipCount - baseMip,
            CGxDeviceGLSDL::s_gxTexFmtToGLSDLFmt[gxTex->m_format],
            flags
        );

        if (gxTex->m_flags.m_renderTarget) {
            gxTex->m_apiSpecificData2 = this->m_GLSDLDevice.CreateTexture2D(
                width,
                height,
                mipCount - baseMip,
                GLTF_D24,
                flags | GLTFLAG_DEPTH
            );
        }
    }

    this->ITexSetFlags(gxTex);

    // TODO
    // CGxDevice::IOnAcquireCPUTex();

    gxTex->m_needsCreation = 0;
}

void CGxDeviceGLSDL::ITexMarkAsUpdated(CGxTex* texId) {
    if (texId->m_needsFlagUpdate && !texId->m_needsCreation && (texId->m_apiSpecificData || texId->m_apiSpecificData2)) {
        this->ITexSetFlags(texId);
    }

    if (texId->m_needsUpdate/* TODO && (this + 4000) */) {
        if (texId->m_needsCreation || (!texId->m_apiSpecificData && !texId->m_apiSpecificData2)) {
            this->ITexCreate(texId);
        }

        if (!texId->m_needsCreation && (texId->m_apiSpecificData || texId->m_apiSpecificData2)) {
            if (texId->m_userFunc) {
                this->ITexUpload(texId);
            }

            CGxDevice::ITexMarkAsUpdated(texId);
        }
    }
}

void CGxDeviceGLSDL::ITexSetFlags(CGxTex* texId) {
    GLTexture* v2 = static_cast<GLTexture*>(texId->m_apiSpecificData);
    GLSDLTexSetFlags(texId, v2);
    texId->m_needsFlagUpdate = 0;
}

void CGxDeviceGLSDL::ITexUpload(CGxTex* texId) {
    uint32_t texelStrideInBytes;
    const void* texels = nullptr;

    texId->m_userFunc(
        GxTex_Lock,
        texId->m_width,
        texId->m_height,
        0,
        0,
        texId->m_userArg,
        texelStrideInBytes,
        texels
    );

    uint32_t width, height, baseMip, mipCount;
    this->ITexWHDStartEnd(texId, width, height, baseMip, mipCount);

    int32_t numFace = texId->m_target == GxTex_CubeMap ? 6 : 1;

    for (int32_t face = 0; face < numFace; face++) {
        for (int32_t mipLevel = baseMip; mipLevel < mipCount; mipLevel++) {
            texels = nullptr;

            texId->m_userFunc(
                GxTex_Latch,
                texId->m_width >> mipLevel,
                texId->m_height >> mipLevel,
                face,
                mipLevel,
                texId->m_userArg,
                texelStrideInBytes,
                texels
            );

            BLIZZARD_ASSERT(texels != nullptr || texId->m_flags.m_renderTarget);

            if (!texId->m_flags.m_renderTarget) {
                GLMipmap* mipmap = static_cast<GLTexture*>(texId->m_apiSpecificData)->GetMipmap(mipLevel - baseMip, CGxDeviceGLSDL::s_glCubeMapFaces[face]);

                CiRect lockRect = {
                    texId->m_updateRect.minY >> mipLevel,
                    texId->m_updateRect.minX >> mipLevel,
                    (texId->m_updateRect.maxY >> mipLevel) + 1,
                    (texId->m_updateRect.maxX >> mipLevel) + 1
                };

                lockRect.maxY = std::min(lockRect.maxY, static_cast<int32_t>(height));
                lockRect.maxX = std::min(lockRect.maxX, static_cast<int32_t>(width));

                BLIZZARD_ASSERT(lockRect.minX >= 0 && lockRect.maxX <= static_cast<int32_t>(width));
                BLIZZARD_ASSERT(lockRect.minY >= 0 && lockRect.maxY <= static_cast<int32_t>(height));

                GLRect rect = {
                    lockRect.minX,
                    lockRect.minY,
                    lockRect.maxX - lockRect.minX,
                    lockRect.maxY - lockRect.minY
                };

                if (mipmap->GetFormatInfo().m_IsCompressed) {
                    int32_t v17 = rect.width + (rect.left & 3);
                    rect.left &= 0xFFFFFFFC;
                    rect.width = (v17 + 3) & 0xFFFFFFFC;
                    rect.width = std::max(rect.width, 4);

                    int32_t v20 = rect.height + (rect.top & 3);
                    rect.top &= 0xFFFFFFFC;
                    rect.height = (v20 + 3) & 0xFFFFFFFC;
                    rect.height = std::max(rect.height, 4);
                }

                const void* src = texels;

                if (texId->m_flags.m_bit15) {
                    src = texels;
                } else if (texId->m_dataFormat == GxTex_Dxt1 || texId->m_dataFormat == GxTex_Dxt3 || texId->m_dataFormat == GxTex_Dxt5) {
                    uint32_t bytesPerBlock = CGxDevice::s_texFormatBytesPerBlock[texId->m_dataFormat];
                    uint32_t offset = (bytesPerBlock * (rect.left >> 2)) + (texelStrideInBytes * (rect.top >> 2));

                    src = static_cast<const char*>(texels) + offset;
                } else {
                    uint32_t bitDepth = CGxDevice::s_texFormatBitDepth[texId->m_dataFormat];
                    uint32_t offset = ((bitDepth * rect.left) >> 3) + (texelStrideInBytes * rect.top);

                    src = static_cast<const char*>(texels) + offset;
                }

                uint32_t dstStride = mipmap->GetPitch();

                BlitFormat dstFmt = GxGetBlitFormat(texId->m_format);

                void* dst = mipmap->Map(GL_WRITE_ONLY, &rect);

                BlitFormat srcFmt = GxGetBlitFormat(texId->m_dataFormat);

                C2iVector size = { rect.width, rect.height };

                Blit(size, BlitAlpha_0, src, texelStrideInBytes, srcFmt, dst, dstStride, dstFmt);

                mipmap->Unmap();
            }

            width = std::max(width >> 1, 1u);
            height = std::max(height >> 1, 1u);
        }
    }

    texId->m_userFunc(
        GxTex_Unlock,
        texId->m_width,
        texId->m_height,
        0,
        0,
        texId->m_userArg,
        texelStrideInBytes,
        texels
    );
}

void CGxDeviceGLSDL::IXformSetProjection(const C44Matrix& matrix) {
    C44Matrix glSDLMat = matrix;

    if (!this->MasterEnable(GxMasterEnable_NormalProjection) && matrix.d3 != 1.0f) {
        C44Matrix shrink = {
            0.2f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.2f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.2f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        glSDLMat = glSDLMat * shrink;
    }

    this->m_GLSDLDevice.SetTransform(GL_PROJECTION, reinterpret_cast<float*>(&glSDLMat));

    this->m_projNative = glSDLMat;

    this->m_projNative.a1 *= -1.0f;
    this->m_projNative.b1 *= -1.0f;
    this->m_projNative.c1 *= -1.0f;
    this->m_projNative.d1 *= -1.0f;
}

void CGxDeviceGLSDL::IXformSetView(const C44Matrix& matrix) {
    this->m_GLSDLDevice.SetTransform('VIEW', reinterpret_cast<const float*>(&matrix));
}

void CGxDeviceGLSDL::IXformSetViewport() {
    auto window = this->DeviceCurWindow();

    GLRect viewport = {
        static_cast<int32_t>((this->m_viewport.x.l * window.maxX) + 0.5f),
        static_cast<int32_t>(((1.0f - this->m_viewport.y.h) * window.maxY) + 0.5f),
        static_cast<int32_t>(((this->m_viewport.x.h - this->m_viewport.x.l) * window.maxX) + 0.5f),
        static_cast<int32_t>(((this->m_viewport.y.h - this->m_viewport.y.l) * window.maxY) + 0.5f),
    };

    this->m_GLSDLDevice.SetViewport(viewport, this->m_viewport.z.l, this->m_viewport.z.h);

    // TODO (this + 4020) = 0;
}

void CGxDeviceGLSDL::PatchPixelShader(CGxShader* ps) {
    // TODO
}

void CGxDeviceGLSDL::PatchVertexShader(CGxShader* vs) {
    if (vs->patched) {
        return;
    }

    vs->patched = 1;

    // TODO properly calculate v2
    int32_t v2 = 192;

    char buf[64];

    for (int32_t i = 0; i < vs->code.Count(); i++) {
        char* str = reinterpret_cast<char*>(&vs->code[i]);

        if (!SStrCmp(str, ".local", 6)) {
            memcpy(str, ".env  ", 6);

            int v16, v12, n;
            if (str[6] == '[' && sscanf(&str[4], " [%d..%d]%n", &v16, &v12, &n) == 2 && v12 == 93) {
                SStrPrintf(buf, sizeof(buf), "[%d..%d]          ", v16, v2 - 1);
                memcpy(&str[4], buf, n);
            }
        }

        int32_t v13, n;
        if (sscanf(str, "PARAM c[%d] = { %n", &v13, &n) == 1 && v13 == 94) {
            SStrPrintf(buf, sizeof(buf), "PARAM c[%d]={              ", v2);
            memcpy(str, buf, n);
        }
    }
}

void CGxDeviceGLSDL::PoolSizeSet(CGxPool* pool, uint32_t size) {
    if (pool->m_usage == GxPoolUsage_Stream) {
        pool->unk1C = 0;
    }

    GLBuffer* buffer = reinterpret_cast<GLBuffer*>(pool->m_apiSpecific);

    if (buffer) {
        delete buffer;
    }

    pool->m_size = size;

    pool->m_apiSpecific = this->m_GLSDLDevice.CreateBuffer(
        CGxDeviceGLSDL::s_poolTarget2BufferType[pool->m_target],
        size,
        nullptr,
        CGxDeviceGLSDL::s_poolUsage2BufferUsage[pool->m_usage],
        CGxDeviceGLSDL::s_poolTarget2BufferFormat[pool->m_target]
    );
}

void CGxDeviceGLSDL::Resize(uint32_t width, uint32_t height) {
    this->m_GLSDLDevice.Resize(width, height);

    CRect rect = { 0.0f, 0.0f, static_cast<float>(height), static_cast<float>(width) };
    this->DeviceSetDefWindow(rect);
}

void CGxDeviceGLSDL::SceneClear(uint32_t mask, CImVector color) {
    CGxDevice::SceneClear(mask, color);

    if (!this->m_context) {
        return;
    }

    uint32_t glMask = 0x0;

    if (mask & 0x1) {
        glMask |= GL_COLOR_BUFFER_BIT;
    }

    if (mask & 0x2) {
        glMask |= GL_DEPTH_BUFFER_BIT;
    }

    this->IXformSetViewport();
    this->IStateSyncScissorRect();

    GLColor4f glColor = {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f
    };

    this->m_GLSDLDevice.Clear(glMask, glColor, 1.0, 0);
}

void CGxDeviceGLSDL::ScenePresent() {
    if (this->m_context) {
        // TODO

        CGxDevice::ScenePresent();

        // TODO

        this->m_GLSDLDevice.Swap();

        // TODO

        CGxDevice::ShaderConstantsClear();
    }

    // TODO
}

void CGxDeviceGLSDL::ShaderCreate(CGxShader* shaders[], EGxShTarget target, const char* a4, const char* a5, int32_t permutations) {
    CGxDevice::ShaderCreate(shaders, target, a4, a5, permutations);

    if (permutations == 1 && !shaders[0]->loaded) {
        if (target == GxSh_Vertex) {
            this->IShaderCreateVertex(shaders[0]);
        } else if (target == GxSh_Pixel) {
            this->IShaderCreatePixel(shaders[0]);
        }
    }
}

int32_t CGxDeviceGLSDL::StereoEnabled() {
    return 0;
}

void CGxDeviceGLSDL::TexDestroy(CGxTex* texId) {
    BLIZZARD_ASSERT(texId);

    auto texture = static_cast<GLTexture*>(texId->m_apiSpecificData);

    if (texture) {
        texture->Release();
        texId->m_apiSpecificData = nullptr;
    }

    // TODO CGxDevice::IOnReleaseCPUTex(texId);
    CGxDevice::TexDestroy(texId);
}

void CGxDeviceGLSDL::XformSetProjection(const C44Matrix& matrix) {
    CGxDevice::XformSetProjection(matrix);
    this->IXformSetProjection(matrix);
}

void CGxDeviceGLSDL::XformSetView(const C44Matrix& matrix) {
    CGxDevice::XformSetView(matrix);
    this->IXformSetView(matrix);
}
