#include "event/Input.hpp"
#include "event/Window.hpp"
#include <windows.h>

int32_t OsInputGet(OSINPUT* id, int32_t* param0, int32_t* param1, int32_t* param2, int32_t* param3) {
    // TODO window rect comparisons

    while (Input::s_queueTail == Input::s_queueHead) {
        MSG msg;

        if (!PeekMessage(&msg, nullptr, 0, 0, 0)) {
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
    }

    OsQueueGet(id, param0, param1, param2, param3);

    return 1;
}
