#include "event/Input.hpp"
#include "client/Gui.hpp"
#include <storm/Error.hpp>
#include <windows.h>

static RECT s_defaultWindowRect;
static int32_t s_savedResize;

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

int32_t ConvertKeyCode(uint32_t vkey, KEY* key) {
    if (vkey >= VK_F1 && vkey <= VK_F12) {
        *key = static_cast<KEY>(KEY_F1 + (vkey - VK_F1));
        return 1;
    }

    if (vkey >= 0x30 && vkey <= 0x39) {
        *key = static_cast<KEY>(KEY_0 + (vkey - 0x30));
        return 1;
    }

    switch (vkey) {
    case VK_BACK: {
        *key = KEY_BACKSPACE;
        return 1;
    }

    case VK_TAB: {
        *key = KEY_TAB;
        return 1;
    }

    case VK_RETURN: {
        *key = KEY_ENTER;
        return 1;
    }

    case VK_PAUSE: {
        *key = KEY_PAUSE;
        return 1;
    }

    case VK_CAPITAL: {
        *key = KEY_CAPSLOCK;
        return 1;
    }

    case VK_ESCAPE: {
        *key = KEY_ESCAPE;
        return 1;
    }

    case VK_SPACE: {
        *key = KEY_SPACE;
        return 1;
    }

    case VK_PRIOR: {
        *key = KEY_PAGEUP;
        return 1;
    }

    case VK_NEXT: {
        *key = KEY_PAGEDOWN;
        return 1;
    }

    case VK_END: {
        *key = KEY_END;
        return 1;
    }

    case VK_HOME: {
        *key = KEY_HOME;
        return 1;
    }

    case VK_LEFT: {
        *key = KEY_LEFT;
        return 1;
    }

    case VK_UP: {
        *key = KEY_UP;
        return 1;
    }

    case VK_RIGHT: {
        *key = KEY_RIGHT;
        return 1;
    }

    case VK_DOWN: {
        *key = KEY_DOWN;
        return 1;
    }

    case VK_SNAPSHOT: {
        *key = KEY_PRINTSCREEN;
        return 1;
    }

    case VK_INSERT: {
        *key = KEY_INSERT;
        return 1;
    }

    case VK_DELETE: {
        *key = KEY_DELETE;
        return 1;
    }

    case VK_NUMPAD0:
    case VK_NUMPAD1:
    case VK_NUMPAD2:
    case VK_NUMPAD3:
    case VK_NUMPAD4:
    case VK_NUMPAD5:
    case VK_NUMPAD6:
    case VK_NUMPAD7:
    case VK_NUMPAD8:
    case VK_NUMPAD9: {
        *key = static_cast<KEY>(KEY_NUMPAD0 + (vkey - VK_NUMPAD0));
        return 1;
    }

    case VK_MULTIPLY: {
        *key = KEY_NUMPAD_MULTIPLY;
        return 1;
    }

    case VK_ADD: {
        *key = KEY_NUMPAD_PLUS;
        return 1;
    }

    case VK_SUBTRACT: {
        *key = KEY_NUMPAD_MINUS;
        return 1;
    }

    case VK_DECIMAL: {
        *key = KEY_NUMPAD_DECIMAL;
        return 1;
    }

    case VK_DIVIDE: {
        *key = KEY_NUMPAD_DIVIDE;
        return 1;
    }

    case VK_NUMLOCK: {
        *key = KEY_NUMLOCK;
        return 1;
    }

    case VK_SCROLL: {
        *key = KEY_SCROLLLOCK;
        return 1;
    }

    case VK_LSHIFT: {
        *key = KEY_LSHIFT;
        return 1;
    }

    case VK_RSHIFT: {
        *key = KEY_RSHIFT;
        return 1;
    }

    case VK_LCONTROL: {
        *key = KEY_LCONTROL;
        return 1;
    }

    case VK_RCONTROL: {
        *key = KEY_RCONTROL;
        return 1;
    }

    case VK_LMENU: {
        *key = KEY_LALT;
        return 1;
    }

    case VK_RMENU: {
        *key = KEY_RALT;
        return 1;
    }

    default: {
        auto character = MapVirtualKey(vkey, MAPVK_VK_TO_CHAR);
        *key = static_cast<KEY>(character);
        if (character && character <= 0xFF) {
            return 1;
        } else {
            return 0;
        }
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

int32_t OsInputGet(OSINPUT* id, int32_t* param0, int32_t* param1, int32_t* param2, int32_t* param3) {
    *id = static_cast<OSINPUT>(-1);

    if (s_savedResize) {
        auto hwnd = static_cast<HWND>(OsGuiGetWindow(0));

        if (!IsIconic(hwnd)) {
            s_savedResize = 0;

            RECT windowRect;
            GetWindowRect(hwnd, &windowRect);

            auto style = GetWindowLong(hwnd, GWL_STYLE);
            RECT clientArea = { 0, 0, 0, 0 };
            AdjustWindowRectEx(&clientArea, style, false, 0);

            auto width = windowRect.right - clientArea.right - (windowRect.left - clientArea.left);
            auto height = windowRect.bottom - clientArea.bottom - (windowRect.top - clientArea.top);

            if (s_defaultWindowRect.right != width || s_defaultWindowRect.bottom != height) {
                s_defaultWindowRect.left = 0;
                s_defaultWindowRect.top = 0;
                s_defaultWindowRect.right = width;
                s_defaultWindowRect.bottom = height;

                *id = OS_INPUT_SIZE;
                *param0 = width;
                *param1 = height;
                *param2 = 0;
                *param3 = 0;

                return 1;
            }
        }
    }

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

    case WM_SIZE:
    case WM_DISPLAYCHANGE: {
        s_savedResize = lparam;
        break;
    }

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

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP: {
        auto keyDown = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;

        if (wparam == VK_SHIFT) {
            // TODO
        } else if (wparam == VK_CONTROL) {
            // TODO
        } else if (wparam == VK_MENU) {
            // TODO
        }

        KEY key;
        if (ConvertKeyCode(wparam, &key)) {
            OsQueuePut(keyDown ? OS_INPUT_KEY_DOWN : OS_INPUT_KEY_UP, key, LOWORD(lparam), 0, 0);

            // Alt + F4
            if (key == KEY_F4 && OsGuiIsModifierKeyDown(2)) {
                break;
            }

            return 0;
        }

        break;
    }

    case WM_CHAR: {
        if (wparam < 32) {
            break;
        }

        uint32_t character = wparam;

        if (wparam >= 128) {
            // TODO
        }

        OsQueuePut(OS_INPUT_CHAR, character, LOWORD(lparam), 0, 0);

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
