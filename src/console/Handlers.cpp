#include "console/Handlers.hpp"
#include "console/Console.hpp"
#include "console/Screen.hpp"
#include "event/Event.hpp"
#include <cstdint>

namespace {

int32_t OnChar(const EVENT_DATA_CHAR* data, void* param) {
    char character[2];

    if (!EventIsKeyDown(ConsoleGetHotKey()) || !ConsoleAccessGetEnabled()) {
        if (!ConsoleGetActive()) {
            return 1;
        }

        // Set the character value and null-terminate the buffer
        character[0] = data->ch;
        character[1] = '\0';

        // TODO
        // PasteInInputLine(character);
        // ResetHighlight();
    }

    return 0;
}

int32_t OnIdle(const EVENT_DATA_IDLE* data, void* param) {
    // TODO repeat buffer logic

    ConsoleScreenAnimate(data->elapsedSec);

    return 1;
}

int32_t OnKeyDown(const EVENT_DATA_KEY* data, void* param) {
    if (data->key == ConsoleGetHotKey() && ConsoleAccessGetEnabled()) {
        // Toggle the console on/off if the console hotkey is pressed down
        // and the console access is enabled for the client
        ConsoleSetActive(!ConsoleGetActive());

        // Reset the highlight when toggled off
        if (!ConsoleGetActive()) {
            // TODO ResetHighlight();
        }

        return 0;
    }

    if (EventIsKeyDown(ConsoleGetHotKey()) || !ConsoleGetActive()) {
        return 1;
    }

    // TODO
    return 0;
}

int32_t OnKeyDownRepeat(const EVENT_DATA_KEY* data, void* param) {
    // TODO
    return 1;
}

int32_t OnKeyUp(const EVENT_DATA_KEY* data, void* param) {
    // TODO
    return 1;
}

int32_t OnMouseDown(const EVENT_DATA_MOUSE* data, void* param) {
    // TODO
    return 1;
}

int32_t OnMouseMove(const EVENT_DATA_MOUSE* data, void* param) {
    // TODO
    return 1;
}

int32_t OnMouseUp(const EVENT_DATA_MOUSE* data, void* param) {
    // TODO
    return 1;
}

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
