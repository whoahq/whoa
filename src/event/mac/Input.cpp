#include "event/Input.hpp"
#include <common/Time.hpp>

bool OsGuiIsModifierKeyDown(int32_t key) {
    // TODO
    return false;
}

int32_t OsGuiProcessMessage(void* message) {
    return 0;
}

int32_t OsInputGet(OSINPUT* id, int32_t* param0, int32_t* param1, int32_t* param2, int32_t* param3) {
    // TODO
    // Unknown logic

    if (!OsInputIsUsingCocoaEventLoop()) {
        // Legacy Carbon input handling
        return 0;
    }

    // TODO
    // Steelseries WoW Mouse logic

    if (Input::s_queueTail == Input::s_queueHead) {
        return 0;
    }

    OsQueueSetParam(3, OsGetAsyncTimeMs());

    return OsQueueGet(id, param0, param1, param2, param3);
}

void OsInputSetMouseMode(OS_MOUSE_MODE mode) {
    // TODO
}

int32_t OsWindowProc(void* window, uint32_t message, uintptr_t wparam, intptr_t lparam) {
    return 0;
}
