#include "client/gui/OsGui.hpp"
#include <emscripten/html5.h>

void* OsGuiGetWindow(int32_t type) {
    // No native window handle on web
    return nullptr;
}

bool OsGuiIsModifierKeyDown(int32_t key) {
    EmscriptenKeyboardEvent state;

    // Get current keyboard state from Emscripten
    // Note: This only works if a key event was recently processed
    switch (key) {
        case 0: // KEY_LSHIFT
        case 1: // KEY_RSHIFT
            return false; // Would need to track state manually
        case 2: // KEY_LCONTROL
        case 3: // KEY_RCONTROL
            return false;
        case 4: // KEY_LALT
        case 5: // KEY_RALT
            return false;
        default:
            return false;
    }
}

int32_t OsGuiProcessMessage(void* message) {
    // Not used on web - events come through Emscripten callbacks
    return 0;
}

void OsGuiSetGxWindow(void* window) {
    // No-op on web
}
