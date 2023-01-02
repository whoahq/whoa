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

    #if defined(WHOA_SYSTEM_WIN)
        extern int32_t s_savedMouseSpeed;
    #endif

    #if defined(WHOA_SYSTEM_MAC)
        extern double s_savedMouseSpeed;
    #endif
}

void CheckMouseModeState(void);

MOUSEBUTTON ConvertButtonNumberToMOUSEBUTTON(int32_t);

void ConvertPosition(int32_t, int32_t, float*, float*);

uint32_t GenerateMouseFlags(void);

const char* GetButtonName(int32_t);

void IEvtInputInitialize();

int32_t IEvtInputProcess(EvtContext* context, int32_t* shutdown);

const char* KeyCodeToString(KEY);

int32_t OsInputGet(OSINPUT*, int32_t*, int32_t*, int32_t*, int32_t*);

void OsInputInitialize(void);

bool OsInputIsUsingCocoaEventLoop(void);

void OsInputPostEvent(OSINPUT, int32_t, int32_t, int32_t, int32_t);

int32_t OsQueueGet(OSINPUT*, int32_t*, int32_t*, int32_t*, int32_t*);

void OsQueuePut(OSINPUT, int32_t, int32_t, int32_t, int32_t);

void OsQueueSetParam(int32_t, int32_t);

#endif
