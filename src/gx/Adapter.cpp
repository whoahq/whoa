#include "gx/Adapter.hpp"
#include "gx/CGxDevice.hpp"
#include "gx/CGxMonitorMode.hpp"

int32_t GxAdapterFormats(EGxApi api, TSGrowableArray<CGxFormat>& adapterFormats) {
    return CGxDevice::AdapterFormats(api, adapterFormats);
}

int32_t GxAdapterMonitorModes(TSGrowableArray<CGxMonitorMode>& monitorModes) {
    return CGxDevice::AdapterMonitorModes(monitorModes);
}
