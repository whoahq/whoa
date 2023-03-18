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

int32_t ConvertButton(uint32_t message, uintptr_t wparam, MOUSEBUTTON* button) {
    switch (message) {
    case WM_NCLBUTTONDOWN:
    case WM_NCLBUTTONUP:
    case WM_NCLBUTTONDBLCLK:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP: {
        *button = MOUSE_BUTTON_LEFT;
        return 1;
    }

    case WM_NCRBUTTONDOWN:
    case WM_NCRBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP: {
        *button = MOUSE_BUTTON_RIGHT;
        return 1;
    }

    case WM_NCMBUTTONDOWN:
    case WM_NCMBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP: {
        *button = MOUSE_BUTTON_MIDDLE;
        return 1;
    }

    case WM_NCXBUTTONDOWN:
    case WM_NCXBUTTONUP:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP: {
        switch (GET_XBUTTON_WPARAM(wparam)) {
        case XBUTTON1: {
            *button = MOUSE_BUTTON_XBUTTON1;
            return 1;
        }
        case XBUTTON2: {
            *button = MOUSE_BUTTON_XBUTTON2;
            return 1;
        }
        default: {
            *button = MOUSE_BUTTON_NONE;
            return 0;
        }
        }
    }

    default: {
        *button = MOUSE_BUTTON_NONE;
        return 0;
    }
    }
}

bool ProcessMouseEvent(MOUSEBUTTON button, uint32_t message, HWND hwnd, OSINPUT id) {
    POINT mousePos;

    if (Input::s_osMouseMode == OS_MOUSE_MODE_RELATIVE) {
        // TODO
    } else {
        GetCursorPos(&mousePos);
        ScreenToClient(hwnd, &mousePos);
    }

    OsQueuePut(id, button, mousePos.x, mousePos.y, 0);

    return message == WM_XBUTTONDOWN
        || message == WM_XBUTTONUP
        || message == WM_NCXBUTTONDOWN
        || message == WM_NCXBUTTONUP;
}

int32_t HandleMouseDown(uint32_t message, uintptr_t wparam, bool* xbutton, HWND hwnd) {
    MOUSEBUTTON button;
    if (!ConvertButton(message, wparam, &button)) {
        return 0;
    }

    if (Input::s_osButtonState == 0) {
        SetCapture(hwnd);
    }

    Input::s_osButtonState |= button;

    auto xb = ProcessMouseEvent(button, message, hwnd, OS_INPUT_MOUSE_DOWN);

    if (xbutton) {
        *xbutton = xb;
    }

    return 1;
}

int32_t HandleMouseUp(uint32_t message, uintptr_t wparam, bool* xbutton, HWND hwnd) {
    MOUSEBUTTON button;
    if (!ConvertButton(message, wparam, &button)) {
        return 0;
    }

    Input::s_osButtonState &= ~button;

    if (Input::s_osButtonState == 0) {
        // TODO
        ReleaseCapture();
        // TODO
    }

    auto xb = ProcessMouseEvent(button, message, hwnd, OS_INPUT_MOUSE_UP);

    if (xbutton) {
        *xbutton = xb;
    }

    return 1;
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

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_XBUTTONDOWN: {
        bool xbutton;
        if (HandleMouseDown(message, wparam, &xbutton, hwnd)) {
            // Normally, a processed button down message should return 0
            // In the case of xbuttons, a processed button down message should return 1
            // See: https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-xbuttondown
            return xbutton ? 1 : 0;
        }

        break;
    }

    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP: {
        if (message == WM_LBUTTONUP) {
            // TODO
        }

        bool xbutton;
        if (HandleMouseUp(message, wparam, &xbutton, hwnd)) {
            // Normally, a processed button down message should return 0
            // In the case of xbuttons, a processed button down message should return 1
            // See: https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-xbuttondown
            return xbutton ? 1 : 0;
        }

        break;
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

        break;
    }

    default:
        break;
    }

    // TODO

    return DefWindowProc(static_cast<HWND>(window), message, wparam, lparam);
}
