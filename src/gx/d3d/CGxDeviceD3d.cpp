#include "gx/d3d/CGxDeviceD3d.hpp"
#include "gx/texture/CGxTex.hpp"

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

    // TODO

    this->DeviceCreatePools();
    this->DeviceCreateStreamBufs();
}

void CGxDeviceD3d::CapsWindowSize(CRect& dst) {
    // TODO
}

void CGxDeviceD3d::CapsWindowSizeInScreenCoords(CRect& dst) {
    // TODO
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
    // TODO
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

        this->m_context = 1;

        // TODO

        return 1;
    }

    this->m_d3dDevice = nullptr;
    return 0;
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

void CGxDeviceD3d::IRsSendToHw(EGxRenderState rs) {
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

void CGxDeviceD3d::IShaderCreate(CGxShader* shader) {
    // TODO
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

int32_t CGxDeviceD3d::StereoEnabled() {
    // TODO
    return 0;
}
