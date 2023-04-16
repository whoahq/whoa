#include "console/Handlers.hpp"
#include "console/Console.hpp"
#include "console/Screen.hpp"
#include "event/Event.hpp"
#include "gx/Screen.hpp"
#include <cstdint>

namespace {

int32_t OnChar(const EVENT_DATA_CHAR* data, void* param) {
    char character[2];

    if (!EventIsKeyDown(ConsoleGetHotKey()) || !ConsoleAccessGetEnabled()) {
        if (!ConsoleGetActive()) {
            return 1;
        } else {
            character[0] = data->ch;
            character[1] = '\0';

            // TODO
            // PasteInInputLine(character);
            // ResetHighlight();
        }
    }

    return 0;
}

int32_t OnIdle(const EVENT_DATA_IDLE* data, void* param) {
    double currentPos;
    double direction;
    float finalPos;
    char* repeatBuffer;
    uint32_t repeatCount;
    RECTF* rect;

    rect = ConsoleScreenGetRect();

    if (ConsoleGetActive()) {
        finalPos = 1.0 - ConsoleGetHeight();

        if (finalPos > 1.0) {
            finalPos = 1.0;
            goto LABEL_7;
        }
    }
    else {
        finalPos = 1.0;
    }
    if (finalPos <= 0.0) {
        finalPos = 0.0;
    }
LABEL_7:
    repeatCount = ConsoleGetRepeatCount();
    repeatBuffer = ConsoleGetRepeatBuffer();

    if (repeatCount && repeatBuffer[0]) {
        // TODO
        // ConsoleCommandExecute(repeatBuffer, 1);
        ConsoleSetRepeatCount(--repeatCount);
    }
    else if (rect->bottom != finalPos) {
        if (ConsoleGetResizeState() == CS_NONE) {
            rect->bottom = finalPos;
LABEL_22:
            ScrnLayerSetRect(ConsoleScreenGetBackgroundLayer(), rect);
            ScrnLayerSetRect(ConsoleScreenGetTextLayer(), rect);

            return 1;
        }
        if (rect->bottom <= finalPos) {
            direction = 1.0;
        }
        else {
            direction = -1.0;
        }

        currentPos = direction * data->elapsedSec * 5.0 + rect->bottom;

        if (ConsoleGetActive()) {
            if ( finalPos > currentPos ) {
                rect->bottom = finalPos;

                goto LABEL_22;
            }
        }
        else if (finalPos < currentPos) {
            rect->bottom = finalPos;

            goto LABEL_22;
        }

        rect->bottom = currentPos;
        
        goto LABEL_22;
    }
    
    return 1;
}

int32_t OnKeyDown(const EVENT_DATA_KEY* data, void* param) {
    bool active;
    
    if (data->key == ConsoleGetHotKey() && ConsoleAccessGetEnabled()) {
        active = ConsoleGetActive() == false;

        ConsoleSetActive(active);

        if (!active) {
            // TODO
            // ResetHighlight();
        }

        return 0;
    }
    else if (EventIsKeyDown(ConsoleGetHotKey()) || !ConsoleGetActive()) {
        return 1;
    }
    else {
        // TODO

        return 0;
    }
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
