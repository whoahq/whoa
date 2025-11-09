#include "gx/Adapter.hpp"
#include "gx/CGxDevice.hpp"

int32_t GxAdapterMonitorModes(TSGrowableArray<CGxMonitorMode>& monitorModes) {
    return CGxDevice::AdapterMonitorModes(monitorModes);
}
