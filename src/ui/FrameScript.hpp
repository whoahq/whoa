#ifndef UI_FRAME_SCRIPT_HPP
#define UI_FRAME_SCRIPT_HPP

#include "object/Types.hpp"
#include "ui/Types.hpp"
#include <common/MD5.hpp>
#include <storm/Array.hpp>
#include <storm/Hash.hpp>
#include <storm/List.hpp>
#include <cmath>
#include <cstdint>

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
        const char* GetName();
};

extern int32_t g_glueFrameScriptGenders[UNITSEX_LAST];
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

int32_t FrameScript_CompileFunction(const char* name, const char* wrapper, const char* body, CStatus* status);

void FrameScript_CreateEvents(const char* names[], uint32_t count);

void FrameScript_Destroy();

void FrameScript_Execute(const char* source, const char* filename, const char* a3);

void FrameScript_Execute(int32_t function, FrameScript_Object* objectThis, int32_t argCount, const char* a4, FrameScript_EventObject* event);

int32_t FrameScript_ExecuteBuffer(const char* buffer, size_t bufferBytes, const char* bufferName, CStatus* status, const char* a5);

int32_t FrameScript_ExecuteFile(const char* filePath, const char* a2, MD5_CTX* md5, CStatus* status);

void FrameScript_Flush();

void FrameScript_GetColor(lua_State* L, int32_t idx, CImVector& color);

lua_State* FrameScript_GetContext();

const char* FrameScript_GetCurrentObject(lua_State* L, int32_t a2);

char const* FrameScript_GetText(const char* a1, int32_t count, FRAMESCRIPT_GENDER gender);

int32_t FrameScript_GetVariable(const char* a1, const char** a2);

int32_t FrameScript_HandleError(lua_State* L);

int32_t FrameScript_Initialize(int32_t a1);

void FrameScript_RegisterFunction(const char* name, int32_t (*function)(lua_State*));

void FrameScript_RegisterScriptEvent(FrameScript_Object* object, FrameScript_EventObject* event);

void FrameScript_SetPluralRule(PLURAL_RULE rule);

void FrameScript_SignalEvent(uint32_t index, const char* format, ...);

const char* FrameScript_Sprintf(lua_State* L, int32_t idx, char buffer[], uint32_t bufferLen);

void FrameScript_UnregisterScriptEvent(FrameScript_Object* object, FrameScript_EventObject* event);

void ScriptEventsInitialize();

#endif
