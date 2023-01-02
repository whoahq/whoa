#ifndef UI_FRAME_SCRIPT_HPP
#define UI_FRAME_SCRIPT_HPP

#include "ui/Types.hpp"
#include <cmath>
#include <cstdint>
#include <common/MD5.hpp>
#include <storm/Array.hpp>
#include <storm/Hash.hpp>
#include <storm/List.hpp>

#define NUM_GLUE_SCRIPT_EVENTS 41

class CImVector;
class CSimpleFrame;
class CStatus;
class FrameScript_Object;
struct lua_State;

struct SIMPLEFRAMENODE : TSLinkedNode<SIMPLEFRAMENODE> {
    CSimpleFrame *frame;
};

struct EVENTLISTENERNODE : TSLinkedNode<EVENTLISTENERNODE> {
    FrameScript_Object *listener;
};

class FrameScript_EventObject : public TSHashObject<FrameScript_EventObject, HASHKEY_STRI> {
    public:
        // Member variables
        TSList<EVENTLISTENERNODE, TSGetLink<EVENTLISTENERNODE>> listeners;
        TSList<EVENTLISTENERNODE, TSGetLink<EVENTLISTENERNODE>> unregisterListeners;
        TSList<EVENTLISTENERNODE, TSGetLink<EVENTLISTENERNODE>> registerListeners;
        uint32_t signalCount = 0;
        uint32_t pendingSignalCount = 0;
        // TODO performance counters

        // Member functions
        FrameScript_EventObject() : TSHashObject<FrameScript_EventObject, HASHKEY_STRI>() {};
        const char* GetName(void);
};

extern const char* g_glueScriptEvents[41];
extern const char* g_scriptEvents[722];

namespace FrameScript {
    extern void* s_mempool;
    extern int32_t s_blocksizes[9];
    extern lua_State* s_context;
    extern int64_t s_scriptTimeUsed;
    extern int32_t s_scriptProfileEnabled;
    extern int32_t s_errorHandlerFun;
    extern double_t s_scriptTimeDivisor;
    extern int32_t s_errorHandlerRef;
    extern int32_t s_recursiveTableHash;
    extern char const* s_compat_lua;
    extern int32_t s_pluralRule;
    extern int32_t s_handlingError;
    extern TSFixedArray<FrameScript_EventObject*> s_scriptEvents;
    extern TSHashTable<FrameScript_EventObject, HASHKEY_STRI> s_scriptEventsHash;
}

int32_t FrameScript_CompileFunction(const char*, const char*, const char*, CStatus*);

void FrameScript_CreateEvents(const char*[], uint32_t);

void FrameScript_Destroy(void);

void FrameScript_Execute(const char*, const char*, const char*);

void FrameScript_Execute(int32_t, FrameScript_Object*, int32_t, const char*, FrameScript_EventObject*);

int32_t FrameScript_ExecuteBuffer(const char*, size_t, const char*, CStatus*, const char*);

int32_t FrameScript_ExecuteFile(const char*, const char*, MD5_CTX*, CStatus*);

void FrameScript_Flush(void);

void FrameScript_GetColor(lua_State* L, int32_t idx, CImVector& color);

lua_State* FrameScript_GetContext(void);

const char* FrameScript_GetCurrentObject(lua_State*, int32_t);

char const* FrameScript_GetText(const char*, int32_t, FRAMESCRIPT_GENDER);

int32_t FrameScript_GetVariable(const char*, const char**);

int32_t FrameScript_HandleError(lua_State*);

int32_t FrameScript_Initialize(int32_t);

void FrameScript_RegisterFunction(const char*, int32_t (*)(lua_State*));

void FrameScript_RegisterScriptEvent(FrameScript_Object*, FrameScript_EventObject*);

void FrameScript_SetPluralRule(PLURAL_RULE);

void FrameScript_SignalEvent(uint32_t, const char*, ...);

void FrameScript_UnregisterScriptEvent(FrameScript_Object*, FrameScript_EventObject*);

void GlueScriptEventsInitialize(void);

void ScriptEventsInitialize(void);

#endif
