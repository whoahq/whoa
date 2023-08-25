#include "console/Handlers.hpp"
#include "console/Line.hpp"
#include "console/Console.hpp"
#include "console/Command.hpp"
#include "console/Screen.hpp"
#include "event/Event.hpp"
#include <cstdint>

static int32_t s_historyIndex = 0;

namespace {

int32_t OnChar(const EVENT_DATA_CHAR* data, void* param) {
    char character[2];

    if (ConsoleAccessGetEnabled() && EventIsKeyDown(ConsoleGetHotKey())) {
        return 0;
    }

    if (ConsoleGetActive()) {
        character[0] = char(data->ch);
        character[1] = 0;

        PasteInInputLine(character);
        ResetHighlight();
        return 0;
    }

    // SUniSPutUTF8(data->ch, character);


    return 1;
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
            ResetHighlight();
        }

        return 0;
    }

    if (EventIsKeyDown(ConsoleGetHotKey()) || !ConsoleGetActive()) {
        return 1;
    }

    auto anyControl = (1 << KEY_LCONTROL) | (1 << KEY_RCONTROL);

    auto line = GetInputLine();

    switch (data->key) {
    case KEY_ESCAPE:
        if (line->inputpos < line->inputstart || line->inputpos == line->inputstart) {
            ConsoleSetActive(0);
        } else {
            line->inputpos = line->inputstart;
            line->chars = line->inputstart;
            line->buffer[line->inputstart] = '\0';
            SetInputString(line->buffer);
        }
        break;
    case KEY_PAGEUP:
        MoveLinePtr(1, data->metaKeyState);
        break;
    case KEY_PAGEDOWN:
        MoveLinePtr(0, data->metaKeyState);
        break;
    case KEY_ENTER:
        if (line->inputstart <= line->inputpos && line->inputpos != line->inputstart) {
            line->inputpos = 0;
            GenerateNodeString(line);
            ConsoleCommandExecute(line->buffer + line->inputstart, 1);
            s_historyIndex = -1;
        }
        break;
    case KEY_HOME:
        break;
    case KEY_END:
        break;
    case KEY_C:
        if (data->metaKeyState & anyControl) {
            CutHighlightToClipboard();
        }
        break;
    case KEY_V:
        if (data->metaKeyState & anyControl) {
            PasteClipboardToHighlight();
        }
        break;
    case KEY_LEFT:
        if (line->inputstart <= line->inputpos && line->inputpos != line->inputstart) {
            line->inputpos--;
        }
        break;
    case KEY_RIGHT:
        if (line->inputpos < line->chars) {
            line->inputpos++;
        }
        break;

    case KEY_BACKSPACE:
        BackspaceLine(line);
        break;
    }

    if (data->key != KEY_TAB && data->key != KEY_LSHIFT && data->key != KEY_RSHIFT && data->key != KEY_LALT && data->key != KEY_RALT && !(data->metaKeyState & anyControl)) {
        // s_completionMode = 0;
        ResetHighlight();
    }

    // TODO
    return 0;
}

int32_t OnKeyDownRepeat(const EVENT_DATA_KEY* data, void* param) {
    if (data->key == ConsoleGetHotKey() && ConsoleAccessGetEnabled()) {
        ConsoleSetActive(!ConsoleGetActive());
        return 0;
    }

    switch (data->key) {
    case KEY_PAGEUP:
        MoveLinePtr(1, data->metaKeyState);
        break;
    case KEY_PAGEDOWN:
        MoveLinePtr(0, data->metaKeyState);
        break;
    case KEY_LEFT:
        if (line->inputstart <= line->inputpos && line->inputpos != line->inputstart) {
            line->inputpos--;
        }
        break;
    case KEY_RIGHT:
        if (line->inputpos < line->chars) {
            line->inputpos++;
        }
        break;
    case KEY_BACKSPACE:
        BackspaceLine(line);
        break;
    }

    if (data->key != KEY_TAB && data->key != KEY_LSHIFT && data->key != KEY_RSHIFT && data->key != KEY_LALT && data->key != KEY_RALT && !(data->metaKeyState & anyControl)) {
        // s_completionMode = 0;
        ResetHighlight();
    }


    return 1;
}

int32_t OnKeyUp(const EVENT_DATA_KEY* data, void* param) {
    // TODO
    return 1;
}

int32_t OnMouseDown(const EVENT_DATA_MOUSE* data, void* param) {
    auto consoleHeight = ConsoleGetHeight();
    auto fontHeight = ConsoleGetFontHeight();

    if (EventIsKeyDown(ConsoleGetHotKey()) || !ConsoleGetActive() || (1.0f - consoleHeight) > data->y) {
        return 1;
    }

    float clickPos = 1.0f - data->y;

    if (clickPos < (std::min(consoleHeight, 1.0f) - (fontHeight * 0.75f)) || clickPos > consoleHeight) {
        ResetHighlight();

        auto line = GetLineAtMousePosition(data->y);

        if (line) {
            SetHighlightCopyText(line->buffer);
            SetHighlightState(HS_HIGHLIGHTING);

            float v7 = 1.0f - (consoleHeight - (fontHeight * 0.75f) - (fontHeight) - ((consoleHeight - clickPos) / fontHeight - 1.0) * fontHeight);

            auto hRect = GetHighlightRect();

            hRect.bottom = v7;
            hRect.top = v7 - fontHeight;

            SetHighlightStart(v7);
            SetHighlightEnd(v7);

            UpdateHighlight();

            return 0;
        }

        ResetHighlightCopyText();
        return 0;
    }

    ResetHighlight();

    ConsoleSetResizeState(CS_STRETCH);

    return 1;
}

int32_t OnMouseMove(const EVENT_DATA_MOUSE* data, void* param) {
    if (EventIsKeyDown(ConsoleGetHotKey()) || !ConsoleGetActive()) {
        return 1;
    }

    if (ConsoleGetResizeState() == CS_STRETCH) {
        auto newHeight = std::max(1.0f - data->y, ConsoleGetFontHeight());
        ConsoleSetHeight(newHeight);
    } else if ((1.0f - ConsoleGetHeight()) > data->y) {
        return 1;
    }

    SetHighlightEnd(data->x);

    if (GetHighlightState() == HS_HIGHLIGHTING) {
        UpdateHighlight();
    }

    return 1;
}

int32_t OnMouseUp(const EVENT_DATA_MOUSE* data, void* param) {
    if (EventIsKeyDown(ConsoleGetHotKey()) || !ConsoleGetActive()) {
        return 1;
    }

    SetHighlightState(HS_ENDHIGHLIGHT);
    ConsoleSetResizeState(CS_NONE);

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
