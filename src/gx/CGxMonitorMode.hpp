#ifndef GX_C_GX_MONITOR_MODE_HPP
#define GX_C_GX_MONITOR_MODE_HPP

#include <tempest/Vector.hpp>
#include <cstdint>

class CGxMonitorMode {
    public:
        C2iVector size;
        uint32_t bpp;
        uint32_t refreshRate;
};

int32_t CGxMonitorModeSort(void const* modeA, void const* modeB);

#endif
