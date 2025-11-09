#include "gx/win/Display.hpp"

int32_t FindDisplayDevice(PDISPLAY_DEVICE device, uint32_t flag) {
    device->cb = sizeof(DISPLAY_DEVICE);

    DWORD deviceNum = 0;
    while (EnumDisplayDevices(nullptr, deviceNum++, device, 0)) {
        if ((device->StateFlags & flag) == flag) {
            return 1;
        }
    }

    return 0;
}
