#ifndef GX_ADAPTER_HPP
#define GX_ADAPTER_HPP

#include "gx/CGxFormat.hpp"
#include "gx/CGxMonitorMode.hpp"
#include "gx/Types.hpp"
#include <storm/Array.hpp>
#include <cstdint>

int32_t GxAdapterFormats(EGxApi api, TSGrowableArray<CGxFormat>& adapterFormats);

int32_t GxAdapterMonitorModes(TSGrowableArray<CGxMonitorMode>& monitorModes);

#endif
