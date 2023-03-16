#include "event/Window.hpp"
#include "event/Input.hpp"
#include <windows.h>

int32_t OsGuiProcessMessage(void* message) {
    // TODO
    return 0;
}

int32_t OsWindowProc(void* window, uint32_t message, uintptr_t wparam, intptr_t lparam) {
    auto hwnd = static_cast<HWND>(window);

    // TODO

    switch (message) {
    // TODO handle remaining message types

    case WM_ACTIVATE: {
        auto isMinimized = IsIconic(hwnd);
        auto isActive = wparam != WA_INACTIVE;
        Input::s_windowFocused = isActive && !isMinimized;

        // TODO capture

        // TODO mouse speed

        OsQueuePut(OS_INPUT_FOCUS, Input::s_windowFocused != 0, 0, 0, 0);

        break;
    }

    case WM_CLOSE: {
        OsQueuePut(OS_INPUT_CLOSE, 0, 0, 0, 0);
        return 0;
    }

    default:
        break;
    }

    // TODO

    return DefWindowProc(static_cast<HWND>(window), message, wparam, lparam);
}
