#include "gx/CGxDevice.hpp"
#include "gx/Gx.hpp"
#include "gx/Shader.hpp"
#include "gx/texture/CGxTex.hpp"
#include "util/SFile.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
#include <new>
#include <storm/Error.hpp>
#include <storm/Memory.hpp>

#if defined(WHOA_SYSTEM_WIN)
    #include "gx/d3d/CGxDeviceD3d.hpp"
#endif

#if defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_WIN)
    #include "gx/glsdl/CGxDeviceGLSDL.hpp"
#endif

#if defined(WHOA_SYSTEM_MAC)
    #include "gx/gll/CGxDeviceGLL.hpp"
#endif

uint32_t CGxDevice::s_alphaRef[] = {
    0,      // GxBlend_Opaque
    224,    // GxBlend_AlphaKey
    1,      // GxBlend_Alpha
    1,      // GxBlend_Add
    1,      // GxBlend_Mod
    1,      // GxBlend_Mod2x
    1,      // GxBlend_ModAdd
    0,      // GxBlend_InvSrcAlphaAdd
    0,      // GxBlend_InvSrcAlphaOpaque
    0,      // GxBlend_SrcAlphaOpaque
    0,      // GxBlend_NoAlphaAdd
    0       // GxBlend_ConstantAlpha
};

C3Vector CGxDevice::s_pointScaleIdentity = { 1.0f, 0.0f, 0.0f };

uint32_t CGxDevice::s_primVtxAdjust[] = {
    0,      // GxPrim_Points
    0,      // GxPrim_Lines
    1,      // GxPrim_LineStrip
    0,      // GxPrim_Triangles
    2,      // GxPrim_TriangleStrip
    2,      // GxPrim_TriangleFan
};

uint32_t CGxDevice::s_primVtxDiv[] = {
    1,      // GxPrim_Points
    2,      // GxPrim_Lines
    1,      // GxPrim_LineStrip
    3,      // GxPrim_Triangles
    1,      // GxPrim_TriangleStrip
    1,      // GxPrim_TriangleFan
};

ShaderConstants CGxDevice::s_shadowConstants[2];

uint32_t CGxDevice::s_streamPoolSize[] = {
    0x2C0000,   // GxPoolTarget_Vertex
    0x40000     // GxPoolTarget_Index
};

uint32_t CGxDevice::s_texFormatBitDepth[] = {
    0,      // GxTex_Unknown
    32,     // GxTex_Abgr8888
    32,     // GxTex_Argb8888
    16,     // GxTex_Argb4444
    16,     // GxTex_Argb1555
    16,     // GxTex_Rgb565
    4,      // GxTex_Dxt1
    8,      // GxTex_Dxt3
    8,      // GxTex_Dxt5
    16,     // GxTex_Uv88
    32,     // GxTex_Gr1616F
    32,     // GxTex_R32F
    32      // GxTex_D24X8
};

uint32_t CGxDevice::s_texFormatBytesPerBlock[] = {
    0,      // GxTex_Unknown
    4,      // GxTex_Abgr8888
    4,      // GxTex_Argb8888
    2,      // GxTex_Argb4444
    2,      // GxTex_Argb1555
    2,      // GxTex_Rgb565
    8,      // GxTex_Dxt1
    16,     // GxTex_Dxt3
    16,     // GxTex_Dxt5
    2,      // GxTex_Uv88
    4,      // GxTex_Gr1616F
    4,      // GxTex_R32F
    4       // GxTex_D24X8
};

void CGxDevice::Log(const char* format, ...) {
    // TODO
}

void CGxDevice::Log(const CGxFormat& format) {
    // TODO
}

#if defined(WHOA_SYSTEM_WIN)
CGxDevice* CGxDevice::NewD3d() {
    auto m = SMemAlloc(sizeof(CGxDeviceD3d), __FILE__, __LINE__, 0x0);
    return new (m) CGxDeviceD3d();
}

CGxDevice* CGxDevice::NewD3d9Ex() {
    // TODO
    return nullptr;
}
#endif

#if defined(WHOA_SYSTEM_MAC)
CGxDevice* CGxDevice::NewGLL() {
    auto m = SMemAlloc(sizeof(CGxDeviceGLL), __FILE__, __LINE__, 0x0);
    return new (m) CGxDeviceGLL();
}
#endif


#if defined(WHOA_SYSTEM_WIN) || defined(WHOA_SYSTEM_LINUX)
CGxDevice* CGxDevice::NewOpenGl() {
    auto m = SMemAlloc(sizeof(CGxDeviceGLSDL), __FILE__, __LINE__, 0x0);
    return new (m) CGxDeviceGLSDL();
}
#endif

uint32_t CGxDevice::PrimCalcCount(EGxPrim primType, uint32_t count) {
    auto div = CGxDevice::s_primVtxDiv[primType];
    if (div != 1) {
        count /= div;
    }

    return count - CGxDevice::s_primVtxAdjust[primType];
}

CGxDevice::CGxDevice() {
    // TODO
    // - implement rest of constructor

    this->IRsInit();

    // Set default viewport
    this->m_viewport.x = { 0.0f, 1.0f };
    this->m_viewport.y = { 0.0f, 1.0f };
    this->m_viewport.z = { 0.0f, 1.0f };

    // Turn on all master enables
    this->m_appMasterEnables = 511;
    this->m_hwMasterEnables = 511;

    this->ShaderConstantsClear();
}

CGxBuf* CGxDevice::BufCreate(CGxPool* pool, uint32_t itemSize, uint32_t itemCount, uint32_t index) {
    auto m = SMemAlloc(sizeof(CGxBuf), __FILE__, __LINE__, 0x0);
    auto buf = new (m) CGxBuf(pool, itemSize, itemCount, index);

    pool->m_bufList.LinkToTail(buf);

    return buf;
}

char* CGxDevice::BufLock(CGxBuf* buf) {
    buf->unk1E = 1;
    buf->unk1F = 0;

    this->m_bufLocked[buf->m_pool->m_target] = buf;

    return nullptr;
}

CGxBuf* CGxDevice::BufStream(EGxPoolTarget target, uint32_t itemSize, uint32_t itemCount) {
    CGxBuf* buf = this->m_streamBufs[target];
    CGxPool* pool = buf->m_pool;

    if (pool && pool->m_size < itemSize * itemCount) {
        this->PoolSizeSet(pool, itemSize * itemCount);
    }

    buf->m_itemSize = itemSize;
    buf->m_itemCount = itemCount;
    buf->m_size = itemSize * itemCount;
    buf->unk1C = 0;

    return buf;
}

int32_t CGxDevice::BufUnlock(CGxBuf* buf, uint32_t size) {
    this->m_bufLocked[buf->m_pool->m_target] = nullptr;

    return 1;
}

void CGxDevice::BufData(CGxBuf* buf, const void* data, size_t size, uintptr_t offset) {
    buf->unk1E = 1;
    buf->unk1F = 0;
}

const CGxCaps& CGxDevice::Caps() const {
    return this->m_caps;
}

int32_t CGxDevice::DeviceCreate(int32_t (*windowProc)(void* window, uint32_t message, uintptr_t wparam, intptr_t lparam), const CGxFormat& format) {
    this->m_windowProc = windowProc;

    return this->DeviceSetFormat(format);
}

void CGxDevice::DeviceCreatePools() {
    this->m_vertexPool = this->PoolCreate(
        GxPoolTarget_Vertex,
        GxPoolUsage_Stream,
        CGxDevice::s_streamPoolSize[GxPoolTarget_Vertex],
        GxPoolHintBit_Unk3,
        "Stream_vtx"
    );

    this->m_indexPool = this->PoolCreate(
        GxPoolTarget_Index,
        GxPoolUsage_Stream,
        CGxDevice::s_streamPoolSize[GxPoolTarget_Index],
        GxPoolHintBit_Unk3,
        "Stream_idx"
    );
}

void CGxDevice::DeviceCreateStreamBufs() {
    this->m_streamBufs[GxPoolTarget_Vertex] = this->BufCreate(this->m_vertexPool, 0, 0, 0);
    this->m_streamBufs[GxPoolTarget_Index] = this->BufCreate(this->m_indexPool, 0, 0, 0);
}

const CRect& CGxDevice::DeviceCurWindow() {
    return this->m_curWindowRect;
}

int32_t CGxDevice::DeviceSetFormat(const CGxFormat& format) {
    memcpy(&this->m_format, &format, sizeof(this->m_format));

    return 1;
}

void CGxDevice::DeviceSetCurWindow(const CRect& rect) {
    this->m_curWindowRect = rect;
}

void CGxDevice::DeviceSetDefWindow(CRect const& rect) {
    this->m_defWindowRect = rect;
    this->DeviceSetCurWindow(rect);
}

const CRect& CGxDevice::DeviceDefWindow() {
    return this->m_defWindowRect;
}

void CGxDevice::ICursorCreate(const CGxFormat& format) {
    // TODO
}

int32_t CGxDevice::IDevIsWindowed() {
    return this->m_format.window;
}

void CGxDevice::IRsDirty(EGxRenderState which) {
    auto rs = &this->m_appRenderStates[which];

    if (!rs->m_dirty) {
        auto ds = this->m_dirtyStates.New();
        *ds = which;

        rs->m_dirty = 1;
    }

    if (rs->m_stackDepth != this->m_stackOffsets.Count()) {
        auto ps = this->m_pushedStates.New();
        ps->m_which = which;
        ps->m_value = rs->m_value;
        ps->m_stackDepth = rs->m_stackDepth;

        rs->m_stackDepth = this->m_stackOffsets.Count();
    }
}

void CGxDevice::IRsForceUpdate() {
    for (int32_t which = 0; which < GxRenderStates_Last; which++) {
        auto& rs = this->m_appRenderStates[which];
        auto& hs = this->m_hwRenderStates[which];

        auto ds = this->m_dirtyStates.New();
        *ds = static_cast<EGxRenderState>(which);

        rs.m_dirty = 1;

        // TODO manage this through operators
        hs.m_data.i[0] = ~rs.m_value.m_data.i[0];
        hs.m_data.i[1] = ~rs.m_value.m_data.i[1];
        hs.m_data.i[2] = ~rs.m_value.m_data.i[2];
        hs.filler = ~rs.m_value.filler;
    }
}

void CGxDevice::IRsForceUpdate(EGxRenderState which) {
    if (!this->m_context) {
        return;
    }

    auto& rs = this->m_appRenderStates[which];
    auto& hs = this->m_hwRenderStates[which];

    auto ds = this->m_dirtyStates.New();
    *ds = which;

    rs.m_dirty = 1;

    // TODO manage this through operators
    hs.m_data.i[0] = ~rs.m_value.m_data.i[0];
    hs.m_data.i[1] = ~rs.m_value.m_data.i[1];
    hs.m_data.i[2] = ~rs.m_value.m_data.i[2];
    hs.filler = ~rs.m_value.filler;
}

void CGxDevice::IRsInit() {
    this->m_appRenderStates.SetCount(GxRenderStates_Last);
    this->m_hwRenderStates.SetCount(GxRenderStates_Last);

    memset(this->m_appRenderStates.m_data, 0, sizeof(CGxAppRenderState) * GxRenderStates_Last);
    memset(this->m_hwRenderStates.m_data, 0, sizeof(CGxStateBom) * GxRenderStates_Last);

    this->m_appRenderStates[GxRs_PolygonOffset].m_value     = 0;
    this->m_appRenderStates[GxRs_MatDiffuse].m_value        = 0xFFFFFFFF;
    this->m_appRenderStates[GxRs_MatEmissive].m_value       = 0;
    this->m_appRenderStates[GxRs_MatSpecular].m_value       = 0;
    this->m_appRenderStates[GxRs_MatSpecularExp].m_value    = 0.0f;
    this->m_appRenderStates[GxRs_NormalizeNormals].m_value  = 0;
    this->m_appRenderStates[GxRs_BlendingMode].m_value      = GxBlend_Opaque;
    this->m_appRenderStates[GxRs_FogStart].m_value          = 1.0f;
    this->m_appRenderStates[GxRs_FogEnd].m_value            = 0.0f;
    this->m_appRenderStates[GxRs_FogColor].m_value          = 0xFF808080;
    this->m_appRenderStates[GxRs_Lighting].m_value          = 1;
    this->m_appRenderStates[GxRs_Fog].m_value               = 1;
    this->m_appRenderStates[GxRs_DepthTest].m_value         = 1;
    this->m_appRenderStates[GxRs_DepthFunc].m_value         = 0;
    this->m_appRenderStates[GxRs_DepthWrite].m_value        = 1;
    this->m_appRenderStates[GxRs_ColorWrite].m_value        = 15;
    this->m_appRenderStates[GxRs_Culling].m_value           = 1;
    this->m_appRenderStates[GxRs_ClipPlaneMask].m_value     = 0;
    this->m_appRenderStates[GxRs_Multisample].m_value       = 1;
    this->m_appRenderStates[GxRs_ScissorTest].m_value       = 0;

    this->m_appRenderStates[GxRs_Texture0].m_value          = nullptr;
    this->m_appRenderStates[GxRs_ColorOp0].m_value          = 0;
    this->m_appRenderStates[GxRs_AlphaOp0].m_value          = 0;
    this->m_appRenderStates[GxRs_TexGen0].m_value           = 0;
    this->m_appRenderStates[GxRs_Unk61].m_value             = 0;
    this->m_appRenderStates[GxRs_Unk69].m_value             = 0;

    this->m_appRenderStates[GxRs_Texture1].m_value          = nullptr;
    this->m_appRenderStates[GxRs_ColorOp1].m_value          = 0;
    this->m_appRenderStates[GxRs_AlphaOp1].m_value          = 0;
    this->m_appRenderStates[GxRs_TexGen1].m_value           = 0;
    this->m_appRenderStates[GxRs_Unk62].m_value             = 0;
    this->m_appRenderStates[GxRs_Unk70].m_value             = 1;

    this->m_appRenderStates[GxRs_Texture2].m_value          = nullptr;
    this->m_appRenderStates[GxRs_ColorOp2].m_value          = 0;
    this->m_appRenderStates[GxRs_AlphaOp2].m_value          = 0;
    this->m_appRenderStates[GxRs_TexGen2].m_value           = 0;
    this->m_appRenderStates[GxRs_Unk63].m_value             = 0;
    this->m_appRenderStates[GxRs_Unk71].m_value             = 2;

    this->m_appRenderStates[GxRs_Texture3].m_value          = nullptr;
    this->m_appRenderStates[GxRs_ColorOp3].m_value          = 0;
    this->m_appRenderStates[GxRs_AlphaOp3].m_value          = 0;
    this->m_appRenderStates[GxRs_TexGen3].m_value           = 0;
    this->m_appRenderStates[GxRs_Unk64].m_value             = 0;
    this->m_appRenderStates[GxRs_Unk72].m_value             = 3;

    this->m_appRenderStates[GxRs_Texture4].m_value          = nullptr;
    this->m_appRenderStates[GxRs_ColorOp4].m_value          = 0;
    this->m_appRenderStates[GxRs_AlphaOp4].m_value          = 0;
    this->m_appRenderStates[GxRs_TexGen4].m_value           = 0;
    this->m_appRenderStates[GxRs_Unk65].m_value             = 0;
    this->m_appRenderStates[GxRs_Unk73].m_value             = 4;

    this->m_appRenderStates[GxRs_Texture5].m_value          = nullptr;
    this->m_appRenderStates[GxRs_ColorOp5].m_value          = 0;
    this->m_appRenderStates[GxRs_AlphaOp5].m_value          = 0;
    this->m_appRenderStates[GxRs_TexGen5].m_value           = 0;
    this->m_appRenderStates[GxRs_Unk66].m_value             = 0;
    this->m_appRenderStates[GxRs_Unk74].m_value             = 5;

    this->m_appRenderStates[GxRs_Texture6].m_value          = nullptr;
    this->m_appRenderStates[GxRs_ColorOp6].m_value          = 0;
    this->m_appRenderStates[GxRs_AlphaOp6].m_value          = 0;
    this->m_appRenderStates[GxRs_TexGen6].m_value           = 0;
    this->m_appRenderStates[GxRs_Unk67].m_value             = 0;
    this->m_appRenderStates[GxRs_Unk75].m_value             = 6;

    this->m_appRenderStates[GxRs_Texture7].m_value          = nullptr;
    this->m_appRenderStates[GxRs_ColorOp7].m_value          = 0;
    this->m_appRenderStates[GxRs_AlphaOp7].m_value          = 0;
    this->m_appRenderStates[GxRs_TexGen7].m_value           = 0;
    this->m_appRenderStates[GxRs_Unk68].m_value             = 0;
    this->m_appRenderStates[GxRs_Unk76].m_value             = 7;

    this->m_appRenderStates[GxRs_Texture8].m_value          = nullptr;
    this->m_appRenderStates[GxRs_Texture9].m_value          = nullptr;
    this->m_appRenderStates[GxRs_Texture10].m_value         = nullptr;
    this->m_appRenderStates[GxRs_Texture11].m_value         = nullptr;
    this->m_appRenderStates[GxRs_Texture12].m_value         = nullptr;
    this->m_appRenderStates[GxRs_Texture13].m_value         = nullptr;
    this->m_appRenderStates[GxRs_Texture14].m_value         = nullptr;
    this->m_appRenderStates[GxRs_Texture15].m_value         = nullptr;

    this->m_appRenderStates[GxRs_PixelShader].m_value       = nullptr;
    this->m_appRenderStates[GxRs_VertexShader].m_value      = nullptr;

    this->m_appRenderStates[GxRs_PointScale].m_value        = 1.0f;
    this->m_appRenderStates[GxRs_PointScaleAttenuation].m_value = CGxDevice::s_pointScaleIdentity;
    this->m_appRenderStates[GxRs_PointScaleMin].m_value     = 0.0f;
    this->m_appRenderStates[GxRs_PointScaleMax].m_value     = 1.0f;
    this->m_appRenderStates[GxRs_PointSprite].m_value       = 0;

    this->m_appRenderStates[GxRs_Unk84].m_value             = 0.0f;
    this->m_appRenderStates[GxRs_ColorMaterial].m_value     = 0;
}

void CGxDevice::IRsSync(int32_t force) {
    if (force) {
        this->IRsForceUpdate();
    }

    for (int32_t i = this->m_dirtyStates.Count() - 1; i >= 0; i--) {
        auto ds = this->m_dirtyStates[i];
        auto rs = &this->m_appRenderStates[ds];
        auto hs = &this->m_hwRenderStates[ds];

        if (rs->m_dirty && rs->m_value != *hs) {
            this->IRsSendToHw(ds);
        }

        *hs = rs->m_value;
        rs->m_dirty = 0;
    }

    this->m_dirtyStates.SetCount(0);
}

void CGxDevice::IShaderLoad(CGxShader* shaders[], EGxShTarget target, const char* a4, const char* a5, int32_t permutations) {
    int32_t profile = this->m_caps.m_shaderTargets[target];

    if (!profile) {
        return;
    }

    char path[260];
    SFile* file;

    while (true) {
        sprintf(path, "%s\\%s\\%s.bls", a4, g_gxShaderProfileNames[target][profile], a5);
        SFile::Open(path, &file);

        // Opened shader file
        if (file) {
            break;
        }

        // Fallback
        if (target == GxSh_Vertex) {
            // vs_3_0 -> vs_2_0
            // vs_2_0 -> vs_1_1

            if (profile == 3) {
                profile = 2;
            } else if (profile == 2) {
                profile = 1;
            } else {
                break;
            }
        } else if (target == GxSh_Pixel) {
            // nvts3 -> nvts
            // nvts2 -> nvts
            // ps_3_0 -> ps_2_0
            // ps_2_0 -> ps_1_4
            // ps_1_4 -> ps_1_1

            if (profile == 9 || profile == 10) {
                profile = 8;
            } else if (profile == 4) {
                profile = 3;
            } else if (profile == 3) {
                profile = 2;
            } else if (profile == 2) {
                profile = 1;
            } else {
                break;
            }
        }
    }

    if (!file) {
        return;
    }

    uint32_t signature;
    uint32_t version;
    uint32_t permutationCount;

    if (file) {
        SFile::Read(file, &signature, 4, nullptr, nullptr, nullptr);

        if (signature == 'GXSH') {
            SFile::Read(file, &version, 4, nullptr, nullptr, nullptr);

            if (version == 0x10003) {
                SFile::Read(file, &permutationCount, 4, nullptr, nullptr, nullptr);

                // TODO
                // assert(permutationCount == permutations);

                if (permutations > 0) {
                    for (int32_t p = 0; p < permutations; p++) {
                        shaders[p]->Load(file);

                        shaders[p]->loaded = 0;
                        shaders[p]->int34 = 0;
                    }
                }
            }
        }

        SFile::Close(file);
    }
}

void CGxDevice::ITexMarkAsUpdated(CGxTex* texId) {
    if (!texId->m_needsUpdate) {
        return;
    }

    texId->m_updateRect = {
        static_cast<int32_t>(texId->m_height),
        static_cast<int32_t>(texId->m_width),
        0,
        0
    };

    texId->m_needsUpdate = 0;
}

void CGxDevice::ITexWHDStartEnd(CGxTex* texId, uint32_t& width, uint32_t& height, uint32_t& baseMip, uint32_t& mipCount) {
    width = texId->m_width;
    height = texId->m_height;

    if ((texId->m_flags.m_filter == GxTex_Nearest || texId->m_flags.m_filter == GxTex_Linear || texId->m_flags.m_generateMipMaps) && !texId->m_flags.m_forceMipTracking) {
        baseMip = 0;
        mipCount = 1;

        return;
    }

    mipCount = 1;

    if (texId->m_format == GxTex_Dxt1 || texId->m_format == GxTex_Dxt3 || texId->m_format == GxTex_Dxt5) {
        uint32_t shortEdge = std::min(texId->m_width, texId->m_height);

        while (shortEdge > 4) {
            shortEdge /= 2;
            mipCount++;
        }
    } else {
        uint32_t longEdge = std::max(texId->m_width, texId->m_height);

        while (longEdge > 1) {
            longEdge /= 2;
            mipCount++;
        }
    }

    baseMip = std::min(this->m_baseMipLevel, mipCount - 1);

    width >>= baseMip;
    height >>= baseMip;

    if (texId->m_flags.m_forceMipTracking) {
        mipCount = baseMip + 1;
    }
}

int32_t CGxDevice::MasterEnable(EGxMasterEnables state) {
    return ((1 << state) & this->m_appMasterEnables) != 0;
}

void CGxDevice::MasterEnableSet(EGxMasterEnables state, int32_t enable) {
    this->m_appMasterEnables = ((enable & 1) << state) | (this->m_appMasterEnables & ~(1 << state));

    switch (state) {
        case GxMasterEnable_Lighting:
            this->IRsForceUpdate(GxRs_Lighting);
            break;

        case GxMasterEnable_Fog:
            this->IRsForceUpdate(GxRs_Fog);
            break;

        case GxMasterEnable_DepthTest:
            this->IRsForceUpdate(GxRs_DepthTest);
            break;

        case GxMasterEnable_DepthWrite:
            this->IRsForceUpdate(GxRs_DepthWrite);
            break;

        case GxMasterEnable_ColorWrite:
            this->IRsForceUpdate(GxRs_ColorWrite);
            break;

        case GxMasterEnable_Culling:
            this->IRsForceUpdate(GxRs_Culling);
            break;

        default:
            break;
    }
}

void CGxDevice::PrimIndexPtr(CGxBuf* buf) {
    if (buf->unk1E || this->m_primIndexBuf != buf) {
        buf->unk1E = 0;
        this->m_primIndexDirty = 1;
        this->m_primIndexBuf = buf;
    }
}

void CGxDevice::PrimVertexFormat(CGxBuf* buf, CGxVertexAttrib* attribs, uint32_t count) {
    for (int32_t i = 0; i < count; i++) {
        int32_t attrib = attribs->attrib;

        int32_t dirty = buf->unk1E
            || this->m_primVertexFormatBuf[attrib] != buf
            || this->m_primVertexFormatAttrib[attrib].type != attribs->type
            || this->m_primVertexFormatAttrib[attrib].offset != attribs->offset
            || this->m_primVertexFormatAttrib[attrib].bufSize != attribs->bufSize;

        if (dirty) {
            this->m_primVertexDirty |= 1 << attrib;
        }

        this->m_primVertexFormatBuf[attrib] = buf;

        this->m_primVertexFormatAttrib[attrib].attrib = attribs->attrib;
        this->m_primVertexFormatAttrib[attrib].type = attribs->type;
        this->m_primVertexFormatAttrib[attrib].offset = attribs->offset;
        this->m_primVertexFormatAttrib[attrib].bufSize = attribs->bufSize;

        attribs++;
    }

    buf->unk1E = 0;
    this->m_primVertexFormat = GxVertexBufferFormats_Last;
}

void CGxDevice::PrimVertexMask(uint32_t mask) {
    this->m_primVertexDirty |= mask ^ this->m_primVertexMask;
    this->m_primVertexMask = mask;
    this->m_primVertexFormat = GxVertexBufferFormats_Last;
}

void CGxDevice::PrimVertexPtr(CGxBuf* buf, EGxVertexBufferFormat format) {
    this->m_primVertexFormat = format;
    this->m_primVertexBuf = buf;
    this->m_primVertexSize = Buffer::s_vertexBufDesc[format].size;
}

CGxPool* CGxDevice::PoolCreate(EGxPoolTarget target, EGxPoolUsage usage, uint32_t size, EGxPoolHintBits hint, const char* name) {
    auto m = SMemAlloc(sizeof(CGxPool), __FILE__, __LINE__, 0x0);
    auto pool = new (m) CGxPool(target, usage, size, hint, name);

    this->m_poolList.LinkToTail(pool);

    return pool;
}

void CGxDevice::RsGet(EGxRenderState which, int32_t& value) {
    value = static_cast<int32_t>(this->m_appRenderStates[which].m_value);
}

void CGxDevice::RsSet(EGxRenderState which, int32_t value) {
    if (!this->m_context) {
        return;
    }

    if (this->m_appRenderStates[which].m_value != value) {
        this->IRsDirty(which);
        this->m_appRenderStates[which].m_value = value;
    }
}

void CGxDevice::RsSet(EGxRenderState which, void* value) {
    if (!this->m_context) {
        return;
    }

    if (this->m_appRenderStates[which].m_value != value) {
        this->IRsDirty(which);
        this->m_appRenderStates[which].m_value = value;

        if (value) {
            if (which >= GxRs_Texture0 && which <= GxRs_Texture15) {
                CGxTex* texture = static_cast<CGxTex*>(value);

                if (texture->m_flags.m_renderTarget && texture->m_needsUpdate) {
                    this->ITexMarkAsUpdated(texture);
                }
            }
        }
    }
}

void CGxDevice::RsSetAlphaRef() {
    if (!this->m_context) {
        return;
    }

    int32_t blendingMode;
    this->RsGet(GxRs_BlendingMode, blendingMode);

    this->RsSet(GxRs_AlphaRef, CGxDevice::s_alphaRef[blendingMode]);
}

void CGxDevice::RsPop() {
    auto topOfStack = this->m_stackOffsets[this->m_stackOffsets.Count() - 1];

    if (this->m_pushedStates.Count() > topOfStack) {
        auto bottomOfStack = this->m_pushedStates.Count() - 1;
        auto stackSize = this->m_pushedStates.Count() - topOfStack;

        for (uint32_t stackOffset = 0; stackOffset < stackSize; stackOffset++) {
            auto ps = &this->m_pushedStates[bottomOfStack - stackOffset];
            auto rs = &this->m_appRenderStates[ps->m_which];

            if (!rs->m_dirty) {
                auto ds = this->m_dirtyStates.New();
                *ds = ps->m_which;

                rs->m_dirty = 1;
            }

            rs->m_value = ps->m_value;
            rs->m_stackDepth = ps->m_stackDepth;
        }
    }

    this->m_pushedStates.SetCount(topOfStack);
    this->m_stackOffsets.SetCount(this->m_stackOffsets.Count() - 1);
}

void CGxDevice::RsPush() {
    auto offset = this->m_stackOffsets.New();
    *offset = this->m_pushedStates.Count();
}

void CGxDevice::ScenePresent() {
    // TODO
}

void CGxDevice::ShaderConstantsClear() {
    for (int32_t i = 0; i < 256; i++) {
        CGxDevice::s_shadowConstants[0].constants[i] = {
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max()
        };

        CGxDevice::s_shadowConstants[1].constants[i] = {
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max()
        };
    }

    CGxDevice::s_shadowConstants[0].unk1 = 0;
    CGxDevice::s_shadowConstants[0].unk2 = 255;

    CGxDevice::s_shadowConstants[1].unk1 = 0;
    CGxDevice::s_shadowConstants[1].unk2 = 255;
}

char* CGxDevice::ShaderConstantsLock(EGxShTarget target) {
    return target == GxSh_Vertex
        ? reinterpret_cast<char*>(&CGxDevice::s_shadowConstants[1].constants)
        : reinterpret_cast<char*>(&CGxDevice::s_shadowConstants[0].constants);
}

void CGxDevice::ShaderConstantsSet(EGxShTarget target, uint32_t index, const float* constants, uint32_t count) {
    STORM_ASSERT((index + count - 1) <= 255);

    if (!count) {
        return;
    }

    ShaderConstants* dst;

    if (target == GxSh_Vertex) {
        dst = &CGxDevice::s_shadowConstants[1];
    } else if (target == GxSh_Pixel) {
        dst = &CGxDevice::s_shadowConstants[0];
    } else {
        STORM_ASSERT(false);
    }

    const float* c = constants;

    for (int32_t i = index; i < index + count; i++, c += 4) {
        int32_t dirty = 0;

        if (dst->constants[i].x != c[0]) {
            dirty = 1;
            dst->constants[i].x = c[0];
        }

        if (dst->constants[i].y != c[1]) {
            dirty = 1;
            dst->constants[i].y = c[1];
        }

        if (dst->constants[i].z != c[2]) {
            dirty = 1;
            dst->constants[i].z = c[2];
        }

        if (dst->constants[i].w != c[3]) {
            dirty = 1;
            dst->constants[i].w = c[3];
        }

        if (dirty) {
            if (dst->unk2 > i) {
                dst->unk2 = i;
            }

            if (dst->unk1 < i) {
                dst->unk1 = i;
            }
        }
    }
}

void CGxDevice::ShaderConstantsUnlock(EGxShTarget target, uint32_t index, uint32_t count) {
    if (target == GxSh_Pixel) {
        ShaderConstants& dst = CGxDevice::s_shadowConstants[0];
        dst.unk2 = std::min(dst.unk2, index);
        dst.unk1 = std::max(dst.unk1, index + count - 1);
    } else {
        ShaderConstants& dst = CGxDevice::s_shadowConstants[1];
        dst.unk2 = std::min(dst.unk2, index);
        dst.unk1 = std::max(dst.unk1, index + count - 1);
    }
}

void CGxDevice::ShaderCreate(CGxShader* shaders[], EGxShTarget target, const char* a4, const char* a5, int32_t permutations) {
    auto shaderList = &this->m_shaderList[target];

    if (permutations == 0) {
        return;
    }

    if (permutations == 1) {
        auto shader = shaderList->Ptr(a5);

        if (shader) {
            shaders[0] = shader;
            shader->refCount++;
            return;
        }

        shader = shaderList->New(a5, 0, 0);
        shaders[0] = shader;

        shader->refCount++;
        shader->target = target;

        this->IShaderLoad(shaders, target, a4, a5, permutations);

        return;
    }

    memset(shaders, 0, permutations * sizeof(void*));

    char key[256];

    int32_t p = 0;

    while (p < permutations) {
        sprintf(key, "%s:%d", a5, p);

        auto shader = shaderList->Ptr(key);
        shaders[p] = shader;

        if (!shader) {
            break;
        }

        p++;
    }

    // Fully loaded
    if (p == permutations) {
        for (p = 0; p < permutations; p++) {
            shaders[p]->refCount++;
        }

        return;
    }

    for (p = 0; p < permutations; p++) {
        sprintf(key, "%s:%d", a5, p);

        auto shader = shaderList->New(key, 0, 0);
        shaders[p] = shader;

        shader->refCount++;
        shader->target = target;
    }

    this->IShaderLoad(shaders, target, a4, a5, permutations);
}

int32_t CGxDevice::TexCreate(EGxTexTarget target, uint32_t width, uint32_t height, uint32_t depth, EGxTexFormat format, EGxTexFormat dataFormat, CGxTexFlags flags, void* userArg, void (*userFunc)(EGxTexCommand, uint32_t, uint32_t, uint32_t, uint32_t, void*, uint32_t&, const void*&), const char* name, CGxTex*& texId) {
    auto m = SMemAlloc(sizeof(CGxTex), __FILE__, __LINE__, 0);
    auto tex = new (m) CGxTex(
        target,
        width,
        height,
        depth,
        format,
        dataFormat,
        flags,
        userArg,
        userFunc,
        name
    );

    texId = tex;

    // TODO
    // - link tex to list in device

    return 1;
}

void CGxDevice::TexDestroy(CGxTex* texId) {
    // TODO

    if (texId) {
        delete texId;
    }
}

void CGxDevice::TexMarkForUpdate(CGxTex* texId, const CiRect& updateRect, int32_t immediate) {
    texId->m_needsUpdate = 1;

    // If the bounds of the updateRect are invalid, default to { 0, 0, height, width }
    if (updateRect.minY >= updateRect.maxY || updateRect.minX >= updateRect.maxX) {
        texId->m_updateRect = {
            0,
            0,
            static_cast<int32_t>(texId->m_height),
            static_cast<int32_t>(texId->m_width)
        };
    } else {
        texId->m_updateRect = updateRect;
    }

    if (immediate) {
        this->ITexMarkAsUpdated(texId);
    }
}

void CGxDevice::TexSetWrap(CGxTex* texId, EGxTexWrapMode wrapU, EGxTexWrapMode wrapV) {
    if (texId->m_flags.m_wrapU == wrapU && texId->m_flags.m_wrapV == wrapV) {
        return;
    }

    texId->m_flags.m_wrapU = wrapU;
    texId->m_flags.m_wrapV = wrapV;
    texId->m_needsFlagUpdate = 1;

    for (int32_t rs = GxRs_Texture0; rs <= GxRs_Texture15; rs++) {
        this->IRsForceUpdate(static_cast<EGxRenderState>(rs));
    }
}

void CGxDevice::ValidateDraw(CGxBatch* batch, int32_t count) {
    // TODO
}

void CGxDevice::XformPop(EGxXform xf) {
    this->m_xforms[xf].Pop();
}

void CGxDevice::XformProjection(C44Matrix& matrix) {
    matrix = this->m_projection;
}

void CGxDevice::XformProjNative(C44Matrix& matrix) {
    matrix = this->m_projNative;

    if (this->m_api == GxApi_OpenGl) {
        matrix.c0 *= -1.0f;
        matrix.c1 *= -1.0f;
        matrix.c2 *= -1.0f;
        matrix.c3 *= -1.0f;
    }
}

void CGxDevice::XformPush(EGxXform xf) {
    this->m_xforms[xf].Push();
}

void CGxDevice::XformSet(EGxXform xf, const C44Matrix& matrix) {
    this->m_xforms[xf].Top() = matrix;
}

void CGxDevice::XformSetProjection(const C44Matrix& matrix) {
    this->m_projection = matrix;
}

void CGxDevice::XformSetView(const C44Matrix& matrix) {
    this->m_xforms[GxXform_View].Top() = matrix;

    for (int32_t i = GxRs_TexGen0; i < GxRs_TexGen7; i++) {
        if (static_cast<int32_t>(this->m_appRenderStates[i].m_value) - 1 <= 1) {
            this->IRsForceUpdate(static_cast<EGxRenderState>(i));
        }
    }
}

void CGxDevice::XformSetViewport(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
    if (
        minX == this->m_viewport.x.l
        && maxX == this->m_viewport.x.h
        && minY == this->m_viewport.y.l
        && maxY == this->m_viewport.y.h
        && minZ == this->m_viewport.z.l
        && maxZ == this->m_viewport.z.h
    ) {
        return;
    }

    this->intF6C = 1;

    this->m_viewport.x.l = minX;
    this->m_viewport.x.h = maxX;
    this->m_viewport.y.l = minY;
    this->m_viewport.y.h = maxY;
    this->m_viewport.z.l = minZ;
    this->m_viewport.z.h = maxZ;
}

void CGxDevice::XformView(C44Matrix& matrix) {
    matrix = this->m_xforms[GxXform_View].m_mtx[this->m_xforms[GxXform_View].m_level];
}

void CGxDevice::XformViewport(float& minX, float& maxX, float& minY, float& maxY, float& minZ, float& maxZ) {
    minX = this->m_viewport.x.l;
    maxX = this->m_viewport.x.h;
    minY = this->m_viewport.y.l;
    maxY = this->m_viewport.y.h;
    minZ = this->m_viewport.z.l;
    maxZ = this->m_viewport.z.h;
}
