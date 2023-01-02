#ifndef GX_DEVICE_HPP
#define GX_DEVICE_HPP

#include "gx/CGxDevice.hpp"
#include "gx/Types.hpp"
#include <cstdint>

class CGxFormat;

extern CGxDevice* g_theGxDevicePtr;

CGxDevice* GxDevCreate(EGxApi, long (*)(void*, uint32_t, uint32_t, long), const CGxFormat&);

EGxApi GxDevApi(void);

int32_t GxMasterEnable(EGxMasterEnables state);

#endif
