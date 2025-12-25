#include "gx/mtl/CGxDeviceMTL.hpp"
#include "app/mac/View.h"
#include "gx/Buffer.hpp"
#include "gx/CGxBatch.hpp"
#include "gx/Window.hpp"
#include "gx/texture/CGxTex.hpp"
#include "math/Utils.hpp"
#include "util/Autorelease.hpp"
#include <storm/Memory.hpp>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

namespace {
    MTLPrimitiveType MtlPrimitiveType(EGxPrim prim) {
        switch (prim) {
            case GxPrim_Points:
                return MTLPrimitiveTypePoint;
            case GxPrim_Lines:
                return MTLPrimitiveTypeLine;
            case GxPrim_LineStrip:
                return MTLPrimitiveTypeLineStrip;
            case GxPrim_Triangles:
                return MTLPrimitiveTypeTriangle;
            case GxPrim_TriangleStrip:
                return MTLPrimitiveTypeTriangleStrip;
            case GxPrim_TriangleFan:
                return MTLPrimitiveTypeTriangle;
            default:
                return MTLPrimitiveTypeTriangle;
        }
    }

    bool GxTexIsCompressed(EGxTexFormat format) {
        return format == GxTex_Dxt1 || format == GxTex_Dxt3 || format == GxTex_Dxt5;
    }

    MTLPixelFormat MtlPixelFormatForGx(EGxTexFormat format) {
        // Note: GxTex format names describe the packed 32-bit value layout (MSB to LSB).
        // On little-endian, this means the byte order in memory is reversed.
        // Metal pixel formats describe byte order in memory.
        // GxTex_Argb8888: packed as 0xAARRGGBB, bytes in memory: BB GG RR AA -> use BGRA
        // GxTex_Abgr8888: packed as 0xAABBGGRR, bytes in memory: RR GG BB AA -> use RGBA
        switch (format) {
            case GxTex_Abgr8888:
                return MTLPixelFormatRGBA8Unorm;
            case GxTex_Argb8888:
                return MTLPixelFormatBGRA8Unorm;
            case GxTex_Argb4444:
                // Convert ARGB4444 to RGBA8 during upload for simpler handling
                return MTLPixelFormatRGBA8Unorm;
            case GxTex_Argb1555:
                return MTLPixelFormatBGR5A1Unorm;
            case GxTex_Rgb565:
                return MTLPixelFormatB5G6R5Unorm;
            case GxTex_Dxt1:
                return MTLPixelFormatBC1_RGBA;
            case GxTex_Dxt3:
                return MTLPixelFormatBC2_RGBA;
            case GxTex_Dxt5:
                return MTLPixelFormatBC3_RGBA;
            case GxTex_Uv88:
                return MTLPixelFormatRG8Unorm;
            case GxTex_Gr1616F:
                return MTLPixelFormatRG16Float;
            case GxTex_R32F:
                return MTLPixelFormatR32Float;
            default:
                return MTLPixelFormatInvalid;
        }
    }

    MTLBlendFactor MtlBlendFactorForSrc(EGxBlend blend) {
        switch (blend) {
            case GxBlend_Alpha:
            case GxBlend_Add:
            case GxBlend_SrcAlphaOpaque:
                return MTLBlendFactorSourceAlpha;
            case GxBlend_Mod:
            case GxBlend_Mod2x:
            case GxBlend_ModAdd:
                return MTLBlendFactorDestinationColor;
            case GxBlend_InvSrcAlphaAdd:
            case GxBlend_InvSrcAlphaOpaque:
                return MTLBlendFactorOneMinusSourceAlpha;
            case GxBlend_NoAlphaAdd:
            case GxBlend_Opaque:
            case GxBlend_AlphaKey:
                return MTLBlendFactorOne;
            case GxBlend_ConstantAlpha:
                return MTLBlendFactorBlendAlpha;
            default:
                return MTLBlendFactorOne;
        }
    }

    MTLBlendFactor MtlBlendFactorForDst(EGxBlend blend) {
        switch (blend) {
            case GxBlend_Alpha:
                return MTLBlendFactorOneMinusSourceAlpha;
            case GxBlend_Add:
            case GxBlend_ModAdd:
            case GxBlend_InvSrcAlphaAdd:
            case GxBlend_NoAlphaAdd:
            case GxBlend_ConstantAlpha:
                return MTLBlendFactorOne;
            case GxBlend_Mod:
                return MTLBlendFactorZero;
            case GxBlend_Mod2x:
                return MTLBlendFactorSourceColor;
            case GxBlend_InvSrcAlphaOpaque:
            case GxBlend_SrcAlphaOpaque:
            case GxBlend_Opaque:
            case GxBlend_AlphaKey:
                return MTLBlendFactorZero;
            default:
                return MTLBlendFactorZero;
        }
    }

    void ApplyViewport(CGxDeviceMTL* device, id<MTLRenderCommandEncoder> encoder, id<MTLTexture> texture) {
        const double width = texture.width;
        const double height = texture.height;
        double x = (device->m_viewport.x.l * width) + 0.5;
        double y = ((1.0 - device->m_viewport.y.h) * height) + 0.5;
        double w = (device->m_viewport.x.h * width) - x + 0.5;
        double h = ((1.0 - device->m_viewport.y.l) * height) - y + 0.5;

        if (w < 0.0) {
            w = 0.0;
        }
        if (h < 0.0) {
            h = 0.0;
        }

        MTLViewport viewport = { x, y, w, h, device->m_viewport.z.l, device->m_viewport.z.h };
        [encoder setViewport:viewport];

        NSUInteger sx = x < 0.0 ? 0 : static_cast<NSUInteger>(x);
        NSUInteger sy = y < 0.0 ? 0 : static_cast<NSUInteger>(y);
        NSUInteger sw = static_cast<NSUInteger>(w);
        NSUInteger sh = static_cast<NSUInteger>(h);

        if (sx > texture.width) {
            sx = texture.width;
        }
        if (sy > texture.height) {
            sy = texture.height;
        }
        if (sx + sw > texture.width) {
            sw = texture.width - sx;
        }
        if (sy + sh > texture.height) {
            sh = texture.height - sy;
        }

        MTLScissorRect scissor = { sx, sy, sw, sh };
        [encoder setScissorRect:scissor];

        device->intF6C = 0;
    }

    const char kMetalShaderSource[] =
        "#include <metal_stdlib>\n"
        "using namespace metal;\n"
        "struct VertexColorIn {\n"
        "    float3 position [[attribute(0)]];\n"
        "    float4 color [[attribute(1)]];\n"
        "};\n"
        "struct VertexTexIn {\n"
        "    float3 position [[attribute(0)]];\n"
        "    float2 texcoord [[attribute(1)]];\n"
        "};\n"
        "struct VertexTex2In {\n"
        "    float3 position [[attribute(0)]];\n"
        "    float2 texcoord [[attribute(1)]];\n"
        "    float2 texcoord1 [[attribute(2)]];\n"
        "};\n"
        "struct VertexColorTexIn {\n"
        "    float3 position [[attribute(0)]];\n"
        "    float4 color [[attribute(1)]];\n"
        "    float2 texcoord [[attribute(2)]];\n"
        "};\n"
        "struct VertexColorTex2In {\n"
        "    float3 position [[attribute(0)]];\n"
        "    float4 color [[attribute(1)]];\n"
        "    float2 texcoord [[attribute(2)]];\n"
        "    float2 texcoord1 [[attribute(3)]];\n"
        "};\n"
        "struct VertexSolidIn {\n"
        "    float3 position [[attribute(0)]];\n"
        "};\n"
        "struct VertexSkinIn {\n"
        "    float3 position [[attribute(0)]];\n"
        "    float4 weights [[attribute(1)]];\n"
        "    uchar4 indices [[attribute(2)]];\n"
        "};\n"
        "struct VertexSkinTexIn {\n"
        "    float3 position [[attribute(0)]];\n"
        "    float4 weights [[attribute(1)]];\n"
        "    uchar4 indices [[attribute(2)]];\n"
        "    float2 texcoord [[attribute(3)]];\n"
        "};\n"
        "struct VertexSkinTex2In {\n"
        "    float3 position [[attribute(0)]];\n"
        "    float4 weights [[attribute(1)]];\n"
        "    uchar4 indices [[attribute(2)]];\n"
        "    float2 texcoord [[attribute(3)]];\n"
        "    float2 texcoord1 [[attribute(4)]];\n"
        "};\n"
        "struct VertexOut {\n"
        "    float4 position [[position]];\n"
        "    float pointSize [[point_size]];\n"
        "    float4 color;\n"
        "    float2 texcoord;\n"
        "    float2 texcoord1;\n"
        "    float viewZ;\n"
        "};\n"
        "struct PSConstants {\n"
        "    float alphaRef;\n"
        "    float fogStart;\n"
        "    float fogEnd;\n"
        "    float fogEnabled;\n"
        "    float4 color;\n"
        "    float4 fogColor;\n"
        "};\n"
        "struct VSConstants {\n"
        "    float4x4 mvp;\n"
        "    float pointSize;\n"
        "    float pad[3];\n"
        "};\n"
        "vertex VertexOut vs_color(VertexColorIn in [[stage_in]], constant VSConstants& c [[buffer(1)]]) {\n"
        "    VertexOut out;\n"
        "    float4 pos4 = c.mvp * float4(in.position, 1.0);\n"
        "    out.position = pos4;\n"
        "    out.color = in.color;\n"
        "    out.texcoord = float2(0.0, 0.0);\n"
        "    out.texcoord1 = float2(0.0, 0.0);\n"
        "    out.viewZ = pos4.w;\n"
        "    out.pointSize = c.pointSize;\n"
        "    return out;\n"
        "}\n"
        "vertex VertexOut vs_tex(VertexTexIn in [[stage_in]], constant VSConstants& c [[buffer(1)]]) {\n"
        "    VertexOut out;\n"
        "    float4 pos4 = c.mvp * float4(in.position, 1.0);\n"
        "    out.position = pos4;\n"
        "    out.color = float4(1.0, 1.0, 1.0, 1.0);\n"
        "    out.texcoord = in.texcoord;\n"
        "    out.texcoord1 = in.texcoord;\n"
        "    out.viewZ = pos4.w;\n"
        "    out.pointSize = c.pointSize;\n"
        "    return out;\n"
        "}\n"
        "vertex VertexOut vs_tex2(VertexTex2In in [[stage_in]], constant VSConstants& c [[buffer(1)]]) {\n"
        "    VertexOut out;\n"
        "    float4 pos4 = c.mvp * float4(in.position, 1.0);\n"
        "    out.position = pos4;\n"
        "    out.color = float4(1.0, 1.0, 1.0, 1.0);\n"
        "    out.texcoord = in.texcoord;\n"
        "    out.texcoord1 = in.texcoord1;\n"
        "    out.viewZ = pos4.w;\n"
        "    out.pointSize = c.pointSize;\n"
        "    return out;\n"
        "}\n"
        "vertex VertexOut vs_color_tex(VertexColorTexIn in [[stage_in]], constant VSConstants& c [[buffer(1)]]) {\n"
        "    VertexOut out;\n"
        "    float4 pos4 = c.mvp * float4(in.position, 1.0);\n"
        "    out.position = pos4;\n"
        "    out.color = in.color;\n"
        "    out.texcoord = in.texcoord;\n"
        "    out.texcoord1 = in.texcoord;\n"
        "    out.viewZ = pos4.w;\n"
        "    out.pointSize = c.pointSize;\n"
        "    return out;\n"
        "}\n"
        "vertex VertexOut vs_color_tex2(VertexColorTex2In in [[stage_in]], constant VSConstants& c [[buffer(1)]]) {\n"
        "    VertexOut out;\n"
        "    float4 pos4 = c.mvp * float4(in.position, 1.0);\n"
        "    out.position = pos4;\n"
        "    out.color = in.color;\n"
        "    out.texcoord = in.texcoord;\n"
        "    out.texcoord1 = in.texcoord1;\n"
        "    out.viewZ = pos4.w;\n"
        "    out.pointSize = c.pointSize;\n"
        "    return out;\n"
        "}\n"
        "vertex VertexOut vs_solid(VertexSolidIn in [[stage_in]], constant VSConstants& c [[buffer(1)]]) {\n"
        "    VertexOut out;\n"
        "    float4 pos4 = c.mvp * float4(in.position, 1.0);\n"
        "    out.position = pos4;\n"
        "    out.color = float4(1.0, 1.0, 1.0, 1.0);\n"
        "    out.texcoord = float2(0.0, 0.0);\n"
        "    out.texcoord1 = float2(0.0, 0.0);\n"
        "    out.viewZ = pos4.w;\n"
        "    out.pointSize = c.pointSize;\n"
        "    return out;\n"
        "}\n"
        "vertex VertexOut vs_skin(VertexSkinIn in [[stage_in]], constant VSConstants& c [[buffer(1)]], constant float4* vc [[buffer(2)]]) {\n"
        "    VertexOut out;\n"
        "    float4 pos = float4(in.position, 1.0);\n"
        "    float4 w = in.weights;\n"
        "    uint4 idx = uint4(in.indices);\n"
        "    float3 skinned = float3(0.0);\n"
        "    for (uint i = 0; i < 4; ++i) {\n"
        "        uint base = 31 + idx[i] * 3;\n"
        "        float4 c0 = vc[base + 0];\n"
        "        float4 c1 = vc[base + 1];\n"
        "        float4 c2 = vc[base + 2];\n"
        "        float3 p;\n"
        "        p.x = dot(pos, c0);\n"
        "        p.y = dot(pos, c1);\n"
        "        p.z = dot(pos, c2);\n"
        "        skinned += p * w[i];\n"
        "    }\n"
        "    float4 pos4 = c.mvp * float4(skinned, 1.0);\n"
        "    out.position = pos4;\n"
        "    // Read diffuse (vc[28]) and emissive (vc[29]) for color animation\n"
        "    float4 diffuse = vc[28];\n"
        "    float4 emissive = vc[29];\n"
        "    float4 color = diffuse + emissive;\n"
        "    out.color = (diffuse.x > 1e30) ? float4(1.0, 1.0, 1.0, 1.0) : color;\n"
        "    out.texcoord = float2(0.0, 0.0);\n"
        "    out.texcoord1 = float2(0.0, 0.0);\n"
        "    out.viewZ = pos4.w;\n"
        "    out.pointSize = c.pointSize;\n"
        "    return out;\n"
        "}\n"
        "vertex VertexOut vs_skin_tex(VertexSkinTexIn in [[stage_in]], constant VSConstants& c [[buffer(1)]], constant float4* vc [[buffer(2)]]) {\n"
        "    VertexOut out;\n"
        "    float4 pos = float4(in.position, 1.0);\n"
        "    float4 w = in.weights;\n"
        "    uint4 idx = uint4(in.indices);\n"
        "    float3 skinned = float3(0.0);\n"
        "    for (uint i = 0; i < 4; ++i) {\n"
        "        uint base = 31 + idx[i] * 3;\n"
        "        float4 c0 = vc[base + 0];\n"
        "        float4 c1 = vc[base + 1];\n"
        "        float4 c2 = vc[base + 2];\n"
        "        float3 p;\n"
        "        p.x = dot(pos, c0);\n"
        "        p.y = dot(pos, c1);\n"
        "        p.z = dot(pos, c2);\n"
        "        skinned += p * w[i];\n"
        "    }\n"
        "    float4 pos4 = c.mvp * float4(skinned, 1.0);\n"
        "    out.position = pos4;\n"
        "    // Read diffuse (vc[28]) and emissive (vc[29]) for color animation\n"
        "    float4 diffuse = vc[28];\n"
        "    float4 emissive = vc[29];\n"
        "    float4 color = diffuse + emissive;\n"
        "    out.color = (diffuse.x > 1e30) ? float4(1.0, 1.0, 1.0, 1.0) : color;\n"
        "    out.texcoord = in.texcoord;\n"
        "    out.texcoord1 = in.texcoord;\n"
        "    out.viewZ = pos4.w;\n"
        "    out.pointSize = c.pointSize;\n"
        "    return out;\n"
        "}\n"
        "vertex VertexOut vs_skin_tex2(VertexSkinTex2In in [[stage_in]], constant VSConstants& c [[buffer(1)]], constant float4* vc [[buffer(2)]]) {\n"
        "    VertexOut out;\n"
        "    float4 pos = float4(in.position, 1.0);\n"
        "    float4 w = in.weights;\n"
        "    uint4 idx = uint4(in.indices);\n"
        "    float3 skinned = float3(0.0);\n"
        "    for (uint i = 0; i < 4; ++i) {\n"
        "        uint base = 31 + idx[i] * 3;\n"
        "        float4 c0 = vc[base + 0];\n"
        "        float4 c1 = vc[base + 1];\n"
        "        float4 c2 = vc[base + 2];\n"
        "        float3 p;\n"
        "        p.x = dot(pos, c0);\n"
        "        p.y = dot(pos, c1);\n"
        "        p.z = dot(pos, c2);\n"
        "        skinned += p * w[i];\n"
        "    }\n"
        "    float4 pos4 = c.mvp * float4(skinned, 1.0);\n"
        "    out.position = pos4;\n"
        "    // Read diffuse (vc[28]) and emissive (vc[29]) for color animation\n"
        "    float4 diffuse = vc[28];\n"
        "    float4 emissive = vc[29];\n"
        "    float4 color = diffuse + emissive;\n"
        "    out.color = (diffuse.x > 1e30) ? float4(1.0, 1.0, 1.0, 1.0) : color;\n"
        "    out.texcoord = in.texcoord;\n"
        "    out.texcoord1 = in.texcoord1;\n"
        "    out.viewZ = pos4.w;\n"
        "    out.pointSize = c.pointSize;\n"
        "    return out;\n"
        "}\n"
        "float4 applyFog(float4 color, float viewZ, constant PSConstants& ps) {\n"
        "    if (ps.fogEnabled > 0.0 && ps.fogEnd > ps.fogStart) {\n"
        "        float fogFactor = saturate((ps.fogEnd - viewZ) / (ps.fogEnd - ps.fogStart));\n"
        "        color.rgb = mix(ps.fogColor.rgb, color.rgb, fogFactor);\n"
        "    }\n"
        "    return color;\n"
        "}\n"
        "fragment float4 ps_main(VertexOut in [[stage_in]], constant PSConstants& ps [[buffer(0)]]) {\n"
        "    float4 color = in.color * ps.color;\n"
        "    if (ps.alphaRef > 0.0 && color.a < ps.alphaRef) {\n"
        "        discard_fragment();\n"
        "    }\n"
        "    return applyFog(color, in.viewZ, ps);\n"
        "}\n"
        "fragment float4 ps_tex1(VertexOut in [[stage_in]], texture2d<float> tex0 [[texture(0)]], sampler samp0 [[sampler(0)]], constant PSConstants& ps [[buffer(0)]]) {\n"
        "    float4 color = tex0.sample(samp0, in.texcoord) * in.color * ps.color;\n"
        "    if (ps.alphaRef > 0.0 && color.a < ps.alphaRef) {\n"
        "        discard_fragment();\n"
        "    }\n"
        "    return applyFog(color, in.viewZ, ps);\n"
        "}\n"
        "fragment float4 ps_tex(VertexOut in [[stage_in]], texture2d<float> tex0 [[texture(0)]], sampler samp0 [[sampler(0)]], texture2d<float> tex1 [[texture(1)]], sampler samp1 [[sampler(1)]], constant PSConstants& ps [[buffer(0)]]) {\n"
        "    float4 color = tex0.sample(samp0, in.texcoord) * tex1.sample(samp1, in.texcoord1) * in.color * ps.color;\n"
        "    if (ps.alphaRef > 0.0 && color.a < ps.alphaRef) {\n"
        "        discard_fragment();\n"
        "    }\n"
        "    return applyFog(color, in.viewZ, ps);\n"
        "}\n";
}

CGxDeviceMTL::CGxDeviceMTL() : CGxDevice() {
    this->m_api = GxApi_Metal;
    this->m_caps.m_colorFormat = GxCF_rgba;

    this->DeviceCreatePools();
    this->DeviceCreateStreamBufs();
}

void CGxDeviceMTL::ITexMarkAsUpdated(CGxTex* texId) {
    if (texId->m_needsFlagUpdate && texId->m_apiSpecificData2) {
        auto sampler = (id<MTLSamplerState>)texId->m_apiSpecificData2;
        [sampler release];
        texId->m_apiSpecificData2 = nullptr;
    }

    if (texId->m_needsUpdate) {
        if (texId->m_needsCreation || !texId->m_apiSpecificData) {
            this->ITexCreate(texId);
        }

        if (!texId->m_needsCreation && texId->m_apiSpecificData && texId->m_userFunc) {
            this->ITexUpload(texId);
        }

        CGxDevice::ITexMarkAsUpdated(texId);
    }
}

void CGxDeviceMTL::IRsSendToHw(EGxRenderState which) {
    auto* state = &this->m_appRenderStates[which];
    
    switch (which) {
        // Texture states - mark textures as needing update
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
            CGxTex* texture = static_cast<CGxTex*>(static_cast<void*>(state->m_value));
            if (texture) {
                this->ITexMarkAsUpdated(texture);
            }
            break;
        }
        
        // States handled at draw time - no immediate GPU action needed
        case GxRs_BlendingMode:
        case GxRs_AlphaRef:
        case GxRs_DepthTest:
        case GxRs_DepthFunc:
        case GxRs_DepthWrite:
        case GxRs_Culling:
        case GxRs_Fog:
        case GxRs_FogStart:
        case GxRs_FogEnd:
        case GxRs_FogColor:
        case GxRs_Lighting:
        case GxRs_VertexShader:
        case GxRs_PixelShader:
        case GxRs_ColorWrite:
        case GxRs_ScissorTest:
            // Metal applies these during Draw() call
            break;
            
        // States not yet implemented
        case GxRs_PolygonOffset:
        case GxRs_MatDiffuse:
        case GxRs_MatEmissive:
        case GxRs_MatSpecular:
        case GxRs_MatSpecularExp:
        case GxRs_NormalizeNormals:
        case GxRs_ClipPlaneMask:
        case GxRs_Multisample:
        case GxRs_ColorOp0:
        case GxRs_ColorOp1:
        case GxRs_ColorOp2:
        case GxRs_ColorOp3:
        case GxRs_ColorOp4:
        case GxRs_ColorOp5:
        case GxRs_ColorOp6:
        case GxRs_ColorOp7:
        case GxRs_AlphaOp0:
        case GxRs_AlphaOp1:
        case GxRs_AlphaOp2:
        case GxRs_AlphaOp3:
        case GxRs_AlphaOp4:
        case GxRs_AlphaOp5:
        case GxRs_AlphaOp6:
        case GxRs_AlphaOp7:
        case GxRs_TexGen0:
        case GxRs_TexGen1:
        case GxRs_TexGen2:
        case GxRs_TexGen3:
        case GxRs_TexGen4:
        case GxRs_TexGen5:
        case GxRs_TexGen6:
        case GxRs_TexGen7:
        case GxRs_PointScale:
        case GxRs_PointScaleAttenuation:
        case GxRs_PointScaleMin:
        case GxRs_PointScaleMax:
        case GxRs_PointSprite:
        case GxRs_ColorMaterial:
            // Not implemented in Metal backend
            break;
            
        default:
            // Unknown state
            break;
    }
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

    this->m_caps.m_shaderTargets[GxSh_Vertex] = GxShVS_arbvp1;
    this->m_caps.m_shaderTargets[GxSh_Pixel] = GxShPS_arbfp1;

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

void CGxDeviceMTL::EnsureLibrary() {
    if (this->m_shaderLibrary || !this->m_device) {
        return;
    }

    auto device = (id<MTLDevice>)this->m_device;

    NSString* source = [[NSString alloc] initWithUTF8String:kMetalShaderSource];
    NSError* error = nil;
    id<MTLLibrary> library = [device newLibraryWithSource:source options:nil error:&error];
    [source release];

    if (!library) {
        return;
    }

    this->m_shaderLibrary = library;
}

void* CGxDeviceMTL::GetPipeline(EGxVertexBufferFormat format, bool useColor, bool useSkin, bool useTex, int32_t blendMode) {
    if (format >= GxVertexBufferFormats_Last || !this->m_device || !this->m_layer) {
        return nullptr;
    }

    if (blendMode < 0 || blendMode >= GxBlends_Last) {
        blendMode = GxBlend_Opaque;
    }

    this->EnsureLibrary();
    if (!this->m_shaderLibrary) {
        return nullptr;
    }

    auto device = (id<MTLDevice>)this->m_device;
    auto layer = (CAMetalLayer*)this->m_layer;
    auto library = (id<MTLLibrary>)this->m_shaderLibrary;

    NSString* vsName = nil;
    NSString* psName = nil;

    bool hasPosition = false;
    bool hasColor = false;
    bool hasBlendWeight = false;
    bool hasBlendIndices = false;
    bool hasTex0 = false;
    bool hasTex1 = false;
    uint32_t posOffset = 0;
    uint32_t colorOffset = 0;
    uint32_t blendWeightOffset = 0;
    uint32_t blendIndexOffset = 0;
    uint32_t tex0Offset = 0;
    uint32_t tex1Offset = 0;

    auto& bufDesc = Buffer::s_vertexBufDesc[format];

    for (uint32_t i = 0; i < bufDesc.attribCount; ++i) {
        const auto& attrib = bufDesc.attribs[i];

        if (attrib.attrib == GxVA_Position) {
            posOffset = attrib.offset;
            hasPosition = true;
        } else if (attrib.attrib == GxVA_BlendWeight) {
            blendWeightOffset = attrib.offset;
            hasBlendWeight = true;
        } else if (attrib.attrib == GxVA_BlendIndices) {
            blendIndexOffset = attrib.offset;
            hasBlendIndices = true;
        } else if (attrib.attrib == GxVA_Color0) {
            colorOffset = attrib.offset;
            hasColor = true;
        } else if (attrib.attrib == GxVA_TexCoord0) {
            tex0Offset = attrib.offset;
            hasTex0 = true;
        } else if (attrib.attrib == GxVA_TexCoord1) {
            tex1Offset = attrib.offset;
            hasTex1 = true;
        }
    }

    bool useTexPipeline = useTex && hasTex0;
    bool useTex2Pipeline = useTexPipeline && hasTex1;

    void* (*pipelineTable)[GxBlends_Last] = nullptr;
    if (useSkin) {
        if (useTex2Pipeline) {
            pipelineTable = this->m_pipelineSkinTex2;
        } else if (useTexPipeline) {
            pipelineTable = this->m_pipelineSkinTex;
        } else {
            pipelineTable = this->m_pipelineSkin;
        }
    } else if (useColor) {
        if (useTex2Pipeline) {
            pipelineTable = this->m_pipelineColorTex2;
        } else if (useTexPipeline) {
            pipelineTable = this->m_pipelineColorTex;
        } else {
            pipelineTable = this->m_pipelineColor;
        }
    } else {
        if (useTex2Pipeline) {
            pipelineTable = this->m_pipelineSolidTex2;
        } else if (useTexPipeline) {
            pipelineTable = this->m_pipelineSolidTex;
        } else {
            pipelineTable = this->m_pipelineSolid;
        }
    }

    if (pipelineTable[format][blendMode]) {
        return pipelineTable[format][blendMode];
    }

    if (useSkin) {
        if (useTex2Pipeline) {
            vsName = @"vs_skin_tex2";
        } else {
            vsName = useTexPipeline ? @"vs_skin_tex" : @"vs_skin";
        }
    } else if (useColor) {
        if (useTex2Pipeline) {
            vsName = @"vs_color_tex2";
        } else {
            vsName = useTexPipeline ? @"vs_color_tex" : @"vs_color";
        }
    } else {
        if (useTex2Pipeline) {
            vsName = @"vs_tex2";
        } else {
            vsName = useTexPipeline ? @"vs_tex" : @"vs_solid";
        }
    }

    // Select pixel shader based on texture usage:
    // - ps_main: no textures
    // - ps_tex1: single texture (tex0 only)
    // - ps_tex: two textures (tex0 * tex1)
    if (useTex2Pipeline) {
        psName = @"ps_tex";
    } else if (useTexPipeline) {
        psName = @"ps_tex1";
    } else {
        psName = @"ps_main";
    }

    id<MTLFunction> vs = [library newFunctionWithName:vsName];
    id<MTLFunction> ps = [library newFunctionWithName:psName];

    if (!vs || !ps) {
        if (vs) {
            [vs release];
        }
        if (ps) {
            [ps release];
        }
        return nullptr;
    }

    auto desc = [MTLRenderPipelineDescriptor new];
    desc.vertexFunction = vs;
    desc.fragmentFunction = ps;
    desc.colorAttachments[0].pixelFormat = layer.pixelFormat;
    desc.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;

    auto vdesc = [MTLVertexDescriptor vertexDescriptor];

    if (!hasPosition || (useColor && !hasColor) || (useSkin && (!hasBlendWeight || !hasBlendIndices)) || (useTexPipeline && !hasTex0) || (useTex2Pipeline && !hasTex1)) {
        [desc release];
        [vs release];
        [ps release];
        return nullptr;
    }

    vdesc.attributes[0].format = MTLVertexFormatFloat3;
    vdesc.attributes[0].offset = posOffset;
    vdesc.attributes[0].bufferIndex = 0;

    if (useSkin) {
        vdesc.attributes[1].format = MTLVertexFormatUChar4Normalized;
        vdesc.attributes[1].offset = blendWeightOffset;
        vdesc.attributes[1].bufferIndex = 0;
        vdesc.attributes[2].format = MTLVertexFormatUChar4;
        vdesc.attributes[2].offset = blendIndexOffset;
        vdesc.attributes[2].bufferIndex = 0;
        if (useTexPipeline) {
            vdesc.attributes[3].format = MTLVertexFormatFloat2;
            vdesc.attributes[3].offset = tex0Offset;
            vdesc.attributes[3].bufferIndex = 0;
            if (useTex2Pipeline) {
                vdesc.attributes[4].format = MTLVertexFormatFloat2;
                vdesc.attributes[4].offset = tex1Offset;
                vdesc.attributes[4].bufferIndex = 0;
            }
        }
    } else if (useColor) {
        vdesc.attributes[1].format = MTLVertexFormatUChar4Normalized;
        vdesc.attributes[1].offset = colorOffset;
        vdesc.attributes[1].bufferIndex = 0;
        if (useTexPipeline) {
            vdesc.attributes[2].format = MTLVertexFormatFloat2;
            vdesc.attributes[2].offset = tex0Offset;
            vdesc.attributes[2].bufferIndex = 0;
            if (useTex2Pipeline) {
                vdesc.attributes[3].format = MTLVertexFormatFloat2;
                vdesc.attributes[3].offset = tex1Offset;
                vdesc.attributes[3].bufferIndex = 0;
            }
        }
    } else if (useTexPipeline) {
        vdesc.attributes[1].format = MTLVertexFormatFloat2;
        vdesc.attributes[1].offset = tex0Offset;
        vdesc.attributes[1].bufferIndex = 0;
        if (useTex2Pipeline) {
            vdesc.attributes[2].format = MTLVertexFormatFloat2;
            vdesc.attributes[2].offset = tex1Offset;
            vdesc.attributes[2].bufferIndex = 0;
        }
    }

    vdesc.layouts[0].stride = bufDesc.size;
    vdesc.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
    vdesc.layouts[0].stepRate = 1;

    desc.vertexDescriptor = vdesc;

    bool enableBlend = blendMode > GxBlend_AlphaKey;
    desc.colorAttachments[0].blendingEnabled = enableBlend ? YES : NO;
    if (enableBlend) {
        desc.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
        desc.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
        desc.colorAttachments[0].sourceRGBBlendFactor = MtlBlendFactorForSrc(static_cast<EGxBlend>(blendMode));
        desc.colorAttachments[0].destinationRGBBlendFactor = MtlBlendFactorForDst(static_cast<EGxBlend>(blendMode));
        desc.colorAttachments[0].sourceAlphaBlendFactor = desc.colorAttachments[0].sourceRGBBlendFactor;
        desc.colorAttachments[0].destinationAlphaBlendFactor = desc.colorAttachments[0].destinationRGBBlendFactor;
    }
    
    // ColorWrite is applied via writeMask; for now set all channels enabled.
    // A full implementation would need separate pipelines per write mask value.
    desc.colorAttachments[0].writeMask = MTLColorWriteMaskAll;

    NSError* error = nil;
    id<MTLRenderPipelineState> pipeline = [device newRenderPipelineStateWithDescriptor:desc error:&error];
    [desc release];
    [vs release];
    [ps release];

    if (!pipeline) {
        return nullptr;
    }

    pipelineTable[format][blendMode] = pipeline;
    return pipeline;
}

void* CGxDeviceMTL::GetPoolBuffer(CGxPool* pool) {
    if (!pool || !this->m_device) {
        return nullptr;
    }

    auto device = (id<MTLDevice>)this->m_device;
    auto buffer = (id<MTLBuffer>)pool->m_apiSpecific;

    if (!pool->m_mem) {
        pool->m_mem = SMemAlloc(pool->m_size, __FILE__, __LINE__, 0x0);
    }

    if (!buffer || buffer.length < static_cast<NSUInteger>(pool->m_size)) {
        if (buffer) {
            [buffer release];
        }

        buffer = [device newBufferWithBytesNoCopy:pool->m_mem
                                           length:pool->m_size
                                          options:MTLResourceStorageModeShared
                                      deallocator:nil];
        pool->m_apiSpecific = buffer;
    }

    return buffer;
}

void CGxDeviceMTL::BeginFrame() {
    if (this->m_frameEncoder || !this->m_device || !this->m_commandQueue || !this->m_layer) {
        return;
    }

    auto commandQueue = (id<MTLCommandQueue>)this->m_commandQueue;
    auto layer = (CAMetalLayer*)this->m_layer;

    id<CAMetalDrawable> drawable = [layer nextDrawable];
    if (!drawable) {
        return;
    }

    const bool clearRequested = (this->m_clearMask & 0x1) != 0;
    const uint8_t r = (this->m_clearColor >> 16) & 0xFF;
    const uint8_t g = (this->m_clearColor >> 8) & 0xFF;
    const uint8_t b = this->m_clearColor & 0xFF;
    const uint8_t a = (this->m_clearColor >> 24) & 0xFF;

    auto pass = [MTLRenderPassDescriptor renderPassDescriptor];
    pass.colorAttachments[0].texture = drawable.texture;
    pass.colorAttachments[0].loadAction = MTLLoadActionClear;
    pass.colorAttachments[0].storeAction = MTLStoreActionStore;
    pass.colorAttachments[0].clearColor = MTLClearColorMake(
        clearRequested ? r / 255.0f : 0.0f,
        clearRequested ? g / 255.0f : 0.0f,
        clearRequested ? b / 255.0f : 0.0f,
        clearRequested ? a / 255.0f : 1.0f
    );

    this->EnsureDepthTexture(drawable.texture.width, drawable.texture.height);
    if (this->m_depthTexture) {
        auto depthTex = (id<MTLTexture>)this->m_depthTexture;
        pass.depthAttachment.texture = depthTex;
        pass.depthAttachment.loadAction = (this->m_clearMask & 0x2) ? MTLLoadActionClear : MTLLoadActionLoad;
        pass.depthAttachment.storeAction = MTLStoreActionStore;
        pass.depthAttachment.clearDepth = 1.0;
    }

    id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
    id<MTLRenderCommandEncoder> encoder = [commandBuffer renderCommandEncoderWithDescriptor:pass];

    ApplyViewport(this, encoder, drawable.texture);

    this->m_frameCommandBuffer = commandBuffer;
    this->m_frameEncoder = encoder;
    this->m_frameDrawable = drawable;
    this->m_frameHasDraw = 0;
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

    System_Autorelease::ScopedPool autorelease;

    if (!this->m_frameEncoder) {
        this->BeginFrame();
    }

    if (!this->m_frameEncoder || !this->m_frameCommandBuffer || !this->m_frameDrawable) {
        return;
    }

    auto encoder = (id<MTLRenderCommandEncoder>)this->m_frameEncoder;
    if (this->intF6C) {
        auto drawable = (id<CAMetalDrawable>)this->m_frameDrawable;
        ApplyViewport(this, encoder, drawable.texture);
    }
    auto commandBuffer = (id<MTLCommandBuffer>)this->m_frameCommandBuffer;
    auto drawable = (id<CAMetalDrawable>)this->m_frameDrawable;

    [encoder endEncoding];
    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];

    this->m_frameCommandBuffer = nullptr;
    this->m_frameEncoder = nullptr;
    this->m_frameDrawable = nullptr;
}

void CGxDeviceMTL::Draw(CGxBatch* batch, int32_t indexed) {
    if (!this->m_context || !batch) {
        return;
    }

    this->BeginFrame();

    if (!this->m_frameEncoder) {
        return;
    }

    auto encoder = (id<MTLRenderCommandEncoder>)this->m_frameEncoder;

    auto vertexBuf = this->m_primVertexFormatBuf[GxVA_Position];
    if (!vertexBuf) {
        return;
    }

    // Check ColorWrite - skip rendering if completely disabled
    int32_t colorWrite = static_cast<int32_t>(this->m_appRenderStates[GxRs_ColorWrite].m_value);
    bool colorWriteEnabled = this->MasterEnable(GxMasterEnable_ColorWrite);
    if (!colorWriteEnabled || colorWrite == 0) {
        return; // No color channels will be written
    }

    bool useColor = (this->m_primVertexMask & GxPrim_Color0) != 0;
    bool useSkin = this->m_primVertexFormat == GxVBF_PBNT2;
    bool useTex = false;
    
    // Check for texture coordinates in two ways:
    // 1. Standard vertex buffer format - check buffer descriptor
    // 2. Custom vertex format (m_primVertexFormat == Last) - check m_primVertexMask
    if (this->m_primVertexFormat < GxVertexBufferFormats_Last) {
        const auto& bufDesc = Buffer::s_vertexBufDesc[this->m_primVertexFormat];
        for (uint32_t i = 0; i < bufDesc.attribCount; ++i) {
            if (bufDesc.attribs[i].attrib == GxVA_TexCoord0) {
                useTex = true;
                break;
            }
        }
    }
    // Also check the primitive vertex mask for TexCoord0 bit (covers custom formats)
    if (!useTex && (this->m_primVertexMask & (1 << GxVA_TexCoord0))) {
        useTex = true;
    }
    
    // Debug logging - enable with WHOA_GX_MTL_LOG_DRAW=1
    static bool s_logDraw = std::getenv("WHOA_GX_MTL_LOG_DRAW") != nullptr;
    static int s_drawCount = 0;
    if (s_logDraw && s_drawCount < 500) {
        auto texState = static_cast<CGxTex*>(static_cast<void*>(this->m_appRenderStates[GxRs_Texture0].m_value));
        int32_t blendModeDbg = static_cast<int32_t>(this->m_appRenderStates[GxRs_BlendingMode].m_value);
        fprintf(stderr, "[MTL Draw #%d] fmt=%d mask=0x%x useTex=%d useColor=%d blend=%d tex0=%p count=%d\n",
                s_drawCount++, this->m_primVertexFormat, this->m_primVertexMask, useTex, useColor, blendModeDbg, texState, batch->m_count);
    }
    
    int32_t blendMode = static_cast<int32_t>(this->m_appRenderStates[GxRs_BlendingMode].m_value);
    auto pipeline = (id<MTLRenderPipelineState>)this->GetPipeline(this->m_primVertexFormat, useColor, useSkin, useTex, blendMode);
    if (!pipeline && useColor) {
        useColor = false;
        pipeline = (id<MTLRenderPipelineState>)this->GetPipeline(this->m_primVertexFormat, false, useSkin, useTex, blendMode);
    }

    if (!pipeline) {
        return;
    }

    auto mtlVertexBuf = (id<MTLBuffer>)this->GetPoolBuffer(vertexBuf->m_pool);
    if (!mtlVertexBuf) {
        return;
    }

    // VSConstants struct matches the Metal shader's VSConstants
    struct MtlVSConstants {
        C44Matrix mvp;
        float pointSize;
        float pad[3];
    } vsConsts;
    vsConsts.pointSize = 1.0f;  // Default point size
    bool useShaderMvp = false;

    // Restore shader constant usage for standard UI formats.
    if (!useSkin) {
        auto vsState = static_cast<CGxShader*>(static_cast<void*>(this->m_appRenderStates[GxRs_VertexShader].m_value));
        if (vsState) {
            static bool s_logConstants = std::getenv("WHOA_GX_MTL_LOG_CONSTANTS") != nullptr;
            if (s_logConstants) {
                 const auto& c = CGxDevice::s_shadowConstants[1].constants;
                 fprintf(stderr, "VS Consts: [0] %f %f %f %f\n", c[0].x, c[0].y, c[0].z, c[0].w);
                 fprintf(stderr, "           [1] %f %f %f %f\n", c[1].x, c[1].y, c[1].z, c[1].w);
                 fprintf(stderr, "           [2] %f %f %f %f\n", c[2].x, c[2].y, c[2].z, c[2].w);
                 fprintf(stderr, "           [3] %f %f %f %f\n", c[3].x, c[3].y, c[3].z, c[3].w);
            }

            // NOTE: VS constant MVP from c[0-3] is disabled - it causes rendering issues.
            // The game may store MVP in constants for some shaders, but the format varies.
            // For now, we rely on the transform stack (world * view * proj).
        }
    }

    if (!useShaderMvp) {
        if (useSkin) {
            vsConsts.mvp = this->m_projNative;
        } else {
            const auto& world = this->m_xforms[GxXform_World].TopConst();
            const auto& view = this->m_xforms[GxXform_View].TopConst();
            vsConsts.mvp = (world * view) * this->m_projNative;
        }
    }
    
    // Get point size from render state if point sprites are enabled
    int32_t pointSprite = static_cast<int32_t>(this->m_appRenderStates[GxRs_PointSprite].m_value);
    if (pointSprite) {
        float pointScale = static_cast<float>(static_cast<int32_t>(this->m_appRenderStates[GxRs_PointScale].m_value));
        if (pointScale > 0.0f) {
            vsConsts.pointSize = pointScale;
        }
    }

    [encoder setRenderPipelineState:pipeline];
    if (blendMode == GxBlend_ConstantAlpha) {
        float alphaRef = static_cast<float>(static_cast<int32_t>(this->m_appRenderStates[GxRs_AlphaRef].m_value)) / 255.0f;
        [encoder setBlendColorRed:1.0 green:1.0 blue:1.0 alpha:alphaRef];
    }

    int32_t depthTest = static_cast<int32_t>(this->m_appRenderStates[GxRs_DepthTest].m_value);
    uint32_t depthFunc = static_cast<uint32_t>(this->m_appRenderStates[GxRs_DepthFunc].m_value);
    int32_t depthWrite = static_cast<int32_t>(this->m_appRenderStates[GxRs_DepthWrite].m_value);

    bool depthTestEnabled = this->MasterEnable(GxMasterEnable_DepthTest) && depthTest;
    bool depthWriteEnabled = this->MasterEnable(GxMasterEnable_DepthWrite) && depthWrite;
    auto depthState = (id<MTLDepthStencilState>)this->GetDepthState(depthTestEnabled, depthWriteEnabled, depthFunc);
    if (depthState) {
        [encoder setDepthStencilState:depthState];
    }
    
    // Polygon offset (depth bias) for decals and overlapping geometry
    int32_t polygonOffset = static_cast<int32_t>(this->m_appRenderStates[GxRs_PolygonOffset].m_value);
    if (polygonOffset != 0) {
        // Apply depth bias to push geometry slightly back, avoiding z-fighting
        float slopeScale = static_cast<float>(polygonOffset) * 2.0f;
        float units = static_cast<float>(polygonOffset);
        [encoder setDepthBias:units slopeScale:slopeScale clamp:0.0f];
    } else {
        [encoder setDepthBias:0.0f slopeScale:0.0f clamp:0.0f];
    }

    int32_t cullMode = static_cast<int32_t>(this->m_appRenderStates[GxRs_Culling].m_value);
    if (cullMode == 0) {
        [encoder setCullMode:MTLCullModeNone];
    } else {
        [encoder setCullMode:MTLCullModeBack];
        // Swap winding: OpenGL uses opposite convention from what we originally had
        [encoder setFrontFacingWinding:(cullMode == 1) ? MTLWindingCounterClockwise : MTLWindingClockwise];
    }
    
    // Scissor test: when disabled, use full framebuffer; when enabled, use viewport scissor
    int32_t scissorTest = static_cast<int32_t>(this->m_appRenderStates[GxRs_ScissorTest].m_value);
    if (!scissorTest) {
        auto layer = (CAMetalLayer*)this->m_layer;
        CGSize size = layer.drawableSize;
        MTLScissorRect fullRect = { 0, 0, static_cast<NSUInteger>(size.width), static_cast<NSUInteger>(size.height) };
        [encoder setScissorRect:fullRect];
    }
    // When scissor test is enabled, the scissor rect is set by IStateSyncScissorRect via BeginFrame

    [encoder setVertexBuffer:mtlVertexBuf offset:vertexBuf->m_index atIndex:0];
    [encoder setVertexBytes:&vsConsts length:sizeof(vsConsts) atIndex:1];
    if (useSkin) {
        [encoder setVertexBytes:CGxDevice::s_shadowConstants[1].constants
                         length:sizeof(CGxDevice::s_shadowConstants[1].constants)
                        atIndex:2];
    }
    if (useTex) {
        auto texState = static_cast<CGxTex*>(static_cast<void*>(this->m_appRenderStates[GxRs_Texture0].m_value));
        auto texture = (id<MTLTexture>)this->GetTexture(texState);
        if (!texture) {
            this->EnsureFallbackTexture();
            texture = (id<MTLTexture>)this->m_fallbackTexture;
        }
        auto sampler = (id<MTLSamplerState>)this->GetSampler(texState);
        if (!sampler) {
            sampler = (id<MTLSamplerState>)this->m_fallbackSampler;
        }
        
        // Debug tex binding
        if (s_logDraw && s_drawCount <= 10) {
            EGxTexFormat gxfmt = texState ? texState->m_format : GxTex_Unknown;
            fprintf(stderr, "  [Tex0] ptr=%p mtlTex=%p dim=%lux%lu gxfmt=%d pixelFmt=%lu\n",
                    texState, texture, texture.width, texture.height, gxfmt, (unsigned long)texture.pixelFormat);
        }
        
        [encoder setFragmentTexture:texture atIndex:0];
        [encoder setFragmentSamplerState:sampler atIndex:0];

        auto texState1 = static_cast<CGxTex*>(static_cast<void*>(this->m_appRenderStates[GxRs_Texture1].m_value));
        auto texture1 = (id<MTLTexture>)this->GetTexture(texState1);
        if (!texture1) {
            this->EnsureFallbackTexture();
            texture1 = (id<MTLTexture>)this->m_fallbackTexture;
        }
        auto sampler1 = (id<MTLSamplerState>)this->GetSampler(texState1);
        if (!sampler1) {
            sampler1 = (id<MTLSamplerState>)this->m_fallbackSampler;
        }
        [encoder setFragmentTexture:texture1 atIndex:1];
        [encoder setFragmentSamplerState:sampler1 atIndex:1];
    }

    struct MtlPSConstants {
        float alphaRef;
        float fogStart;
        float fogEnd;
        float fogEnabled;
        float color[4];
        float fogColor[4];
    } psConsts;

    psConsts.alphaRef = static_cast<float>(static_cast<int32_t>(this->m_appRenderStates[GxRs_AlphaRef].m_value)) / 255.0f;
    
    // Read fog render states
    int32_t fogEnabled = static_cast<int32_t>(this->m_appRenderStates[GxRs_Fog].m_value);
    psConsts.fogEnabled = fogEnabled ? 1.0f : 0.0f;
    psConsts.fogStart = static_cast<float>(static_cast<int32_t>(this->m_appRenderStates[GxRs_FogStart].m_value));
    psConsts.fogEnd = static_cast<float>(static_cast<int32_t>(this->m_appRenderStates[GxRs_FogEnd].m_value));
    
    // Fog color is stored as CImVector (packed ARGB)
    uint32_t fogColorPacked = static_cast<uint32_t>(this->m_appRenderStates[GxRs_FogColor].m_value);
    psConsts.fogColor[0] = static_cast<float>((fogColorPacked >> 16) & 0xFF) / 255.0f;  // R
    psConsts.fogColor[1] = static_cast<float>((fogColorPacked >> 8) & 0xFF) / 255.0f;   // G
    psConsts.fogColor[2] = static_cast<float>((fogColorPacked >> 0) & 0xFF) / 255.0f;   // B
    psConsts.fogColor[3] = static_cast<float>((fogColorPacked >> 24) & 0xFF) / 255.0f;  // A

    // Default color to white
    psConsts.color[0] = 1.0f; psConsts.color[1] = 1.0f; psConsts.color[2] = 1.0f; psConsts.color[3] = 1.0f;

    // Apply pixel shader constants if a pixel shader is active AND Lighting is DISABLED.
    // UI rendering typically disables lighting and uses c[0] for color modulation.
    // World rendering enables lighting and uses c[0] for lighting parameters (which we don't support yet in this simple backend),
    // so we default to white for world objects to avoid blown-out colors.
    auto psState = static_cast<CGxShader*>(static_cast<void*>(this->m_appRenderStates[GxRs_PixelShader].m_value));
    int32_t lighting = static_cast<int32_t>(this->m_appRenderStates[GxRs_Lighting].m_value);

    if (psState) {
        static bool s_logConstants = std::getenv("WHOA_GX_MTL_LOG_CONSTANTS") != nullptr;
        if (s_logConstants) {
             const auto& c = CGxDevice::s_shadowConstants[0].constants;
             fprintf(stderr, "PS Consts (Light=%d, Blend=%d): [0] %f %f %f %f\n", lighting, blendMode, c[0].x, c[0].y, c[0].z, c[0].w);
        }

        if (lighting == 0) {
            const auto& c = CGxDevice::s_shadowConstants[0].constants[0];
            // Apply shader constants if they appear initialized (not FLT_MAX from ShaderConstantsClear).
            // Zero values are valid - they mean "don't modulate" or intentional transparency.
            // Color values are typically in 0-1 range, using 2.0f threshold for HDR margin.
            if (std::abs(c.x) <= 2.0f && std::abs(c.y) <= 2.0f && std::abs(c.z) <= 2.0f && std::abs(c.w) <= 2.0f) {
                psConsts.color[0] = c.x;
                psConsts.color[1] = c.y;
                psConsts.color[2] = c.z;
                psConsts.color[3] = c.w;
            }
        }
    }

    // Debug PS constants color
    if (s_logDraw && s_drawCount <= 10) {
        int32_t depthTest = static_cast<int32_t>(this->m_appRenderStates[GxRs_DepthTest].m_value);
        int32_t depthWrite = static_cast<int32_t>(this->m_appRenderStates[GxRs_DepthWrite].m_value);
        fprintf(stderr, "  [PSConsts] color=(%.2f, %.2f, %.2f, %.2f) alphaRef=%.3f lighting=%d depthTest=%d depthWrite=%d\n",
                psConsts.color[0], psConsts.color[1], psConsts.color[2], psConsts.color[3],
                psConsts.alphaRef, lighting, depthTest, depthWrite);
    }

    [encoder setFragmentBytes:&psConsts length:sizeof(psConsts) atIndex:0];

    auto primitive = MtlPrimitiveType(batch->m_primType);

    if (indexed) {
        auto indexBuf = this->m_primIndexBuf;
        if (!indexBuf || indexBuf->m_itemSize != 2) {
            return;
        }

        auto mtlIndexBuf = (id<MTLBuffer>)this->GetPoolBuffer(indexBuf->m_pool);
        if (!mtlIndexBuf) {
            return;
        }

        const NSUInteger indexOffset = indexBuf->m_index + (batch->m_start * indexBuf->m_itemSize);
        [encoder drawIndexedPrimitives:primitive
                            indexCount:batch->m_count
                             indexType:MTLIndexTypeUInt16
                           indexBuffer:mtlIndexBuf
                     indexBufferOffset:indexOffset];
    } else {
        [encoder drawPrimitives:primitive
                    vertexStart:batch->m_start
                    vertexCount:batch->m_count];
    }

    this->m_frameHasDraw = 1;
}

void CGxDeviceMTL::PoolSizeSet(CGxPool* pool, uint32_t size) {
    if (!pool || pool->m_size >= static_cast<int32_t>(size)) {
        return;
    }

    pool->m_size = static_cast<int32_t>(size);
    pool->unk1C = 0;

    if (pool->m_apiSpecific) {
        auto buffer = (id<MTLBuffer>)pool->m_apiSpecific;
        [buffer release];
        pool->m_apiSpecific = nullptr;
    }

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
    if (texId && texId->m_apiSpecificData) {
        auto texture = (id<MTLTexture>)texId->m_apiSpecificData;
        [texture release];
        texId->m_apiSpecificData = nullptr;
    }
    if (texId && texId->m_apiSpecificData2) {
        auto sampler = (id<MTLSamplerState>)texId->m_apiSpecificData2;
        [sampler release];
        texId->m_apiSpecificData2 = nullptr;
    }
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

void CGxDeviceMTL::XformSetProjection(const C44Matrix& matrix) {
    CGxDevice::XformSetProjection(matrix);

    C44Matrix projNative = matrix;

    if (NotEqual(projNative.c3, 1.0f, WHOA_EPSILON_1) && NotEqual(projNative.c3, 0.0f, WHOA_EPSILON_1)) {
        projNative = projNative * (1.0f / projNative.c3);
    }

    if (projNative.d3 == 0.0f) {
        auto v5 = -(projNative.d2 / (projNative.c2 + 1.0f));
        auto v6 = -(projNative.d2 / (projNative.c2 - 1.0f));
        projNative.c2 = v6 / (v6 - v5);
        projNative.d2 = v6 * v5 / (v5 - v6);
    } else {
        auto v8 = 1.0f / projNative.c2;
        auto v9 = (-1.0f - projNative.d2) * v8;
        auto v10 = v8 * (1.0f - projNative.d2);
        projNative.c2 = 1.0f / (v10 - v9);
        projNative.d2 = v9 / (v9 - v10);
    }

    if (!this->MasterEnable(GxMasterEnable_NormalProjection) && projNative.d3 != 1.0f) {
        C44Matrix shrink = {
            0.2f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.2f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.2f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        projNative = projNative * shrink;
    }

    this->m_xforms[GxXform_Projection].m_dirty = 1;
    this->m_projNative = projNative;
}

void CGxDeviceMTL::EnsureDepthTexture(uint32_t width, uint32_t height) {
    if (!this->m_device || (this->m_depthTexture && this->m_depthWidth == width && this->m_depthHeight == height)) {
        return;
    }

    if (this->m_depthTexture) {
        auto depthTex = (id<MTLTexture>)this->m_depthTexture;
        [depthTex release];
        this->m_depthTexture = nullptr;
    }

    auto device = (id<MTLDevice>)this->m_device;
    auto desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float width:width height:height mipmapped:NO];
    desc.usage = MTLTextureUsageRenderTarget;
    desc.storageMode = MTLStorageModePrivate;

    id<MTLTexture> depthTex = [device newTextureWithDescriptor:desc];
    this->m_depthTexture = depthTex;
    this->m_depthWidth = width;
    this->m_depthHeight = height;
}

void* CGxDeviceMTL::GetDepthState(bool depthTest, bool depthWrite, uint32_t depthFunc) {
    uint32_t funcIndex = std::min<uint32_t>(depthFunc, 3u);
    auto& cached = this->m_depthStates[depthTest ? 1 : 0][depthWrite ? 1 : 0][funcIndex];
    if (cached) {
        return cached;
    }

    auto device = (id<MTLDevice>)this->m_device;
    if (!device) {
        return nullptr;
    }

    auto desc = [MTLDepthStencilDescriptor new];
    if (!depthTest) {
        desc.depthCompareFunction = MTLCompareFunctionAlways;
    } else {
        switch (funcIndex) {
            case 0:
                desc.depthCompareFunction = MTLCompareFunctionLessEqual;
                break;
            case 1:
                desc.depthCompareFunction = MTLCompareFunctionEqual;
                break;
            case 2:
                desc.depthCompareFunction = MTLCompareFunctionGreaterEqual;
                break;
            case 3:
                desc.depthCompareFunction = MTLCompareFunctionLess;
                break;
            default:
                desc.depthCompareFunction = MTLCompareFunctionAlways;
                break;
        }
    }

    desc.depthWriteEnabled = depthWrite ? YES : NO;

    id<MTLDepthStencilState> state = [device newDepthStencilStateWithDescriptor:desc];
    [desc release];

    cached = state;
    return state;
}

void CGxDeviceMTL::ITexCreate(CGxTex* texId) {
    if (!texId || !this->m_device) {
        return;
    }

    EGxTexFormat format = texId->m_dataFormat != GxTex_Unknown ? texId->m_dataFormat : texId->m_format;
    auto pixelFormat = MtlPixelFormatForGx(format);
    if (pixelFormat == MTLPixelFormatInvalid) {
        return;
    }

    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t baseMip = 0;
    uint32_t mipCount = 0;
    this->ITexWHDStartEnd(texId, width, height, baseMip, mipCount);

    auto device = (id<MTLDevice>)this->m_device;
    MTLTextureDescriptor* desc = nil;
    
    if (texId->m_target == GxTex_CubeMap) {
        // Create cubemap texture (6 faces)
        desc = [MTLTextureDescriptor textureCubeDescriptorWithPixelFormat:pixelFormat size:width mipmapped:(mipCount - baseMip) > 1];
    } else {
        // Create 2D texture
        desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:pixelFormat width:width height:height mipmapped:(mipCount - baseMip) > 1];
    }
    
    desc.usage = MTLTextureUsageShaderRead;
    desc.storageMode = MTLStorageModeShared;

    id<MTLTexture> texture = [device newTextureWithDescriptor:desc];
    texId->m_apiSpecificData = texture;
    texId->m_needsCreation = 0;
}

void CGxDeviceMTL::ITexUpload(CGxTex* texId) {
    auto texture = (id<MTLTexture>)texId->m_apiSpecificData;
    if (!texture) {
        return;
    }

    uint32_t texelStrideInBytes = 0;
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

    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t baseMip = 0;
    uint32_t mipCount = 0;
    this->ITexWHDStartEnd(texId, width, height, baseMip, mipCount);

    EGxTexFormat format = texId->m_dataFormat != GxTex_Unknown ? texId->m_dataFormat : texId->m_format;
    const bool compressed = GxTexIsCompressed(format);
    
    // Cubemaps have 6 faces, regular textures have 1
    int32_t numFaces = texId->m_target == GxTex_CubeMap ? 6 : 1;

    for (int32_t face = 0; face < numFaces; ++face) {
        for (uint32_t mipLevel = baseMip; mipLevel < mipCount; ++mipLevel) {
            texels = nullptr;
            texId->m_userFunc(
                GxTex_Latch,
                std::max(texId->m_width >> mipLevel, 1u),
                std::max(texId->m_height >> mipLevel, 1u),
                face,
                mipLevel,
                texId->m_userArg,
                texelStrideInBytes,
                texels
            );

            if (!texels) {
                continue;
            }

            uint32_t mipWidth = std::max(texId->m_width >> mipLevel, 1u);
            uint32_t mipHeight = std::max(texId->m_height >> mipLevel, 1u);
            MTLRegion region = MTLRegionMake2D(0, 0, mipWidth, mipHeight);

            if (compressed) {
                uint32_t blockSize = CGxDevice::s_texFormatBytesPerBlock[format];
                uint32_t blocksWide = std::max(1u, (mipWidth + 3) / 4);
                uint32_t bytesPerRow = blocksWide * blockSize;
                if (texId->m_target == GxTex_CubeMap) {
                    [texture replaceRegion:region mipmapLevel:mipLevel - baseMip slice:face withBytes:texels bytesPerRow:bytesPerRow bytesPerImage:0];
                } else {
                    [texture replaceRegion:region mipmapLevel:mipLevel - baseMip withBytes:texels bytesPerRow:bytesPerRow];
                }
            } else {
                const void* uploadTexels = texels;
                std::vector<uint32_t> convertedData;  // RGBA8 is 32-bit per pixel
                
                // Handle Argb4444 format conversion to RGBA8
                // Game packs as: A[15:12] R[11:8] G[7:4] B[3:0] (each 4 bits)
                // We expand to RGBA8: R[7:0] G[7:0] B[7:0] A[7:0] (each 8 bits)
                if (format == GxTex_Argb4444) {
                    uint32_t pixelCount = mipWidth * mipHeight;
                    convertedData.resize(pixelCount);
                    const uint16_t* srcData = static_cast<const uint16_t*>(texels);
                    
                    for (uint32_t i = 0; i < pixelCount; ++i) {
                        uint16_t pixel = srcData[i];
                        // Extract 4-bit nibbles from game's ARGB format
                        uint8_t a4 = (pixel >> 12) & 0xF;
                        uint8_t r4 = (pixel >> 8) & 0xF;
                        uint8_t g4 = (pixel >> 4) & 0xF;
                        uint8_t b4 = pixel & 0xF;
                        
                        // Expand 4-bit to 8-bit (multiply by 17 = 0x11 to scale 0-15 to 0-255)
                        uint8_t r8 = r4 * 17;
                        uint8_t g8 = g4 * 17;
                        uint8_t b8 = b4 * 17;
                        uint8_t a8 = a4 * 17;
                        
                        // Pack as RGBA8 (little-endian: ABGR in memory)
                        convertedData[i] = (a8 << 24) | (b8 << 16) | (g8 << 8) | r8;
                    }
                    uploadTexels = convertedData.data();
                    texelStrideInBytes = mipWidth * 4;  // 4 bytes per pixel for RGBA8
                }
                
                if (texId->m_target == GxTex_CubeMap) {
                    [texture replaceRegion:region mipmapLevel:mipLevel - baseMip slice:face withBytes:uploadTexels bytesPerRow:texelStrideInBytes bytesPerImage:0];
                } else {
                    [texture replaceRegion:region mipmapLevel:mipLevel - baseMip withBytes:uploadTexels bytesPerRow:texelStrideInBytes];
                }
            }
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

void* CGxDeviceMTL::GetTexture(CGxTex* texId) {
    if (!texId) {
        return nullptr;
    }

    if (texId->m_needsCreation || texId->m_needsUpdate || !texId->m_apiSpecificData) {
        texId->m_needsUpdate = 1;
        this->ITexMarkAsUpdated(texId);
    }

    return texId->m_apiSpecificData;
}

void* CGxDeviceMTL::GetSampler(CGxTex* texId) {
    if (!texId) {
        this->EnsureFallbackTexture();
        return this->m_fallbackSampler;
    }

    if (!texId->m_apiSpecificData2 || texId->m_needsFlagUpdate) {
        auto device = (id<MTLDevice>)this->m_device;
        if (!device) {
            return nullptr;
        }

        if (texId->m_apiSpecificData2) {
            auto sampler = (id<MTLSamplerState>)texId->m_apiSpecificData2;
            [sampler release];
            texId->m_apiSpecificData2 = nullptr;
        }

        auto desc = [MTLSamplerDescriptor new];
        desc.sAddressMode = texId->m_flags.m_wrapU ? MTLSamplerAddressModeRepeat : MTLSamplerAddressModeClampToEdge;
        desc.tAddressMode = texId->m_flags.m_wrapV ? MTLSamplerAddressModeRepeat : MTLSamplerAddressModeClampToEdge;

        switch (texId->m_flags.m_filter) {
            case GxTex_Nearest:
                desc.minFilter = MTLSamplerMinMagFilterNearest;
                desc.magFilter = MTLSamplerMinMagFilterNearest;
                desc.mipFilter = MTLSamplerMipFilterNotMipmapped;
                break;
            case GxTex_NearestMipNearest:
                desc.minFilter = MTLSamplerMinMagFilterNearest;
                desc.magFilter = MTLSamplerMinMagFilterNearest;
                desc.mipFilter = MTLSamplerMipFilterNearest;
                break;
            case GxTex_LinearMipNearest:
                desc.minFilter = MTLSamplerMinMagFilterLinear;
                desc.magFilter = MTLSamplerMinMagFilterLinear;
                desc.mipFilter = MTLSamplerMipFilterNearest;
                break;
            case GxTex_LinearMipLinear:
                desc.minFilter = MTLSamplerMinMagFilterLinear;
                desc.magFilter = MTLSamplerMinMagFilterLinear;
                desc.mipFilter = MTLSamplerMipFilterLinear;
                break;
            case GxTex_Anisotropic:
                desc.minFilter = MTLSamplerMinMagFilterLinear;
                desc.magFilter = MTLSamplerMinMagFilterLinear;
                desc.mipFilter = MTLSamplerMipFilterLinear;
                desc.maxAnisotropy = std::max<uint32_t>(texId->m_flags.m_maxAnisotropy, 1);
                break;
            default:
                desc.minFilter = MTLSamplerMinMagFilterLinear;
                desc.magFilter = MTLSamplerMinMagFilterLinear;
                desc.mipFilter = MTLSamplerMipFilterNotMipmapped;
                break;
        }

        id<MTLSamplerState> sampler = [device newSamplerStateWithDescriptor:desc];
        [desc release];

        texId->m_apiSpecificData2 = sampler;
        texId->m_needsFlagUpdate = 0;
    }

    return texId->m_apiSpecificData2;
}

void CGxDeviceMTL::EnsureFallbackTexture() {
    if (this->m_fallbackTexture || !this->m_device) {
        return;
    }

    auto device = (id<MTLDevice>)this->m_device;
    auto desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm width:1 height:1 mipmapped:NO];
    desc.usage = MTLTextureUsageShaderRead;
    desc.storageMode = MTLStorageModeShared;

    id<MTLTexture> texture = [device newTextureWithDescriptor:desc];
    uint32_t pixel = 0xFFFFFFFF;
    MTLRegion region = MTLRegionMake2D(0, 0, 1, 1);
    [texture replaceRegion:region mipmapLevel:0 withBytes:&pixel bytesPerRow:4];

    auto samplerDesc = [MTLSamplerDescriptor new];
    samplerDesc.sAddressMode = MTLSamplerAddressModeClampToEdge;
    samplerDesc.tAddressMode = MTLSamplerAddressModeClampToEdge;
    samplerDesc.minFilter = MTLSamplerMinMagFilterLinear;
    samplerDesc.magFilter = MTLSamplerMinMagFilterLinear;
    samplerDesc.mipFilter = MTLSamplerMipFilterNotMipmapped;
    id<MTLSamplerState> sampler = [device newSamplerStateWithDescriptor:samplerDesc];
    [samplerDesc release];

    this->m_fallbackTexture = texture;
    this->m_fallbackSampler = sampler;
}

void CGxDeviceMTL::Resize(uint32_t width, uint32_t height) {
    CRect rect = { 0.0f, 0.0f, static_cast<float>(height), static_cast<float>(width) };
    this->DeviceSetDefWindow(rect);
}
