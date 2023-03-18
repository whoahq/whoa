#include "event/Input.hpp"
#include <storm/Error.hpp>
#include <windows.h>

void CenterMouse() {
    // TODO
}

void RestoreMouse() {
    // TODO
}

void SaveMouse(POINT mousePos, HWND hwnd) {
    // TODO
}

int32_t OsGuiProcessMessage(void* message) {
    // TODO
    return 0;
}

int32_t OsInputGet(OSINPUT* id, int32_t* param0, int32_t* param1, int32_t* param2, int32_t* param3) {
    // TODO window rect comparisons

    if (Input::s_queueTail != Input::s_queueHead) {
        OsQueueGet(id, param0, param1, param2, param3);
        return 1;
    }

    // TODO Sub8714B0(dwordB1C220);

    while (true) {
        MSG msg;
        auto peekResult = PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE);

        if (Input::s_queueTail != Input::s_queueHead) {
            break;
        }

        if (!peekResult) {
            return 0;
        }

        if (!GetMessage(&msg, nullptr, 0, 0)) {
            *id = OS_INPUT_SHUTDOWN;
            break;
        }

        if (OsGuiProcessMessage(&msg)) {
            break;
        }

        if (Input::s_queueTail != Input::s_queueHead) {
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (Input::s_queueTail != Input::s_queueHead) {
            break;
        }
    }

    OsQueueGet(id, param0, param1, param2, param3);
    return 1;
}

void OsInputSetMouseMode(OS_MOUSE_MODE mode) {
    STORM_ASSERT(mode < OS_MOUSE_MODES);
    STORM_VALIDATE(mode < OS_MOUSE_MODES, ERROR_INVALID_PARAMETER);

    if (Input::s_osMouseMode == mode) {
        return;
    }

    if (mode == OS_MOUSE_MODE_NORMAL) {
        Input::s_osMouseMode = mode;
        RestoreMouse();
    } else if (mode == OS_MOUSE_MODE_RELATIVE) {
        Input::s_osMouseMode = mode;
        CenterMouse();
    }
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

    case WM_MOUSEMOVE: {
        // TODO

        if (Input::s_osMouseMode == OS_MOUSE_MODE_RELATIVE) {
            // TODO
        } else {
            POINT mousePos;
            GetCursorPos(&mousePos);
            ScreenToClient(hwnd, &mousePos);
            OsQueuePut(OS_INPUT_MOUSE_MOVE, 0, mousePos.x, mousePos.y, 0);
            SaveMouse(mousePos, hwnd);
        }
    }

    default:
        break;
    }

    // TODO

    return DefWindowProc(static_cast<HWND>(window), message, wparam, lparam);
}
