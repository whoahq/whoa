#include "console/Handlers.hpp"
#include "event/Event.hpp"
#include <cstdint>

int32_t OnChar(const EVENT_DATA_CHAR* data, void* a2) {
    // TODO
    return 1;
}

int32_t OnIdle(EVENT_DATA_IDLE* data, void* a2) {
    // TODO
    return 1;
}

int32_t OnKeyDown(EVENT_DATA_KEY* data, void* a2) {
    // TODO
    return 1;
}

int32_t OnKeyDownRepeat(EVENT_DATA_KEY* data, void* a2) {
    // TODO
    return 1;
}

int32_t OnKeyUp(EVENT_DATA_KEY* data, void* a2) {
    // TODO
    return 1;
}

int32_t OnMouseDown(EVENT_DATA_MOUSE* data, void* a2) {
    // TODO
    return 1;
}

int32_t OnMouseMove(EVENT_DATA_MOUSE* data, void* a2) {
    // TODO
    return 1;
}

int32_t OnMouseUp(EVENT_DATA_MOUSE* data, void* a2) {
    // TODO
    return 1;
}

void RegisterHandlers() {
    EventRegisterEx(EVENT_ID_CHAR, reinterpret_cast<EVENTHANDLERFUNC>(OnChar), nullptr, 7.0f);
    EventRegisterEx(EVENT_ID_IDLE, reinterpret_cast<EVENTHANDLERFUNC>(OnIdle), nullptr, 7.0f);
    EventRegisterEx(EVENT_ID_KEYDOWN, reinterpret_cast<EVENTHANDLERFUNC>(OnKeyDown), nullptr, 7.0f);
    EventRegisterEx(EVENT_ID_KEYUP, reinterpret_cast<EVENTHANDLERFUNC>(OnKeyUp), nullptr, 7.0f);
    EventRegisterEx(EVENT_ID_KEYDOWN_REPEATING, reinterpret_cast<EVENTHANDLERFUNC>(OnKeyDownRepeat), nullptr, 7.0f);
    EventRegisterEx(EVENT_ID_MOUSEDOWN, reinterpret_cast<EVENTHANDLERFUNC>(OnMouseDown), nullptr, 7.0f);
    EventRegisterEx(EVENT_ID_MOUSEUP, reinterpret_cast<EVENTHANDLERFUNC>(OnMouseUp), nullptr, 7.0f);
    EventRegisterEx(EVENT_ID_MOUSEMOVE, reinterpret_cast<EVENTHANDLERFUNC>(OnMouseMove), nullptr, 7.0f);
}
