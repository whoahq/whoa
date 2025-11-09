#include "gx/CGxMonitorMode.hpp"

int32_t CGxMonitorModeSort(const void* a, const void* b) {
    auto modeA = static_cast<const CGxMonitorMode*>(a);
    auto modeB = static_cast<const CGxMonitorMode*>(b);

    return (modeA->size.x * modeA->size.y) - (modeB->size.x * modeB->size.y);
}
