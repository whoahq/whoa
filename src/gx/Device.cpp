#include "gx/Device.hpp"
#include "gx/CGxDevice.hpp"
#include "gx/Gx.hpp"

CGxDevice* g_theGxDevicePtr = nullptr;

CGxDevice* GxDevCreate(EGxApi api, int32_t (*windowProc)(void* window, uint32_t message, uintptr_t wparam, intptr_t lparam), const CGxFormat& format) {
    CGxDevice* device;

    #if defined(WHOA_SYSTEM_WIN)
        if (api == GxApi_OpenGl) {
            device = CGxDevice::NewOpenGl();
        } else if (api == GxApi_D3d9) {
            device = CGxDevice::NewD3d();
        } else if (api == GxApi_D3d9Ex) {
            device = CGxDevice::NewD3d9Ex();
        } else {
            // Error
        }
    #endif

    #if defined(WHOA_SYSTEM_MAC)
        if (api == GxApi_OpenGl) {
            device = CGxDevice::NewOpenGl();
        } else if (api == GxApi_GLL) {
            device = CGxDevice::NewGLL();
        } else {
            // Error
        }
    #endif

    g_theGxDevicePtr = device;

    if (g_theGxDevicePtr->DeviceCreate(windowProc, format)) {
        return g_theGxDevicePtr;
    } else {
        if (g_theGxDevicePtr) {
            delete g_theGxDevicePtr;
        }

        return nullptr;
    }
}

EGxApi GxDevApi() {
    return g_theGxDevicePtr->m_api;
}

int32_t GxMasterEnable(EGxMasterEnables state) {
    return g_theGxDevicePtr->MasterEnable(state);
}
