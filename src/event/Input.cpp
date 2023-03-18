#include "event/Input.hpp"
#include "event/Event.hpp"
#include "event/EvtContext.hpp"
#include "event/Queue.hpp"
#include "gx/Window.hpp"
#include <common/Time.hpp>
#include <storm/String.hpp>
#include <storm/Unicode.hpp>
#include <tempest/Rect.hpp>
#include <tempest/Vector.hpp>

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

#if defined(WHOA_SYSTEM_MAC)
    #include "app/mac/MacClient.h"
#endif

namespace Input {
    CRect s_boundingRect;
    OSEVENT s_queue[32];

    MOUSEBUTTON s_buttonConversion[16] = {
        MOUSE_BUTTON_NONE,
        MOUSE_BUTTON_LEFT,
        MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_MIDDLE,
        MOUSE_BUTTON_XBUTTON1,
        MOUSE_BUTTON_XBUTTON2,
        MOUSE_BUTTON_XBUTTON3,
        MOUSE_BUTTON_XBUTTON4,
        MOUSE_BUTTON_XBUTTON5,
        MOUSE_BUTTON_XBUTTON6,
        MOUSE_BUTTON_XBUTTON7,
        MOUSE_BUTTON_XBUTTON8,
        MOUSE_BUTTON_XBUTTON9,
        MOUSE_BUTTON_XBUTTON10,
        MOUSE_BUTTON_XBUTTON11,
        MOUSE_BUTTON_XBUTTON12,
    };
}

int32_t Input::s_buttonDown[16];
uint32_t Input::s_buttonState;
C2iVector Input::s_currentMouse;
uint32_t Input::s_mouseHoldButton;
MOUSEMODE Input::s_mouseMode;
int32_t Input::s_numlockState;
uint32_t Input::s_osButtonState;
OS_MOUSE_MODE Input::s_osMouseMode;
int32_t Input::s_simulatedRightButtonClick;
uint32_t Input::s_metaKeyState;
int32_t Input::s_queueHead;
int32_t Input::s_queueTail;
int32_t Input::s_windowFocused;

#if defined(WHOA_SYSTEM_WIN)
    int32_t Input::s_savedMouseSpeed;
#endif

#if defined(WHOA_SYSTEM_MAC)
    double Input::s_savedMouseSpeed;
#endif

void PostChar(EvtContext* context, int32_t ch, int32_t repeat) {
    EVENT_DATA_CHAR data;

    data.ch = ch;
    data.metaKeyState = Input::s_metaKeyState;
    data.repeat = repeat;

    IEvtQueueDispatch(context, EVENT_ID_CHAR, &data);
}

void PostClose() {
    EventInitiateShutdown();
}

void PostFocus(EvtContext* context, int32_t focus) {
    Input::s_buttonState = 0;
    Input::s_metaKeyState = 0;

    EVENT_DATA_FOCUS data;
    data.focus = focus;

    IEvtQueueDispatch(context, EVENT_ID_FOCUS, &data);

    CheckMouseModeState();
}

void PostKeyDown(EvtContext* context, int32_t key, int32_t repeat, int32_t time) {
    if (key <= KEY_LASTMETAKEY) {
        if ((1 << key) & Input::s_metaKeyState) {
            return;
        }

        Input::s_metaKeyState |= 1 << key;
    }

    EVENT_DATA_KEY data;

    data.key = static_cast<KEY>(key);
    data.metaKeyState = Input::s_metaKeyState;
    data.repeat = repeat;
    data.time = time;

    IEvtQueueDispatch(context, EVENT_ID_KEYDOWN, &data);
}

void PostKeyUp(EvtContext* context, int32_t key, int32_t repeat, int32_t time) {
    if (key <= KEY_LASTMETAKEY) {
        if ( !((1 << key) & Input::s_metaKeyState) ) {
            return;
        }

        Input::s_metaKeyState &= ~(1 << key);
    }

    EVENT_DATA_KEY data;

    data.key = static_cast<KEY>(key);
    data.metaKeyState = Input::s_metaKeyState;
    data.repeat = repeat;
    data.time = time;

    IEvtQueueDispatch(context, EVENT_ID_KEYUP, &data);
}

void PostMouseDown(EvtContext* context, MOUSEBUTTON button, int32_t x, int32_t y, int32_t time) {
    Input::s_buttonState |= button;

    EVENT_DATA_MOUSE data;

    data.mode = Input::s_mouseMode;
    data.button = button;
    data.buttonState = Input::s_buttonState;
    data.metaKeyState = Input::s_metaKeyState;
    data.flags = GenerateMouseFlags();
    data.time = time;

    ConvertPosition(x, y, &data.x, &data.y);

    IEvtQueueDispatch(context, EVENT_ID_MOUSEDOWN, &data);
}

void PostMouseModeChanged(EvtContext* context, MOUSEMODE mode) {
    // TODO
}

void PostMouseMove(EvtContext* context, int32_t x, int32_t y, int32_t time) {
    EVENT_DATA_MOUSE data;

    data.mode = Input::s_mouseMode;
    data.button = MOUSE_BUTTON_NONE;
    data.buttonState = Input::s_buttonState;
    data.metaKeyState = Input::s_metaKeyState;
    data.flags = GenerateMouseFlags();
    data.time = time;

    ConvertPosition(x, y, &data.x, &data.y);

    IEvtQueueDispatch(context, EVENT_ID_MOUSEMOVE, &data);
}

void PostMouseUp(EvtContext* context, MOUSEBUTTON button, int32_t x, int32_t y, uint32_t flags, int32_t time) {
    Input::s_buttonState &= ~button;

    EVENT_DATA_MOUSE data;

    data.mode = Input::s_mouseMode;
    data.button = button;
    data.buttonState = Input::s_buttonState;
    data.metaKeyState = Input::s_metaKeyState;
    data.flags = flags | GenerateMouseFlags();
    data.time = time;

    ConvertPosition(x, y, &data.x, &data.y);

    IEvtQueueDispatch(context, EVENT_ID_MOUSEUP, &data);

    CheckMouseModeState();
}

void PostSize(EvtContext* context, int32_t w, int32_t h) {
    EVENT_DATA_SIZE data;

    data.w = w;
    data.h = h;

    IEvtQueueDispatch(context, EVENT_ID_SIZE, &data);
}

void ProcessInput(const int32_t param[], OSINPUT id, int32_t* shutdown, EvtContext* context) {
    STORM_ASSERT(context);
    STORM_VALIDATE(context, ERROR_INVALID_PARAMETER);

    switch (id) {
        case OS_INPUT_CAPTURE_CHANGED:
            // TODO
            break;

        case OS_INPUT_CHAR:
            PostChar(
                context,
                param[0],
                param[1]
            );

            break;

        case OS_INPUT_STRING:
            // TODO
            break;

        case OS_INPUT_IME:
            // TODO
            break;

        case OS_INPUT_SIZE:
            PostSize(
                context,
                param[0],
                param[1]
            );

            break;

        case OS_INPUT_CLOSE:
            // TODO ConfirmClose()
            PostClose();
            *shutdown = 1;

            break;

        case OS_INPUT_FOCUS:
            PostFocus(
                context,
                param[0]
            );

            break;

        case OS_INPUT_KEY_DOWN:
            PostKeyDown(
                context,
                param[0],
                param[1],
                param[3]
            );

            break;

        case OS_INPUT_KEY_UP:
            PostKeyUp(
                context,
                param[0],
                param[1],
                param[3]
            );

            break;

        case OS_INPUT_MOUSE_DOWN:
            PostMouseDown(
                context,
                static_cast<MOUSEBUTTON>(param[0]),
                param[1],
                param[2],
                param[3]
            );

            break;

        case OS_INPUT_MOUSE_MOVE:
            PostMouseMove(
                context,
                param[1],
                param[2],
                param[3]
            );

            break;

        case OS_INPUT_MOUSE_WHEEL:
            // TODO
            break;

        case OS_INPUT_MOUSE_MOVE_RELATIVE:
            // TODO
            break;

        case OS_INPUT_MOUSE_UP:
            PostMouseUp(
                context,
                static_cast<MOUSEBUTTON>(param[0]),
                param[1],
                param[2],
                0,
                param[3]
            );

            break;

        case OS_INPUT_14:
            // TODO
            break;

        case OS_INPUT_15:
            // TODO
            break;

        case OS_INPUT_16:
            // TODO
            break;

        case OS_INPUT_17:
            // TODO
            break;

        case OS_INPUT_18:
            // TODO
            break;

        case OS_INPUT_SHUTDOWN:
            // TODO
            break;
    }
}

void CheckMouseModeState() {
    if (Input::s_mouseHoldButton) {
        if (Input::s_mouseHoldButton != (Input::s_mouseHoldButton & Input::s_buttonState)) {
            EventSetMouseMode(MOUSE_MODE_NORMAL, 0);
        }
    }
}

MOUSEBUTTON ConvertButtonNumberToMOUSEBUTTON(int32_t buttonNumber) {
    return Input::s_buttonConversion[buttonNumber];
}

void ConvertPosition(int32_t clientx, int32_t clienty, float* x, float* y) {
    if (Input::s_boundingRect.maxX - Input::s_boundingRect.minX != 0.0 && Input::s_boundingRect.maxY - Input::s_boundingRect.minY != 0.0) {
        C2Vector pt = {
            static_cast<float>(clientx),
            static_cast<float>(clienty)
        };

        if (!Input::s_boundingRect.IsPointInside(pt)) {
            // TODO
            // - handle out of bounds positions
        }
    }

    RECT windowDim;
    OsGetDefaultWindowRect(&windowDim);

    *x = static_cast<float>(clientx) / static_cast<float>(windowDim.right - windowDim.left);
    *y = 1.0 - (static_cast<float>(clienty) / static_cast<float>(windowDim.bottom - windowDim.top));
}

void EventSetMouseMode(MOUSEMODE mode, uint32_t holdButton) {
    STORM_ASSERT(mode < MOUSE_MODES);
    STORM_VALIDATE(mode < MOUSE_MODES, ERROR_INVALID_PARAMETER);

    auto contextId = *reinterpret_cast<uint32_t*>(PropGet(PROP_EVENTCONTEXT));
    int32_t findMask;
    auto context = TSingletonInstanceId<EvtContext, offsetof(EvtContext, m_id)>::s_idTable.Ptr(
        contextId,
        0,
        &findMask
    );

    if (context) {
        IEvtInputSetMouseMode(context, mode, holdButton);

        if (findMask != -1) {
            TSingletonInstanceId<EvtContext, offsetof(EvtContext, m_id)>::s_idTable.Unlock(
                findMask & (INSTANCE_TABLE_SLOT_COUNT - 1),
                findMask >= INSTANCE_TABLE_SLOT_COUNT
            );
        }
    }
}

uint32_t GenerateMouseFlags() {
    uint32_t flags = 0;

    if (Input::s_mouseMode == MOUSE_MODE_RELATIVE) {
        flags |= 0x2;
    }

    return flags;
}

const char* GetButtonName(int32_t button) {
    switch (button) {
        case 0x1:
            return "LeftButton";
        case 0x2:
            return "MiddleButton";
        case 0x4:
            return "RightButton";
        case 0x8:
            return "Button4";
        case 0x10:
            return "Button5";
        case 0x20:
            return "Button6";
        case 0x40:
            return "Button7";
        case 0x80:
            return "Button8";
        case 0x100:
            return "Button9";
        case 0x200:
            return "Button10";
        case 0x400:
            return "Button11";
        case 0x800:
            return "Button12";
        case 0x1000:
            return "Button13";
        case 0x2000:
            return "Button14";
        case 0x4000:
            return "Button15";
        case 0x8000:
            return "Button16";
        case 0x10000:
            return "Button17";
        case 0x20000:
            return "Button18";
        case 0x40000:
            return "Button19";
        case 0x80000:
            return "Button20";
        case 0x100000:
            return "Button21";
        case 0x200000:
            return "Button22";
        case 0x400000:
            return "Button23";
        case 0x800000:
            return "Button24";
        case 0x1000000:
            return "Button25";
        case 0x2000000:
            return "Button26";
        case 0x4000000:
            return "Button27";
        case 0x8000000:
            return "Button28";
        case 0x10000000:
            return "Button29";
        case 0x20000000:
            return "Button30";
        case 0x40000000:
            return "Button31";
        default:
            return "UNKNOWN";
    }
}

void IEvtInputInitialize() {
    OsInputInitialize();
}

int32_t IEvtInputProcess(EvtContext* context, int32_t* shutdown) {
    STORM_ASSERT(context);
    STORM_VALIDATE(context, ERROR_INVALID_PARAMETER, 0);

    int32_t v4 = 0;
    OSINPUT id;
    int32_t param[4];

    while (OsInputGet(&id, &param[0], &param[1], &param[2], &param[3])) {
        v4 = 1;
        ProcessInput(param, id, shutdown, context);
    }

    return v4;
}

void IEvtInputSetMouseMode(EvtContext* context, MOUSEMODE mode, uint32_t holdButton) {
    STORM_ASSERT(context);
    STORM_VALIDATE(context, ERROR_INVALID_PARAMETER);

    if ((Input::s_buttonState & holdButton) == holdButton) {
        Input::s_mouseHoldButton = holdButton;

        if (Input::s_mouseMode != mode) {
            OS_MOUSE_MODE osMouseMode = mode == MOUSE_MODE_NORMAL ? OS_MOUSE_MODE_NORMAL : OS_MOUSE_MODE_RELATIVE;
            OsInputSetMouseMode(osMouseMode);

            PostMouseModeChanged(context, mode);
        }
    }
}

const char* KeyCodeToString(KEY key) {
    static char charBuf[8];

    if (key - 33 <= 222) {
        SUniSPutUTF8(key, charBuf);
        return charBuf;
    }

    if (key <= KEY_SPACE) {
        switch (key) {
            case KEY_NONE:
                return "NONE";
            case KEY_LSHIFT:
                return "LSHIFT";
            case KEY_RSHIFT:
                return "RSHIFT";
            case KEY_LCONTROL:
                return "LCTRL";
            case KEY_RCONTROL:
                return "RCTRL";
            case KEY_LALT:
                return "LALT";
            case KEY_RALT:
                return "RALT";
            case KEY_SPACE:
                return "SPACE";
            default:
                return "UNKNOWN";
        }
    }

    if (key <= KEY_ESCAPE) {
        switch (key) {
            case KEY_NUMPAD0:
            case KEY_NUMPAD1:
            case KEY_NUMPAD2:
            case KEY_NUMPAD3:
            case KEY_NUMPAD4:
            case KEY_NUMPAD5:
            case KEY_NUMPAD6:
            case KEY_NUMPAD7:
            case KEY_NUMPAD8:
            case KEY_NUMPAD9:
                SStrPrintf(charBuf, sizeof(charBuf), "NUMPAD%d", key);
                return charBuf;
            case KEY_NUMPAD_PLUS:
                return "NUMPADPLUS";
            case KEY_NUMPAD_MINUS:
                return "NUMPADMINUS";
            case KEY_NUMPAD_MULTIPLY:
                return "NUMPADMULTIPLY";
            case KEY_NUMPAD_DIVIDE:
                return "NUMPADDIVIDE";
            case KEY_NUMPAD_DECIMAL:
                return "NUMPADDECIMAL";
            case KEY_ESCAPE:
                return "ESCAPE";
            default:
                return "UNKNOWN";
        }
    }

    if (key <= KEY_PRINTSCREEN) {
        switch (key) {
            case KEY_ENTER:
                return "ENTER";
            case KEY_BACKSPACE:
                return "BACKSPACE";
            case KEY_TAB:
                return "TAB";
            case KEY_LEFT:
                return "LEFT";
            case KEY_UP:
                return "UP";
            case KEY_RIGHT:
                return "RIGHT";
            case KEY_DOWN:
                return "DOWN";
            case KEY_INSERT:
                return "INSERT";
            case KEY_DELETE:
                return "DELETE";
            case KEY_HOME:
                return "HOME";
            case KEY_END:
                return "END";
            case KEY_PAGEUP:
                return "PAGEUP";
            case KEY_PAGEDOWN:
                return "PAGEDOWN";
            case KEY_CAPSLOCK:
                return "CAPSLOCK";
            case KEY_NUMLOCK:
                return "NUMLOCK";
            case KEY_PRINTSCREEN:
                return "PRINTSCREEN";
            default:
                return "UNKNOWN";
        }
    }

    if (key <= KEY_F12) {
        switch (key) {
            case KEY_F1:
            case KEY_F2:
            case KEY_F3:
            case KEY_F4:
            case KEY_F5:
            case KEY_F6:
            case KEY_F7:
            case KEY_F8:
            case KEY_F9:
            case KEY_F10:
            case KEY_F11:
            case KEY_F12:
                SStrPrintf(charBuf, sizeof(charBuf), "F%d", key - KEY_F1 + 1);
                return charBuf;
            default:
                return "UNKNOWN";
        }
    }

    if (key == KEY_NUMPAD_EQUALS) {
        return "NUMPADEQUALS";
    }

    return "UNKNOWN";
}

void OsInputInitialize() {
    #if defined(WHOA_SYSTEM_WIN)
        Input::s_numlockState = GetAsyncKeyState(144);
        int32_t mouseSpeed = 10;
        SystemParametersInfoA(SPI_GETMOUSESPEED, 0, &mouseSpeed, 0);
        Input::s_savedMouseSpeed = mouseSpeed;
    #endif

    #if defined(WHOA_SYSTEM_MAC)
        // Legacy Carbon input handling
        // if (!byte_143EFE0) {
        //     Carbon_OsInputRegisterHICommandHandler(0x71756974, sub_A4F230);
        // }

        MacClient::SetMouseCoalescingEnabled(true);
        Input::s_savedMouseSpeed = MacClient::GetMouseSpeed();
    #endif
}

bool OsInputIsUsingCocoaEventLoop() {
    // TODO

    return true;
}

void OsInputPostEvent(OSINPUT id, int32_t param0, int32_t param1, int32_t param2, int32_t param3) {
    // TODO
}

int32_t OsQueueGet(OSINPUT* id, int32_t* param0, int32_t* param1, int32_t* param2, int32_t* param3) {
    if (Input::s_queueTail == Input::s_queueHead) {
        return 0;
    }

    OSEVENT event = Input::s_queue[Input::s_queueTail];

    *id = event.id;
    *param0 = event.param[0];
    *param1 = event.param[1];
    *param2 = event.param[2];
    *param3 = event.param[3];

    if (Input::s_queueTail == OS_QUEUE_SIZE - 1) {
        Input:: s_queueTail = 0;
    } else {
        ++Input::s_queueTail;
    }

    return 1;
}

void OsQueuePut(OSINPUT id, int32_t param0, int32_t param1, int32_t param2, int32_t param3) {
    int32_t nextTail = 0;
    int32_t nextHead = 0;

    if (Input::s_queueHead != OS_QUEUE_SIZE - 1) {
        nextHead = Input::s_queueHead + 1;
    }

    if (nextHead == Input::s_queueTail) {
        if (nextHead != OS_QUEUE_SIZE - 1) {
            nextTail = nextHead + 1;
        }

        Input::s_queueTail = nextTail;
    }

    OSEVENT* event = &Input::s_queue[Input::s_queueHead];

    event->id = id;
    event->param[0] = param0;
    event->param[1] = param1;
    event->param[2] = param2;
    event->param[3] = param3;

    Input::s_queueHead = nextHead;
}

void OsQueueSetParam(int32_t index, int32_t param) {
    int32_t pos = Input::s_queueTail;

    while (pos != Input::s_queueHead) {
        OSEVENT* event = &Input::s_queue[pos];
        event->param[index] = param;

        if (pos == OS_QUEUE_SIZE - 1) {
            pos = 0;
        } else {
            ++pos;
        }
    }
}
