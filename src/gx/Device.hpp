#ifndef GX_DEVICE_HPP
#define GX_DEVICE_HPP

#include "gx/CGxDevice.hpp"
#include "gx/Types.hpp"
#include <cstdint>

class CGxFormat;

extern CGxDevice* g_theGxDevicePtr;

CGxDevice* GxDevCreate(EGxApi api, int32_t (*windowProc)(void* window, uint32_t message, uintptr_t wparam, intptr_t lparam), const CGxFormat& format);

EGxApi GxDevApi(void);

int32_t GxMasterEnable(EGxMasterEnables state);

#endif
