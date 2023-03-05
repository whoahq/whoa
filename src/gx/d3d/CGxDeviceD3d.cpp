#include "gx/d3d/CGxDeviceD3d.hpp"

ATOM WindowClassCreate() {
    auto instance = GetModuleHandle(nullptr);

    WNDCLASSEX wc = { 0 };

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = CGxDeviceD3d::WindowProcD3d;
    wc.hInstance = instance;
    wc.lpszClassName = "GxWindowClassD3d";

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
    // TODO
    return 0;
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

int32_t CGxDeviceD3d::ICreateD3d() {
    if (CGxDeviceD3d::ILoadD3dLib(this->m_d3dLib, this->m_d3d) && this->m_d3d->GetDeviceCaps(0, D3DDEVTYPE_HAL, &this->m_d3dCaps) >= S_OK) {
        if (this->m_desktopDisplayMode.Format != D3DFMT_UNKNOWN) {
            return 1;
        }

        D3DDISPLAYMODE displayMode;
        if (this->m_d3d->GetAdapterDisplayMode(0, &displayMode) >= S_OK) {
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

void CGxDeviceD3d::IShaderCreate(CGxShader* shader) {
    // TODO
}

void CGxDeviceD3d::ITexMarkAsUpdated(CGxTex* texId) {
    // TODO
}

void CGxDeviceD3d::PoolSizeSet(CGxPool* pool, uint32_t size) {
    // TODO
}

int32_t CGxDeviceD3d::StereoEnabled() {
    // TODO
    return 0;
}
