#include "gx/Window.hpp"
#include "gx/Device.hpp"

bool s_forceOnscreen;
Rect s_savedWindowRect;
Rect s_savedWindowZoomedRect;

int32_t OsGetDefaultWindowRect(tagRECT* rect) {
    auto window = g_theGxDevicePtr->DeviceDefWindow();

    rect->left = window.minX;
    rect->top = window.minY;
    rect->right = window.maxX;
    rect->bottom = window.maxY;

    return 1;
}

Rect* GetSavedWindowBounds() {
    return &s_savedWindowRect;
}

Rect* GetSavedZoomedWindowBounds() {
    return &s_savedWindowZoomedRect;
}

void SetSavedWindowBounds(Rect rect) {
    s_forceOnscreen = true;
    s_savedWindowRect = rect;
}

void SetSavedZoomedWindowBounds(Rect rect) {
    s_forceOnscreen = true;
    s_savedWindowZoomedRect = rect;
}
