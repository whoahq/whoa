#include "event/Input.hpp"
#include "event/Window.hpp"
#include <storm/Error.hpp>
#include <windows.h>

void CenterMouse() {
    // TODO
}

void RestoreMouse() {
    // TODO
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
