#include "gx/d3d/CGxDeviceD3d.hpp"
#include "gx/CGxBatch.hpp"
#include "gx/texture/CGxTex.hpp"
#include <algorithm>

D3DTEXTUREFILTERTYPE CGxDeviceD3d::s_filterModes[GxTexFilters_Last][3] = {
    // Min, Mag, Mip
    { D3DTEXF_POINT,    D3DTEXF_POINT,  D3DTEXF_NONE    },  // GxTex_Nearest
    { D3DTEXF_LINEAR,   D3DTEXF_LINEAR, D3DTEXF_NONE    },  // GxTex_Linear
    { D3DTEXF_POINT,    D3DTEXF_POINT,  D3DTEXF_POINT   },  // GxTex_NearestMipNearest
    { D3DTEXF_LINEAR,   D3DTEXF_LINEAR, D3DTEXF_POINT   },  // GxTex_LinearMipNearest
    { D3DTEXF_LINEAR,   D3DTEXF_LINEAR, D3DTEXF_LINEAR  },  // GxTex_LinearMipLinear
    { D3DTEXF_LINEAR,   D3DTEXF_LINEAR, D3DTEXF_LINEAR  },  // GxTex_Anisotropic
};

uint32_t CGxDeviceD3d::s_gxAttribToD3dAttribSize[] = {
    4,                      // type 0
    4,                      // type 1
    4,                      // type 2
    8,                      // type 3
    12,                     // type 4
    4,                      // type 5
    4,                      // type 6
};

D3DDECLTYPE CGxDeviceD3d::s_gxAttribToD3dAttribType[] = {
    D3DDECLTYPE_D3DCOLOR,   // type 0
    D3DDECLTYPE_UBYTE4,     // type 1
    D3DDECLTYPE_UBYTE4N,    // type 2
    D3DDECLTYPE_FLOAT2,     // type 3
    D3DDECLTYPE_FLOAT3,     // type 4
    D3DDECLTYPE_SHORT2,     // type 5
    D3DDECLTYPE_FLOAT1,     // type 6
};

D3DDECLUSAGE CGxDeviceD3d::s_gxAttribToD3dAttribUsage[] = {
    D3DDECLUSAGE_POSITION,      // GxVA_Position
    D3DDECLUSAGE_BLENDWEIGHT,   // GxVA_BlendWeight
    D3DDECLUSAGE_BLENDINDICES,  // GxVA_BlendIndices
    D3DDECLUSAGE_NORMAL,        // GxVA_Normal
    D3DDECLUSAGE_COLOR,         // GxVA_Color0
    D3DDECLUSAGE_COLOR,         // GxVA_Color1
    D3DDECLUSAGE_TEXCOORD,      // GxVA_TexCoord0
    D3DDECLUSAGE_TEXCOORD,      // GxVA_TexCoord1
    D3DDECLUSAGE_TEXCOORD,      // GxVA_TexCoord2
    D3DDECLUSAGE_TEXCOORD,      // GxVA_TexCoord3
    D3DDECLUSAGE_TEXCOORD,      // GxVA_TexCoord4
    D3DDECLUSAGE_TEXCOORD,      // GxVA_TexCoord5
    D3DDECLUSAGE_TEXCOORD,      // GxVA_TexCoord6
    D3DDECLUSAGE_TEXCOORD,      // GxVA_TexCoord7
};

uint32_t CGxDeviceD3d::s_gxAttribToD3dAttribUsageIndex[] = {
    0,                          // GxVA_Position
    0,                          // GxVA_BlendWeight
    0,                          // GxVA_BlendIndices
    0,                          // GxVA_Normal
    0,                          // GxVA_Color0
    1,                          // GxVA_Color1
    0,                          // GxVA_TexCoord0
    1,                          // GxVA_TexCoord1
    2,                          // GxVA_TexCoord2
    3,                          // GxVA_TexCoord3
    4,                          // GxVA_TexCoord4
    5,                          // GxVA_TexCoord5
    6,                          // GxVA_TexCoord6
    7,                          // GxVA_TexCoord7
};

D3DFORMAT CGxDeviceD3d::s_GxFormatToD3dFormat[] = {
    D3DFMT_R5G6B5,      // Fmt_Rgb565
    D3DFMT_X8R8G8B8,    // Fmt_ArgbX888
    D3DFMT_A8R8G8B8,    // Fmt_Argb8888
    D3DFMT_A2R10G10B10, // Fmt_Argb2101010
    D3DFMT_D16,         // Fmt_Ds160
    D3DFMT_D24X8,       // Fmt_Ds24X
    D3DFMT_D24S8,       // Fmt_Ds248
    D3DFMT_D32,         // Fmt_Ds320
};

D3DFORMAT CGxDeviceD3d::s_GxTexFmtToD3dFmt[] = {
    D3DFMT_UNKNOWN,     // GxTex_Unknown
    D3DFMT_A8B8G8R8,    // GxTex_Abgr8888
    D3DFMT_A8R8G8B8,    // GxTex_Argb8888
    D3DFMT_A4R4G4B4,    // GxTex_Argb4444
    D3DFMT_A1R5G5B5,    // GxTex_Argb1555
    D3DFMT_R5G6B5,      // GxTex_Rgb565
    D3DFMT_DXT1,        // GxTex_Dxt1
    D3DFMT_DXT3,        // GxTex_Dxt3
    D3DFMT_DXT5,        // GxTex_Dxt5
    D3DFMT_V8U8,        // GxTex_Uv88
    D3DFMT_G16R16F,     // GxTex_Gr1616F
    D3DFMT_R32F,        // GxTex_R32F
    D3DFMT_D24X8,       // GxTex_D24X8
};

EGxTexFormat CGxDeviceD3d::s_GxTexFmtToUse[] = {
    GxTex_Unknown,
    GxTex_Abgr8888,
    GxTex_Argb8888,
    GxTex_Argb4444,
    GxTex_Argb1555,
    GxTex_Rgb565,
    GxTex_Dxt1,
    GxTex_Dxt3,
    GxTex_Dxt5,
    GxTex_Uv88,
    GxTex_Gr1616F,
    GxTex_R32F,
    GxTex_D24X8,
};

D3DPRIMITIVETYPE CGxDeviceD3d::s_primitiveConversion[] = {
    D3DPT_POINTLIST,    // GxPrim_Points
    D3DPT_LINELIST,     // GxPrim_Lines
    D3DPT_LINESTRIP,    // GxPrim_LineStrip
    D3DPT_TRIANGLELIST, // GxPrim_Triangles
    D3DPT_TRIANGLESTRIP, // GxPrim_TriangleStrip
    D3DPT_TRIANGLEFAN,  // GxPrim_TriangleFan
};

EGxTexFormat CGxDeviceD3d::s_tolerableTexFmtMapping[] = {
    GxTex_Unknown,      // GxTex_Unknown
    GxTex_Argb4444,     // GxTex_Abgr8888
    GxTex_Argb4444,     // GxTex_Argb8888
    GxTex_Argb4444,     // GxTex_Argb4444
    GxTex_Argb4444,     // GxTex_Argb1555
    GxTex_Argb4444,     // GxTex_Rgb565
    GxTex_Dxt1,         // GxTex_Dxt1
    GxTex_Dxt3,         // GxTex_Dxt3
    GxTex_Dxt5,         // GxTex_Dxt5
    GxTex_Uv88,         // GxTex_Uv88
    GxTex_Gr1616F,      // GxTex_Gr1616F
    GxTex_R32F,         // GxTex_R32F
    GxTex_D24X8,        // GxTex_D24X8
};

D3DTEXTUREADDRESS CGxDeviceD3d::s_wrapModes[] = {
    D3DTADDRESS_CLAMP,  // GxTex_Clamp
    D3DTADDRESS_WRAP,   // GxTex_Wrap
};

ATOM WindowClassCreate() {
    auto instance = GetModuleHandle(nullptr);

    WNDCLASSEX wc = { 0 };

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = CGxDeviceD3d::WindowProcD3d;
    wc.hInstance = instance;
    wc.lpszClassName = TEXT("GxWindowClassD3d");

    wc.hIcon = static_cast<HICON>(LoadImage(instance, TEXT("BlizzardIcon.ico"), 1u, 0, 0, 0x40));
    wc.hCursor = LoadCursor(instance, TEXT("BlizzardCursor.cur"));

    if (!wc.hCursor) {
        wc.hCursor = LoadCursor(instance, IDC_ARROW);
    }

    return RegisterClassEx(&wc);
}

int32_t CGxDeviceD3d::ILoadD3dLib(HINSTANCE& d3dLib, LPDIRECT3D9& d3d) {
    d3dLib = nullptr;
    d3d = nullptr;

    d3dLib = LoadLibrary(TEXT("d3d9.dll"));

    if (d3dLib) {
        auto d3dCreateProc = GetProcAddress(d3dLib, "Direct3DCreate9");

        if (d3dCreateProc) {
            d3d = reinterpret_cast<LPDIRECT3D9>(d3dCreateProc());

            if (d3d) {
                return 1;
            }

            CGxDevice::Log("CGxDeviceD3d::ILoadD3dLib(): unable to d3dCreateProc()");
        } else {
            CGxDevice::Log("CGxDeviceD3d::ILoadD3dLib(): unable to GetProcAddress()");
        }
    } else {
        CGxDevice::Log("CGxDeviceD3d::ILoadD3dLib(): unable to LoadLibrary()");
    }

    CGxDeviceD3d::IUnloadD3dLib(d3dLib, d3d);

    return 0;
}

void CGxDeviceD3d::IUnloadD3dLib(HINSTANCE& d3dLib, LPDIRECT3D9& d3d) {
    if (d3d) {
        d3d->Release();
    }

    if (d3dLib) {
        FreeLibrary(d3dLib);
    }
}

LRESULT CGxDeviceD3d::WindowProcD3d(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto device = reinterpret_cast<CGxDeviceD3d*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (uMsg) {
    case WM_CREATE: {
        auto lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(lpcs->lpCreateParams));

        return 0;
    }

    case WM_DESTROY: {
        device->DeviceWM(GxWM_Destroy, 0, 0);

        return 0;
    }

    case WM_SIZE: {
        CRect windowRect = {
            0.0f,
            0.0f,
            static_cast<float>(HIWORD(lParam)),
            static_cast<float>(LOWORD(lParam))
        };

        int32_t resizeType = 0;
        if (wParam == SIZE_MINIMIZED) {
            resizeType = 1;
        } else if (wParam == SIZE_MAXHIDE) {
            resizeType = 2;
        }

        device->DeviceWM(GxWM_Size, reinterpret_cast<uintptr_t>(&windowRect), resizeType);

        break;
    }

    case WM_ACTIVATE: {
        // TODO

        break;
    }

    case WM_SETFOCUS: {
        device->DeviceWM(GxWM_SetFocus, 0, 0);

        return 0;
    }

    case WM_KILLFOCUS: {
        device->DeviceWM(GxWM_KillFocus, 0, 0);

        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT paint;
        BeginPaint(hWnd, &paint);
        EndPaint(hWnd, &paint);

        return 0;
    }

    case WM_ERASEBKGND: {
        return 0;
    }

    case WM_SETCURSOR: {
        // TODO

        return 1;
    }

    case WM_DISPLAYCHANGE: {
        // TODO

        break;
    }

    case WM_SYSCOMMAND: {
        // TODO

        break;
    }

    case WM_SIZING: {
        // TODO

        break;
    }

    default:
        break;
    }

    // TODO

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

CGxDeviceD3d::CGxDeviceD3d() : CGxDevice() {
    // TODO

    this->m_api = GxApi_D3d9;

    // TODO remove m_shaderProfiles in favor of caps-defined profiles
    this->m_shaderProfiles[GxSh_Vertex] = GxShVS_vs_3_0;
    this->m_shaderProfiles[GxSh_Pixel] = GxShPS_ps_3_0;

    // TODO

    memset(this->m_deviceStates, 0xFF, sizeof(this->m_deviceStates));

    // TODO

    this->DeviceCreatePools();
    this->DeviceCreateStreamBufs();
}

char* CGxDeviceD3d::BufLock(CGxBuf* buf) {
    CGxDevice::BufLock(buf);
    return this->IBufLock(buf);
}

int32_t CGxDeviceD3d::BufUnlock(CGxBuf* buf, uint32_t size) {
    CGxDevice::BufUnlock(buf, size);
    this->IBufUnlock(buf);

    return 1;
}

void CGxDeviceD3d::CapsWindowSize(CRect& dst) {
    // TODO
}

void CGxDeviceD3d::CapsWindowSizeInScreenCoords(CRect& dst) {
    // TODO
}

int32_t CGxDeviceD3d::CreatePoolAPI(CGxPool* pool) {
    if (pool->m_target == GxPoolTarget_Vertex) {
        pool->m_apiSpecific = this->ICreateD3dVB(pool->m_usage, pool->m_size);
    } else if (pool->m_target == GxPoolTarget_Index) {
        pool->m_apiSpecific = this->ICreateD3dIB(pool->m_usage, pool->m_size);
    }

    return 1;
}

int32_t CGxDeviceD3d::DeviceCreate(long (*windowProc)(void*, uint32_t, uint32_t, long), const CGxFormat& format) {
    this->m_ownhwnd = 1;

    // TODO gamma ramp

    this->m_hwndClass = WindowClassCreate();

    if (this->m_hwndClass) {
        if (this->ICreateD3d() && this->CGxDevice::DeviceCreate(windowProc, format)) {
            return 1;
        } else {
            // TODO
            // this->DeviceDestroy();
            return 0;
        }
    }

    // TODO CGxDevice::Log("CGxDeviceD3d::DeviceCreate(): WindowClassCreate() failed: %s", OsGetLastErrorStr());
    // TODO this->DeviceDestroy();

    return 0;
}

int32_t CGxDeviceD3d::DeviceSetFormat(const CGxFormat& format) {
    CGxDevice::Log("CGxDeviceD3d::DeviceSetFormat():");
    CGxDevice::Log(format);

    if (this->m_hwnd) {
        ShowWindow(this->m_hwnd, 0);
    }

    // TODO

    if (this->m_hwnd) {
        DestroyWindow(this->m_hwnd);
    }

    this->m_hwnd = nullptr;

    this->m_format = format;

    CGxFormat createFormat = format;

    if (this->ICreateWindow(createFormat) && this->ICreateD3dDevice(createFormat) && this->CGxDevice::DeviceSetFormat(format)) {
        this->intF64 = 1;

        // TODO

        if (this->m_format.window == 0) {
            RECT windowRect;
            GetWindowRect(this->m_hwnd, &windowRect);
            ClipCursor(&windowRect);
        }

        return 1;
    }
}

void CGxDeviceD3d::DeviceWM(EGxWM wm, uintptr_t param1, uintptr_t param2) {
    switch (wm) {
    case GxWM_Size: {
        if (param2 == 1 || param2 == 2) {
            this->m_windowVisible = 0;
        } else {
            this->m_windowVisible = 1;

            auto& windowRect = *reinterpret_cast<CRect*>(param1);
            this->DeviceSetDefWindow(windowRect);

            if (this->m_d3dDevice && this->m_context) {
                // TODO
            }
        }

        break;
    }
    }
}

void CGxDeviceD3d::Draw(CGxBatch* batch, int32_t indexed) {
    if (!this->m_context || this->intF5C) {
        return;
    }

    this->IStateSync();

    int32_t baseIndex = 0;
    if (!this->m_caps.int10) {
        baseIndex = this->m_primVertexFormatBuf[0]->m_index / this->m_primVertexFormatBuf[0]->m_itemSize;
    }

    if (indexed) {
        this->m_d3dDevice->DrawIndexedPrimitive(
            CGxDeviceD3d::s_primitiveConversion[batch->m_primType],
            baseIndex,
            batch->m_minIndex,
            batch->m_maxIndex - batch->m_minIndex + 1,
            batch->m_start + (this->m_primIndexBuf->m_index / 2),
            CGxDevice::PrimCalcCount(batch->m_primType, batch->m_count)
        );
    } else {
        this->m_d3dDevice->DrawPrimitive(
            CGxDeviceD3d::s_primitiveConversion[batch->m_primType],
            baseIndex,
            CGxDevice::PrimCalcCount(batch->m_primType, batch->m_count)
        );
    }
}

void CGxDeviceD3d::DsSet(EDeviceState state, uint32_t val) {
    // TODO
}

char* CGxDeviceD3d::IBufLock(CGxBuf* buf) {
    if (!this->m_context) {
        // TODO
        return nullptr;
    }

    auto pool = buf->m_pool;
    uint32_t lockFlags = 0x0;

    if (pool->m_usage == GxPoolUsage_Stream) {
        auto v6 = buf->m_itemSize + pool->unk1C - 1 - (buf->m_itemSize + pool->unk1C - 1) % buf->m_itemSize;
        if (buf->m_size + v6 <= pool->m_size) {
            lockFlags = D3DLOCK_NOOVERWRITE;
            buf->m_index = v6;
            pool->unk1C = buf->m_size + v6;
        } else {
            lockFlags = D3DLOCK_DISCARD;
            pool->Discard();
            buf->m_index = 0;
            pool->unk1C = buf->m_size;
        }
    } else if (pool->m_usage == GxPoolUsage_Dynamic) {
        lockFlags = D3DLOCK_NOOVERWRITE;
    }

    if (!pool->m_apiSpecific) {
        this->CreatePoolAPI(pool);
    }

    if (!pool->m_apiSpecific) {
        // TODO
        return nullptr;
    }

    // Invalid target
    if (pool->m_target >= GxPoolTargets_Last) {
        return nullptr;
    }

    char* data = nullptr;
    HRESULT lockResult = S_OK;

    if (pool->m_target == GxPoolTarget_Vertex) {
        auto d3dBuf = static_cast<LPDIRECT3DVERTEXBUFFER9>(pool->m_apiSpecific);
        lockResult = d3dBuf->Lock(buf->m_index, buf->m_size, reinterpret_cast<void**>(&data), lockFlags);
    } else if (pool->m_target == GxPoolTarget_Index) {
        auto d3dBuf = static_cast<LPDIRECT3DINDEXBUFFER9>(pool->m_apiSpecific);
        lockResult = d3dBuf->Lock(buf->m_index, buf->m_size, reinterpret_cast<void**>(&data), lockFlags);
    }

    if (SUCCEEDED(lockResult)) {
        if (buf->m_size) {
            // TODO

            if (pool->m_usage == GxPoolUsage_Stream) {
                *data = 0;
            } else {
                *data = *data;
            }

            // TODO
        }
    } else {
        this->IBufUnlock(buf);

        // TODO
        return nullptr;
    }

    return data;
}

void CGxDeviceD3d::IBufUnlock(CGxBuf* buf) {
    // TODO

    auto pool = buf->m_pool;

    if (pool->m_target == GxPoolTarget_Vertex) {
        auto d3dBuf = static_cast<LPDIRECT3DVERTEXBUFFER9>(pool->m_apiSpecific);
        buf->unk1D = SUCCEEDED(d3dBuf->Unlock());
    } else if (pool->m_target == GxPoolTarget_Index) {
        auto d3dBuf = static_cast<LPDIRECT3DINDEXBUFFER9>(pool->m_apiSpecific);
        buf->unk1D = SUCCEEDED(d3dBuf->Unlock());
    } else {
        buf->unk1D = 1;
    }
}

int32_t CGxDeviceD3d::ICreateD3d() {
    if (CGxDeviceD3d::ILoadD3dLib(this->m_d3dLib, this->m_d3d) && SUCCEEDED(this->m_d3d->GetDeviceCaps(0, D3DDEVTYPE_HAL, &this->m_d3dCaps))) {
        if (this->m_desktopDisplayMode.Format != D3DFMT_UNKNOWN) {
            return 1;
        }

        D3DDISPLAYMODE displayMode;
        if (SUCCEEDED(this->m_d3d->GetAdapterDisplayMode(0, &displayMode))) {
            this->m_desktopDisplayMode.Width = displayMode.Width;
            this->m_desktopDisplayMode.Height = displayMode.Height;
            this->m_desktopDisplayMode.RefreshRate = displayMode.RefreshRate;
            this->m_desktopDisplayMode.Format = displayMode.Format;

            return 1;
        }
    }

    this->IDestroyD3d();

    return 0;
}

int32_t CGxDeviceD3d::ICreateD3dDevice(const CGxFormat& format) {
    // TODO stereoscopic setup

    auto hwTnL = format.hwTnL;
    if (hwTnL && (this->m_d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0) {
        hwTnL = false;
    }
    this->m_d3dIsHwDevice = hwTnL;

    D3DPRESENT_PARAMETERS d3dpp;
    this->ISetPresentParms(d3dpp, format);

    uint32_t behaviorFlags = hwTnL
        ? D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE
        : D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE;

    if (SUCCEEDED(this->m_d3d->CreateDevice(0, D3DDEVTYPE_HAL, this->m_hwnd, behaviorFlags, &d3dpp, &this->m_d3dDevice))) {
        // TODO

        this->m_devAdapterFormat = d3dpp.BackBufferFormat;
        this->m_context = 1;

        // TODO

        this->ISetCaps(format);

        // TODO

        return 1;
    }

    this->m_d3dDevice = nullptr;
    return 0;
}

LPDIRECT3DINDEXBUFFER9 CGxDeviceD3d::ICreateD3dIB(EGxPoolUsage usage, uint32_t size) {
    uint32_t d3dUsage = this->m_d3dIsHwDevice ? D3DUSAGE_WRITEONLY : D3DUSAGE_SOFTWAREPROCESSING;
    D3DPOOL d3dPool = D3DPOOL_MANAGED;

    if (usage == GxPoolUsage_Dynamic || usage == GxPoolUsage_Stream) {
        d3dUsage |= D3DUSAGE_DYNAMIC;
        d3dPool = D3DPOOL_DEFAULT;
    }

    LPDIRECT3DINDEXBUFFER9 indexBuf = nullptr;

    if (SUCCEEDED(this->m_d3dDevice->CreateIndexBuffer(size, d3dUsage, D3DFMT_INDEX16, d3dPool, &indexBuf, nullptr))) {
        return indexBuf;
    }

    return nullptr;
}

LPDIRECT3DVERTEXBUFFER9 CGxDeviceD3d::ICreateD3dVB(EGxPoolUsage usage, uint32_t size) {
    uint32_t d3dUsage = this->m_d3dIsHwDevice ? D3DUSAGE_WRITEONLY : D3DUSAGE_SOFTWAREPROCESSING;
    D3DPOOL d3dPool = D3DPOOL_MANAGED;

    if (usage == GxPoolUsage_Dynamic || usage == GxPoolUsage_Stream) {
        d3dUsage |= D3DUSAGE_DYNAMIC;
        d3dPool = D3DPOOL_DEFAULT;
    }

    LPDIRECT3DVERTEXBUFFER9 vertexBuf = nullptr;

    if (SUCCEEDED(this->m_d3dDevice->CreateVertexBuffer(size, d3dUsage, D3DFMT_INDEX16, d3dPool, &vertexBuf, nullptr))) {
        return vertexBuf;
    }

    return nullptr;
}

LPDIRECT3DVERTEXDECLARATION9 CGxDeviceD3d::ICreateD3dVertexDecl(D3DVERTEXELEMENT9 elements[], uint32_t count) {
    if (this->m_primVertexFormat < GxVertexBufferFormats_Last) {
        for (int32_t i = 0; i < count; i++) {
            auto& element = elements[i];
            auto foo = 1;
        }

        if (!this->m_d3dVertexDecl[this->m_primVertexFormat]) {
            this->m_d3dDevice->CreateVertexDeclaration(elements, &this->m_d3dVertexDecl[this->m_primVertexFormat]);
        }

        return this->m_d3dVertexDecl[this->m_primVertexFormat];
    }

    // TODO new vertex buffer format

    return nullptr;
}

bool CGxDeviceD3d::ICreateWindow(CGxFormat& format) {
    auto instance = GetModuleHandle(nullptr);

    DWORD dwStyle;
    if (format.window == 0) {
        dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;
    } else if (format.maximize == 1) {
        dwStyle = WS_POPUP | WS_VISIBLE;
    } else if (format.maximize == 2) {
        dwStyle = WS_POPUP;
    } else {
        dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    }

    // TODO

    int32_t width = format.size.x ? format.size.x : CW_USEDEFAULT;
    int32_t height = format.size.y ? format.size.y : CW_USEDEFAULT;

    if (format.window && format.maximize != 1 && format.size.x && format.size.y) {
        // TODO adjust width and height
    }

    this->m_hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        TEXT("GxWindowClassD3d"),
        TEXT("World of Warcraft"),
        dwStyle,
        format.pos.x,
        format.pos.y,
        width,
        height,
        nullptr,
        nullptr,
        instance,
        this
    );

    if (this->m_hwnd && format.maximize != 2) {
        ShowWindow(this->m_hwnd, 1);
    }

    return this->m_hwnd != nullptr;
}

void CGxDeviceD3d::IDestroyD3d() {
    this->IDestroyD3dDevice();
    CGxDeviceD3d::IUnloadD3dLib(this->m_d3dLib, this->m_d3d);
}

void CGxDeviceD3d::IDestroyD3dDevice() {
    // TODO
}

void CGxDeviceD3d::IRsSendToHw(EGxRenderState which) {
    auto state = &this->m_appRenderStates[which];

    switch (which) {
    // TODO handle all render states

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
        uint32_t tmu = which - GxRs_Texture0;
        auto texture = static_cast<CGxTex*>(static_cast<void*>(state->m_value));
        this->ISetTexture(tmu, texture);

        break;
    }

    case GxRs_VertexShader: {
        auto shader = static_cast<CGxShader*>(static_cast<void*>(state->m_value));
        this->IShaderBindVertex(shader);

        break;
    }

    case GxRs_PixelShader: {
        auto shader = static_cast<CGxShader*>(static_cast<void*>(state->m_value));
        this->IShaderBindPixel(shader);

        break;
    }

    default:
        break;
    }
}

void CGxDeviceD3d::ISceneBegin() {
    if (this->m_context) {
        // TODO

        if (SUCCEEDED(this->m_d3dDevice->BeginScene())) {
            this->m_inScene = 1;
        }

        return;
    }

    // TODO
}

void CGxDeviceD3d::ISceneEnd() {
    if (this->m_inScene) {
        this->m_d3dDevice->EndScene();
        this->m_inScene = 0;
    }
}

void CGxDeviceD3d::ISetCaps(const CGxFormat& format) {
    // Texture stages

    int32_t maxSimultaneousTextures = this->m_d3dCaps.MaxSimultaneousTextures;
    this->m_caps.m_numTmus = std::min(maxSimultaneousTextures, 8);

    // Rasterization rules

    this->m_caps.m_pixelCenterOnEdge = 0;
    this->m_caps.m_texelCenterOnEdge = 1;

    // Max texture size

    uint32_t maxTextureWidth = this->m_d3dCaps.MaxTextureWidth;
    this->m_caps.m_texMaxSize[GxTex_2d] = std::max(maxTextureWidth, 256u);
    this->m_caps.m_texMaxSize[GxTex_CubeMap] = std::max(maxTextureWidth, 256u);
    this->m_caps.m_texMaxSize[GxTex_Rectangle] = std::max(maxTextureWidth, 256u);
    this->m_caps.m_texMaxSize[GxTex_NonPow2] = std::max(maxTextureWidth, 256u);

    // Max vertex index

    this->m_caps.m_maxIndex = this->m_d3dCaps.MaxVertexIndex;

    // Trilinear filtering

    this->m_caps.m_texFilterTrilinear =
        (this->m_d3dCaps.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR) != 0;

    // Anisotropic filtering

    this->m_caps.m_texFilterAnisotropic =
        (this->m_d3dCaps.TextureFilterCaps & (D3DPTFILTERCAPS_MINFANISOTROPIC | D3DPTFILTERCAPS_MAGFANISOTROPIC)) != 0;

    if (this->m_d3dCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC) {
        CGxDeviceD3d::s_filterModes[GxTex_Anisotropic][0] = D3DTEXF_ANISOTROPIC;
    }

    if (this->m_d3dCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC) {
        CGxDeviceD3d::s_filterModes[GxTex_Anisotropic][1] = D3DTEXF_ANISOTROPIC;
    }

    this->m_caps.m_maxTexAnisotropy = this->m_d3dCaps.MaxAnisotropy;

    if (this->m_caps.m_texFilterAnisotropic && this->m_d3dCaps.MaxAnisotropy < 2) {
        this->m_caps.m_texFilterAnisotropic = 0;
    }

    // Misc capabilities

    this->m_caps.m_depthBias = (this->m_d3dCaps.RasterCaps & D3DPRASTERCAPS_DEPTHBIAS) != 0;
    this->m_caps.m_numStreams = this->m_d3dCaps.MaxStreams;
    this->m_caps.int10 = (this->m_d3dCaps.Caps2 & 1) != 0; // unknown caps flag

    // Shader targets

    auto pixelShaderVersion = this->m_d3dCaps.PixelShaderVersion;

    if (pixelShaderVersion >= D3DPS_VERSION(3, 0)) {
        this->m_caps.m_pixelShaderTarget = GxShPS_ps_3_0;
    } else if (pixelShaderVersion >= D3DPS_VERSION(2, 0)) {
        this->m_caps.m_pixelShaderTarget = GxShPS_ps_2_0;
    } else if (pixelShaderVersion >= D3DPS_VERSION(1, 4)) {
        this->m_caps.m_pixelShaderTarget = GxShPS_ps_1_4;
    } else if (pixelShaderVersion >= D3DPS_VERSION(1, 1)) {
        this->m_caps.m_pixelShaderTarget = GxShPS_ps_1_1;
    }

    if (this->m_caps.m_pixelShaderTarget != GxShPS_none) {
        auto vertexShaderVersion = this->m_d3dCaps.VertexShaderVersion;

        if (vertexShaderVersion >= D3DVS_VERSION(3, 0)) {
            this->m_caps.m_vertexShaderTarget = GxShVS_vs_3_0;
        } else if (vertexShaderVersion >= D3DVS_VERSION(2, 0)) {
            this->m_caps.m_vertexShaderTarget = GxShVS_vs_2_0;
        } else if (vertexShaderVersion == D3DVS_VERSION(1, 1)) {
            this->m_caps.m_vertexShaderTarget = GxShVS_vs_1_1;
        }

        // TODO maxVertexShaderConst
    }

    // TODO modify shader targets based on format

    // Texture formats

    for (int32_t i = 0; i < GxTexFormats_Last; i++) {
        if (i == GxTex_Unknown) {
            this->m_caps.m_texFmt[i] = 0;
        } else {
            this->m_caps.m_texFmt[i] = this->m_d3d->CheckDeviceFormat(
                0,
                D3DDEVTYPE_HAL,
                this->m_devAdapterFormat,
                0,
                D3DRTYPE_TEXTURE,
                CGxDeviceD3d::s_GxTexFmtToD3dFmt[i]
            ) == D3D_OK;
        }
    }

    this->m_caps.m_generateMipMaps = (this->m_d3dCaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP) != 0;

    // TODO

    // Texture targets

    this->m_caps.m_texTarget[GxTex_2d] = 1;
    this->m_caps.m_texTarget[GxTex_CubeMap] = (this->m_d3dCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP) != 0;
    this->m_caps.m_texTarget[GxTex_Rectangle] = 0;
    this->m_caps.m_texTarget[GxTex_NonPow2] =
        (this->m_d3dCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL) != 0 || (this->m_d3dCaps.TextureCaps & D3DPTEXTURECAPS_POW2) == 0;

    // TODO
}

void CGxDeviceD3d::ISetPresentParms(D3DPRESENT_PARAMETERS& d3dpp, const CGxFormat& format) {
    memset(&d3dpp, 0, sizeof(d3dpp));

    if (format.window) {
        D3DDISPLAYMODE currentMode;
        D3DFORMAT backBufferFormat;
        if (SUCCEEDED(this->m_d3d->GetAdapterDisplayMode(0, &currentMode))) {
            backBufferFormat = currentMode.Format;
        } else {
            backBufferFormat = this->m_desktopDisplayMode.Format;
        }

        auto& windowRect = this->DeviceCurWindow();

        d3dpp.Windowed = true;
        d3dpp.BackBufferWidth = windowRect.maxX;
        d3dpp.BackBufferHeight = windowRect.maxY;
        d3dpp.BackBufferFormat = backBufferFormat;

        if (format.vsync) {
            // TODO d3dpp.BackBufferCount = format.int1C;
            d3dpp.BackBufferCount = 1;
        } else {
            d3dpp.BackBufferCount = 1;
        }

        d3dpp.FullScreen_RefreshRateInHz = 0;
    } else {
        d3dpp.BackBufferWidth = format.size.x;
        d3dpp.BackBufferHeight = format.size.y;
        d3dpp.BackBufferFormat = CGxDeviceD3d::s_GxFormatToD3dFormat[format.colorFormat];
        d3dpp.FullScreen_RefreshRateInHz = format.refreshRate;
    }

    d3dpp.hDeviceWindow = this->m_hwnd;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.EnableAutoDepthStencil = true;
    d3dpp.AutoDepthStencilFormat = CGxDeviceD3d::s_GxFormatToD3dFormat[format.depthFormat];

    switch (format.vsync) {
    case 1:
        d3dpp.PresentationInterval = 1;
        break;
    case 2:
        d3dpp.PresentationInterval = format.window ? 1 : 2;
        break;
    case 3:
        d3dpp.PresentationInterval = format.window ? 1 : 4;
        break;
    case 4:
        d3dpp.PresentationInterval = format.window ? 1 : 8;
        break;
    default:
        d3dpp.PresentationInterval = CW_USEDEFAULT;
        break;
    }

    if (format.sampleCount <= 1) {
        d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
    } else {
        d3dpp.MultiSampleType = static_cast<D3DMULTISAMPLE_TYPE>(format.sampleCount);

        // TODO MultiSampleQuality
    }
}

void CGxDeviceD3d::ISetTexture(uint32_t tmu, CGxTex* texId) {
    if (tmu > 15) {
        return;
    }

    if (texId) {
        this->ITexMarkAsUpdated(texId);
        this->m_d3dDevice->SetTexture(tmu, static_cast<LPDIRECT3DBASETEXTURE9>(texId->m_apiSpecificData));

        // Texture filters
        auto& filters = CGxDeviceD3d::s_filterModes[texId->m_flags.m_filter];
        this->DsSet(static_cast<EDeviceState>(Ds_TssMinFilter0 + tmu), filters[0]);
        this->DsSet(static_cast<EDeviceState>(Ds_TssMagFilter0 + tmu), filters[1]);
        this->DsSet(static_cast<EDeviceState>(Ds_TssMipFilter0 + tmu), filters[2]);

        // Texture addressing
        this->DsSet(static_cast<EDeviceState>(Ds_TssWrapU0 + tmu), CGxDeviceD3d::s_wrapModes[texId->m_flags.m_wrapU]);
        this->DsSet(static_cast<EDeviceState>(Ds_TssWrapV0 + tmu), CGxDeviceD3d::s_wrapModes[texId->m_flags.m_wrapV]);

        // Max anisotropy
        this->DsSet(static_cast<EDeviceState>(Ds_TssMaxAnisotropy0 + tmu), texId->m_flags.m_maxAnisotropy);

        if (tmu < 8) {
            // TODO FFP
        }
    } else {
        this->m_d3dDevice->SetTexture(tmu, nullptr);

        if (tmu < 8) {
            // TODO FFP
        }
    }
}

void CGxDeviceD3d::ISetVertexBuffer(uint32_t stream, LPDIRECT3DVERTEXBUFFER9 buffer, uint32_t offset, uint32_t stride) {
    if (!this->m_caps.int10) {
        offset = 0;
    }

    if (this->m_d3dVertexStreamBuf[stream] != buffer || this->m_d3dVertexStreamOfs[stream] != offset || this->m_d3dVertexStreamStride[stream] != stride) {
        this->m_d3dDevice->SetStreamSource(stream, buffer, offset, stride);

        this->m_d3dVertexStreamBuf[stream] = buffer;
        this->m_d3dVertexStreamOfs[stream] = offset;
        this->m_d3dVertexStreamStride[stream] = stride;
    }
}

void CGxDeviceD3d::IShaderBindPixel(CGxShader* shader) {
    if (!shader) {
        this->m_d3dDevice->SetPixelShader(nullptr);

        // TODO FFP handling

        return;
    }

    if (!shader->loaded) {
        this->IShaderCreatePixel(shader);
    }

    auto d3dShader = static_cast<LPDIRECT3DPIXELSHADER9>(shader->apiSpecific);
    this->m_d3dDevice->SetPixelShader(d3dShader);
}

void CGxDeviceD3d::IShaderBindVertex(CGxShader* shader) {
    if (!shader) {
        this->m_d3dDevice->SetVertexShader(nullptr);
        return;
    }

    if (!shader->loaded) {
        this->IShaderCreateVertex(shader);
    }

    auto d3dShader = static_cast<LPDIRECT3DVERTEXSHADER9>(shader->apiSpecific);
    this->m_d3dDevice->SetVertexShader(d3dShader);
}

void CGxDeviceD3d::IShaderConstantsFlush() {
    // TODO
}

void CGxDeviceD3d::IShaderCreate(CGxShader* shader) {
    if (shader->target == GxSh_Vertex) {
        this->IShaderCreateVertex(shader);
    } else if (shader->target == GxSh_Pixel) {
        this->IShaderCreatePixel(shader);
    }
}

void CGxDeviceD3d::IShaderCreatePixel(CGxShader* shader) {
    shader->valid = 0;

    if (!this->m_context) {
        return;
    }

    shader->loaded = 1;

    if (shader->code.Count() == 0) {
        return;
    }

    LPDIRECT3DPIXELSHADER9 d3dShader;
    if (SUCCEEDED(this->m_d3dDevice->CreatePixelShader(reinterpret_cast<DWORD*>(shader->code.Ptr()), &d3dShader))) {
        shader->apiSpecific = d3dShader;
        shader->valid = 1;
    }
}

void CGxDeviceD3d::IShaderCreateVertex(CGxShader* shader) {
    shader->valid = 0;

    if (!this->m_context) {
        return;
    }

    shader->loaded = 1;

    if (shader->code.Count() == 0) {
        return;
    }

    LPDIRECT3DVERTEXSHADER9 d3dShader;
    if (SUCCEEDED(this->m_d3dDevice->CreateVertexShader(reinterpret_cast<DWORD*>(shader->code.Ptr()), &d3dShader))) {
        shader->apiSpecific = d3dShader;
        shader->valid = 1;
    }
}

void CGxDeviceD3d::IStateSync() {
    // TODO

    this->IShaderConstantsFlush();
    this->IRsSync(0);

    // TODO ffp if vertex shader is disabled

    this->IStateSyncEnables();

    // TODO

    this->IStateSyncVertexPtrs();
    this->IStateSyncIndexPtr();

    // TODO
}

void CGxDeviceD3d::IStateSyncEnables() {
    // TODO
}

void CGxDeviceD3d::IStateSyncIndexPtr() {
    if (!this->m_primIndexDirty) {
        return;
    }

    this->m_primIndexDirty = 0;

    auto d3dIndexBuf = static_cast<LPDIRECT3DINDEXBUFFER9>(this->m_primIndexBuf->m_pool->m_apiSpecific);

    if (this->m_d3dCurrentIndexBuf != d3dIndexBuf) {
        this->m_d3dDevice->SetIndices(d3dIndexBuf);
        this->m_d3dCurrentIndexBuf = d3dIndexBuf;
    }
}

void CGxDeviceD3d::IStateSyncVertexPtrs() {
    if (this->m_primVertexFormat < GxVertexBufferFormats_Last && this->m_d3dVertexDecl[this->m_primVertexFormat]) {
        auto d3dVertexDecl = this->m_d3dVertexDecl[this->m_primVertexFormat];

        if (this->m_d3dCurrentVertexDecl != d3dVertexDecl) {
            this->m_d3dDevice->SetVertexDeclaration(d3dVertexDecl);
            this->m_d3dCurrentVertexDecl = d3dVertexDecl;
        }

        this->ISetVertexBuffer(
            0,
            static_cast<LPDIRECT3DVERTEXBUFFER9>(this->m_primVertexBuf->m_pool->m_apiSpecific),
            this->m_primVertexBuf->m_index,
            this->m_primVertexSize
        );

        return;
    }

    CGxBuf* streamBufs[GxVAs_Last] = { 0 };
    uint32_t streamSizes[GxVAs_Last] = { 0 };

    D3DVERTEXELEMENT9 elements[GxVAs_Last + 1];
    uint32_t elementCount = 0;
    uint32_t streamCount = 0;

    for (uint32_t i = 0; i < GxVAs_Last; i++) {
        if ((1 << i) & this->m_primVertexMask) {
            uint32_t stream = 0;

            if (streamCount) {
                do {
                    if (streamBufs[stream] == this->m_primVertexFormatBuf[i]) {
                        break;
                    }
                    stream++;
                } while (stream < streamCount);
            }

            if (stream == streamCount) {
                streamBufs[stream] = this->m_primVertexFormatBuf[i];
                streamCount++;
            }

            auto& attrib = this->m_primVertexFormatAttrib[i];

            streamSizes[stream] += CGxDeviceD3d::s_gxAttribToD3dAttribSize[attrib.type];

            elements[elementCount].Stream = stream;
            elements[elementCount].Offset = attrib.offset;
            elements[elementCount].Type = CGxDeviceD3d::s_gxAttribToD3dAttribType[attrib.type];
            elements[elementCount].Method = D3DDECLMETHOD_DEFAULT;
            elements[elementCount].Usage = CGxDeviceD3d::s_gxAttribToD3dAttribUsage[attrib.attrib];
            elements[elementCount].UsageIndex = CGxDeviceD3d::s_gxAttribToD3dAttribUsageIndex[attrib.attrib];

            elementCount++;
        }
    }

    elements[elementCount] = D3DDECL_END();
    elementCount++;

    auto d3dVertexDecl = this->ICreateD3dVertexDecl(elements, elementCount);
    if (this->m_d3dCurrentVertexDecl != d3dVertexDecl) {
        this->m_d3dDevice->SetVertexDeclaration(d3dVertexDecl);
        this->m_d3dCurrentVertexDecl = d3dVertexDecl;
    }

    for (uint32_t stream = 0; stream < streamCount; stream++) {
        auto streamBuf = streamBufs[stream];

        this->ISetVertexBuffer(
            stream,
            static_cast<LPDIRECT3DVERTEXBUFFER9>(streamBuf->m_pool->m_apiSpecific),
            streamBuf->m_index,
            streamSizes[stream]
        );
    }
}

void CGxDeviceD3d::ITexCreate(CGxTex* texId) {
    uint32_t width, height, startLevel, endLevel;
    this->ITexWHDStartEnd(texId, width, height, startLevel, endLevel);

    texId->m_format = CGxDeviceD3d::s_GxTexFmtToUse[texId->m_format];

    uint32_t d3dUsage = 0;
    D3DPOOL d3dPool = D3DPOOL_MANAGED;

    if (texId->m_flags.m_renderTarget) {
        d3dUsage = D3DUSAGE_RENDERTARGET;
        d3dPool = D3DPOOL_DEFAULT;
    }

    if (texId->m_flags.m_generateMipMaps) {
        d3dUsage |= D3DUSAGE_AUTOGENMIPMAP;
    }

    // Cube map
    if (texId->m_target == GxTex_CubeMap) {
        auto d3dFormat = CGxDeviceD3d::s_GxTexFmtToD3dFmt[texId->m_format];
        LPDIRECT3DCUBETEXTURE9 d3dTexture;

        if (SUCCEEDED(this->m_d3dDevice->CreateCubeTexture(width, endLevel - startLevel, d3dUsage, d3dFormat, d3dPool, &d3dTexture, nullptr))) {
            texId->m_apiSpecificData = d3dTexture;
            texId->m_needsCreation = 0;
        }

        return;
    }

    // Depth stencil
    if (texId->m_format == GxTex_D24X8) {
        d3dUsage = D3DUSAGE_DEPTHSTENCIL;
        auto d3dFormat = D3DFMT_D24X8;
        LPDIRECT3DTEXTURE9 d3dTexture;

        if (SUCCEEDED(this->m_d3dDevice->CreateTexture(width, height, 1, d3dUsage, d3dFormat, d3dPool, &d3dTexture, nullptr))) {
            texId->m_apiSpecificData = d3dTexture;
            texId->m_needsCreation = 0;
        }

        return;
    }

    // Ordinary texture
    LPDIRECT3DTEXTURE9 d3dTexture;
    auto d3dFormat = CGxDeviceD3d::s_GxTexFmtToD3dFmt[texId->m_format];

    if (SUCCEEDED(this->m_d3dDevice->CreateTexture(width, height, endLevel - startLevel, d3dUsage, d3dFormat, d3dPool, &d3dTexture, nullptr))) {
        texId->m_apiSpecificData = d3dTexture;
        texId->m_needsCreation = 0;

        return;
    }

    // TODO flag check SLOBYTE(texId->m_flags)

    // If texture creation failed, try again with a fallback format
    CGxDeviceD3d::s_GxTexFmtToUse[texId->m_format] = CGxDeviceD3d::s_tolerableTexFmtMapping[texId->m_format];
    texId->m_format = CGxDeviceD3d::s_GxTexFmtToUse[texId->m_format];
    d3dFormat = CGxDeviceD3d::s_GxTexFmtToD3dFmt[texId->m_format];

    if (SUCCEEDED(this->m_d3dDevice->CreateTexture(width, height, endLevel - startLevel, d3dUsage, d3dFormat, d3dPool, &d3dTexture, nullptr))) {
        texId->m_apiSpecificData = d3dTexture;
        texId->m_needsCreation = 0;
    }
}

void CGxDeviceD3d::ITexMarkAsUpdated(CGxTex* texId) {
    if (!texId->m_needsUpdate || !this->m_context) {
        return;
    }

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

void CGxDeviceD3d::ITexUpload(CGxTex* texId) {
    // TODO
}

void CGxDeviceD3d::PoolSizeSet(CGxPool* pool, uint32_t size) {
    // TODO
}

void CGxDeviceD3d::ScenePresent() {
    if (this->m_context) {
        CGxDevice::ScenePresent();
        this->ISceneEnd();

        // TODO

        // TODO fixLag

        // TODO

        if (FAILED(this->m_d3dDevice->Present(nullptr, nullptr, nullptr, nullptr))) {
            this->m_context = 0;
        }

        // TODO stereo handling
    }

    this->ISceneBegin();
}

void CGxDeviceD3d::ShaderCreate(CGxShader* shaders[], EGxShTarget target, const char* a4, const char* a5, int32_t permutations) {
    CGxDevice::ShaderCreate(shaders, target, a4, a5, permutations);

    if (permutations == 1 && !shaders[0]->loaded) {
        this->IShaderCreate(shaders[0]);
    }
}

int32_t CGxDeviceD3d::StereoEnabled() {
    // TODO
    return 0;
}
