#ifndef GX_D3D_C_GX_DEVICE_D3D_HPP
#define GX_D3D_C_GX_DEVICE_D3D_HPP

#include "gx/CGxDevice.hpp"                             
#include <cstdint>
#include <d3d9.h>
#include <windows.h>

class CGxDeviceD3d : public CGxDevice {
    public:
    // Static functions
    static int32_t ILoadD3dLib(HINSTANCE& d3dLib, LPDIRECT3D9& d3d);
    static void IUnloadD3dLib(HINSTANCE& d3dLib, LPDIRECT3D9& d3d);
    static LRESULT WindowProcD3d(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Member variables
    HWND m_hwnd = nullptr;
    ATOM m_hwndClass;
    int32_t m_ownhwnd;
    HINSTANCE m_d3dLib = nullptr;
    LPDIRECT3D9 m_d3d = nullptr;
    LPDIRECT3DDEVICE9 m_d3dDevice = nullptr;
    D3DCAPS9 m_d3dCaps;
    D3DDISPLAYMODE m_desktopDisplayMode;

    // Virtual member functions
    virtual void ITexMarkAsUpdated(CGxTex* texId);
    virtual void IRsSendToHw(EGxRenderState rs);
    virtual int32_t DeviceCreate(long (*windowProc)(void*, uint32_t, uint32_t, long), const CGxFormat& format);
    virtual int32_t DeviceSetFormat(const CGxFormat& format);
    virtual void CapsWindowSize(CRect& dst);
    virtual void CapsWindowSizeInScreenCoords(CRect& dst);
    virtual void PoolSizeSet(CGxPool* pool, uint32_t size);
    virtual void IShaderCreate(CGxShader* shader);
    virtual int32_t StereoEnabled();

    // Member functions
    int32_t ICreateD3d();
    int32_t ICreateD3dDevice(const CGxFormat& format);
    bool ICreateWindow(CGxFormat& format);
    void IDestroyD3d();
    void IDestroyD3dDevice();
};

#endif
