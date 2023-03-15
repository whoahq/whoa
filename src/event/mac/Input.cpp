#include "event/Input.hpp"

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
