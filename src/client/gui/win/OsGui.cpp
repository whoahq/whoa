#include "client/gui/OsGui.hpp"
#include <windows.h>

static void* s_GxDevWindow;

void* OsGuiGetWindow(int32_t type) {
    switch (type) {
    case 0:
        return s_GxDevWindow;
    case 1:
        return GetActiveWindow();
    case 2:
        return GetForegroundWindow();
    default:
        return nullptr;
    }
}

bool OsGuiIsModifierKeyDown(int32_t key) {
    // TODO
    return false;
}

int32_t OsGuiProcessMessage(void* message) {
    // TODO
    return 0;
}

void OsGuiSetGxWindow(void* window) {
    s_GxDevWindow = window;
}
