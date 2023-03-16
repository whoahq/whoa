#ifndef EVENT_INPUT_HPP
#define EVENT_INPUT_HPP

#include "event/Types.hpp"
#include <cstdint>

#define OS_QUEUE_SIZE 32

class C2iVector;
class CRect;
class EvtContext;

namespace Input {
    extern int32_t s_buttonDown[16];
    extern uint32_t s_buttonState;
    extern C2iVector s_currentMouse;
    extern uint32_t s_mouseHoldButton;
    extern MOUSEMODE s_mouseMode;
    extern int32_t s_numlockState;
    extern int32_t s_simulatedRightButtonClick;
    extern uint32_t s_metaKeyState;
    extern int32_t s_queueHead;
    extern int32_t s_queueTail;
    extern int32_t s_windowFocused;

    #if defined(WHOA_SYSTEM_WIN)
        extern int32_t s_savedMouseSpeed;
    #endif

    #if defined(WHOA_SYSTEM_MAC)
        extern double s_savedMouseSpeed;
    #endif
}

void CheckMouseModeState();

MOUSEBUTTON ConvertButtonNumberToMOUSEBUTTON(int32_t buttonNumber);

void ConvertPosition(int32_t clientx, int32_t clienty, float* x, float* y);

uint32_t GenerateMouseFlags();

const char* GetButtonName(int32_t button);

void IEvtInputInitialize();

int32_t IEvtInputProcess(EvtContext* context, int32_t* shutdown);

const char* KeyCodeToString(KEY key);

int32_t OsInputGet(OSINPUT* id, int32_t* param0, int32_t* param1, int32_t* param2, int32_t* param3);

void OsInputInitialize();

bool OsInputIsUsingCocoaEventLoop();

void OsInputPostEvent(OSINPUT id, int32_t param0, int32_t param1, int32_t param2, int32_t param3);

int32_t OsQueueGet(OSINPUT* id, int32_t* param0, int32_t* param1, int32_t* param2, int32_t* param3);

void OsQueuePut(OSINPUT id, int32_t param0, int32_t param1, int32_t param2, int32_t param3);

void OsQueueSetParam(int32_t index, int32_t param);

#endif
