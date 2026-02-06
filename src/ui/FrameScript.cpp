#include "ui/FrameScript.hpp"
#include "ui/FrameScriptInternal.hpp"
#include "ui/FrameScript_Object.hpp"
#include "ui/LuaMemory.hpp"
#include "util/CStatus.hpp"
#include "util/Lua.hpp"
#include "util/SFile.hpp"
#include <algorithm>
#include <cstdint>
#include <storm/Array.hpp>
#include <storm/String.hpp>
#include <tempest/Vector.hpp>

int32_t g_glueFrameScriptGenders[] = {
    2, // UNIT_SEX_MALE
    3, // UNIT_SEX_FEMALE
    1, // UNIT_SEX_NONE
};

void* FrameScript::s_mempool;
lua_State* FrameScript::s_context;
int64_t FrameScript::s_scriptTimeUsed;
int32_t FrameScript::s_scriptProfileEnabled;
int32_t FrameScript::s_errorHandlerFun;
double_t FrameScript::s_scriptTimeDivisor;
int32_t FrameScript::s_errorHandlerRef;
int32_t FrameScript::s_recursiveTableHash;
int32_t FrameScript::s_pluralRule;
int32_t FrameScript::s_handlingError;
TSFixedArray<FrameScript_EventObject*> FrameScript::s_scriptEvents;
TSHashTable<FrameScript_EventObject, HASHKEY_STRI> FrameScript::s_scriptEventsHash;

int32_t FrameScript::s_blocksizes[9] = { 0x10, 0x18, 0x20, 0x28, 0x40, 0x50, 0x80, 0xa0, 0x100 };

const char* FrameScript::s_compat_lua = R"(
-------------------------------------------------------------------
-- Table library
local tab = table
foreach = tab.foreach
foreachi = tab.foreachi
getn = tab.getn
tinsert = tab.insert
tremove = tab.remove
sort = tab.sort
wipe = tab.wipe

-------------------------------------------------------------------
-- math library
local math = math
abs = math.abs
acos = function (x) return math.deg(math.acos(x)) end
asin = function (x) return math.deg(math.asin(x)) end
atan = function (x) return math.deg(math.atan(x)) end
atan2 = function (x,y) return math.deg(math.atan2(x,y)) end
ceil = math.ceil
cos = function (x) return math.cos(math.rad(x)) end
deg = math.deg
exp = math.exp
floor = math.floor
frexp = math.frexp
ldexp = math.ldexp
log = math.log
log10 = math.log10
max = math.max
min = math.min
mod = math.fmod
PI = math.pi
--??? pow = math.pow
rad = math.rad
random = math.random
--randomseed = math.randomseed
sin = function (x) return math.sin(math.rad(x)) end
sqrt = math.sqrt
tan = function (x) return math.tan(math.rad(x)) end

-------------------------------------------------------------------
-- string library
local str = string
strbyte = str.byte
strchar = str.char
strfind = str.find
format = str.format
gmatch = str.gmatch
gsub = str.gsub
strlen = str.len
strlower = str.lower
strmatch = str.match
strrep = str.rep
strrev = str.reverse
strsub = str.sub
strupper = str.upper
-------------------------------------------------------------------
-- Add custom string functions to the string table
str.trim = strtrim
str.split = strsplit
str.join = strjoin
str.replace = strreplace)";

const char* FrameScript_EventObject::GetName() {
    return this->m_key.GetString();
}

int64_t OsGetAsyncClocksPerSecond() {
    // TODO

    return 1000.0;
}

int32_t FrameScript_CompileFunction(const char* name, const char* wrapper, const char* body, CStatus* status) {
    lua_State* L = FrameScript::s_context;

    size_t functionLen = SStrLen(wrapper) + SStrLen(body) + 1;

    char* function = static_cast<char*>(alloca(functionLen));

    // Insert the function body into the wrapper
    SStrPrintf(function, functionLen, wrapper, body);

    lua_rawgeti(L, LUA_REGISTRYINDEX, FrameScript::s_errorHandlerRef);

    if (luaL_loadbuffer(L, function, SStrLen(function), name)) {
        if (status) {
            const char* v10 = lua_tostring(L, -1);
            status->Add(STATUS_ERROR, "%s", v10);
        }

        if (lua_pcall(L, 1, 0, 0)) {
            lua_settop(L, -2);
        }

        return -1;
    } else if (lua_pcall(L, 0, 1, -2)) {
        if (status) {
            const char* v13 = lua_tostring(L, -1);
            status->Add(STATUS_ERROR, "%s", v13);
        }

        lua_settop(L, -3);

        return -1;
    } else {
        int32_t luaRef = luaL_ref(L, LUA_REGISTRYINDEX);

        lua_settop(L, -2);

        return luaRef;
    }
}

void FrameScript_CreateEvents(const char* names[], uint32_t count) {
    FrameScript::s_scriptEvents.Clear();
    FrameScript::s_scriptEvents.SetCount(count);

    for (int32_t i = 0; i < count; i++) {
        if (names[i]) {
            auto event = FrameScript::s_scriptEventsHash.New(names[i], 0, 0);
            FrameScript::s_scriptEvents[i] = event;
        }
    }
}

void FrameScript_Destroy() {
    // TODO
}

void FrameScript_Execute(const char* source, const char* filename, const char* a3) {
    /* TODO taint tracking
    v3 = lua_taintexpected++ == -1;
    v8 = *(_DWORD *)lua_tainted;

    if (!v3 && !lua_taintedclosure) {
        *(_DWORD *)lua_tainted = a3;
    }
    */

    lua_State* L = FrameScript::s_context;
    size_t len = SStrLen(source);

    lua_rawgeti(L, LUA_REGISTRYINDEX, FrameScript::s_errorHandlerRef);

    if (!luaL_loadbuffer(L, source, len, filename)) {
        if (lua_pcall(L, 0, 0, -2)) {
            lua_settop(L, -3);
        } else {
            lua_settop(L, -2);
        }
    } else if (lua_pcall(L, 1, 0, 0)) {
        lua_settop(L, -2);
    }

    /* TODO taint tracking
    if (lua_taintexpected && !lua_taintedclosure) {
        *(_DWORD *)lua_tainted = v8;
    }

    v7 = lua_taintexpected - 1;
    lua_taintexpected = v7;

    if (v7 <= 0) {
        lua_taintexpected = 0;
    }
    */
}

void FrameScript_Execute(int32_t function, FrameScript_Object* objectThis, int32_t argCount, const char* a4, FrameScript_EventObject* event) {
    lua_State* L = FrameScript::s_context;

    int32_t v20 = 1 - argCount + lua_gettop(L);
    int32_t v19 = argCount;

    lua_checkstack(L, argCount + 2);

    if (objectThis) {
        const char* name = objectThis->GetName();

        if (!name) {
            name = "<unnamed>";
        }

        // TODO
        // v6 = alloca(SStrLen(name) + 5);
        // v17 = (char *)&v14;
        // SStrCopy((char *)&v14, "DBG:", 0x7FFFFFFF);
        // SStrCopy(v17 + 4, name, 0x7FFFFFFF);

        lua_pushstring(L, "this");
        lua_rawget(L, LUA_GLOBALSINDEX);

        if (!objectThis->lua_registered) {
            objectThis->RegisterScriptObject(0);
        }

        lua_rawgeti(L, LUA_REGISTRYINDEX, objectThis->lua_objectRef);
        lua_pushstring(L, "this");
        lua_insert(L, -2);
        lua_rawset(L, LUA_GLOBALSINDEX);
    }

    if (event) {
        lua_pushstring(L, "event");
        lua_rawget(L, LUA_GLOBALSINDEX);
        lua_pushvalue(L, v20);
        lua_pushstring(L, "event");
        lua_insert(L, -2);
        lua_rawset(L, LUA_GLOBALSINDEX);
    }

    static char argName[] = { 'a', 'r', 'g', '0', 0, 0, 0 };

    int32_t firstArg = event ? 1 : 0;
    int32_t argId = 0;

    if (firstArg < argCount) {
        for (int32_t i = firstArg; i < argCount; i++) {
            argId++;

            if (argId >= 10) {
                SStrPrintf(&argName[3], 3, "%d", argId);
            } else {
                argName[3] = '0' + argId;
                argName[4] = 0;
            }

            lua_pushstring(L, argName);
            lua_rawget(L, LUA_GLOBALSINDEX);
            lua_pushvalue(L, v20 + i);
            lua_pushstring(L, argName);
            lua_insert(L, -2);
            lua_rawset(L, LUA_GLOBALSINDEX);
        }
    }

    lua_checkstack(L, argCount + 3);

    lua_rawgeti(L, LUA_REGISTRYINDEX, FrameScript::s_errorHandlerRef);
    lua_rawgeti(L, LUA_REGISTRYINDEX, function);

    if (objectThis) {
        if (!objectThis->lua_registered) {
            objectThis->RegisterScriptObject(0);
        }

        lua_rawgeti(L, LUA_REGISTRYINDEX, objectThis->lua_objectRef);
        v19 = argCount + 1;
    }

    for (int32_t i = 0; i < argCount; ++i) {
        lua_pushvalue(L, v20 + i);
    }

    if (lua_pcall(L, v19, 0, -2 - v19)) {
        lua_settop(L, -2);
    }

    lua_settop(L, -2);

    for (int32_t i = argId; i > 0; i--) {
        if (i >= 10) {
            SStrPrintf(&argName[3], 3, "%d", i);
        } else {
            argName[3] = '0' + i;
            argName[4] = 0;
        }

        lua_pushstring(L, argName);
        lua_insert(L, -2);
        lua_rawset(L, LUA_GLOBALSINDEX);
    }

    if (event) {
        lua_pushstring(L, "event");
        lua_insert(L, -2);
        lua_rawset(L, LUA_GLOBALSINDEX);
    }

    if (objectThis) {
        lua_pushstring(L, "this");
        lua_insert(L, -2);
        lua_rawset(L, LUA_GLOBALSINDEX);
    }

    lua_settop(L, -1 - argCount);
}

int32_t FrameScript_ExecuteBuffer(const char* buffer, size_t bufferBytes, const char* bufferName, CStatus* status, const char* a5) {
    lua_State* L = FrameScript::s_context;

    lua_rawgeti(L, LUA_REGISTRYINDEX, FrameScript::s_errorHandlerRef);

    if (luaL_loadbuffer(L, buffer, bufferBytes, bufferName)) {
        if (status) {
            const char* v7 = lua_tostring(L, -1);
            status->Add(STATUS_ERROR, "%s", v7);
        }

        if (lua_pcall(L, 1, 0, 0)) {
            lua_settop(L, -2);
        }

        return 0;
    } else {
        int32_t v9 = 0;

        if (a5) {
            lua_pushstring(L, a5);
            lua_pushvalue(L, -4);
            v9 = 2;
        }

        if (lua_pcall(L, v9, 0, -2 - v9)) {
            if (status) {
                const char* v11 = lua_tostring(L, -1);
                status->Add(STATUS_ERROR, "%s", v11);
            }

            lua_settop(L, -3);

            return 0;
        } else {
            lua_settop(L, -2);

            return 1;
        }
    }
}

int32_t FrameScript_ExecuteFile(const char* filePath, const char* a2, MD5_CTX* md5, CStatus* status) {
    const char* v4 = filePath;

    char v12[260];

    if (SStrStr(filePath, "..")) {
        SStrCopy(v12, filePath, 260);

        char* i;
        char* j;

        for (i = (char*)SStrStr(v12, ".."); i; i = (char*)SStrStr(v12, "..")) {
            char v6 = *(i - 1);

            if (v6 != 92 && v6 != 47) {
                break;
            }

            char v7 = i[2];

            if (v7 != 92 && v7 != 47) {
                break;
            }

            for (j = i - 2; j >= v12; --j) {
                if (*j == 92) {
                    break;
                }

                if (*j == 47) {
                    break;
                }
            }

            SStrCopy(j + 1, i + 3, 0x7FFFFFFF);
        }

        v4 = v12;
    }

    char v11[264];

    SStrPrintf(v11, 261, "@%s", v4);

    void* fileBuffer;
    size_t fileBytes;

    if (SFile::Load(0, v4, &fileBuffer, &fileBytes, 0, 1, nullptr)) {
        if (md5) {
            MD5Update(md5, static_cast<unsigned char*>(fileBuffer), fileBytes);
        }

        int32_t v10 = FrameScript_ExecuteBuffer(static_cast<char*>(fileBuffer), fileBytes, v11, status, a2);

        SFile::Unload(fileBuffer);

        return v10;
    } else {
        if (status) {
            status->Add(STATUS_ERROR, "Error loading %s", v4);
        }

        return 0;
    }
}

void FrameScript_Flush() {
    if (FrameScript::s_context) {
        FrameScript_Destroy();
        FrameScript_Initialize(FrameScript::s_scriptProfileEnabled);
    }
}

void FrameScript_GetColor(lua_State* L, int32_t idx, CImVector& color) {
    float r = lua_tonumber(L, idx + 0);
    r = std::max(0.0f, std::min(r, 1.0f));

    float g = lua_tonumber(L, idx + 1);
    g = std::max(0.0f, std::min(g, 1.0f));

    float b = lua_tonumber(L, idx + 2);
    b = std::max(0.0f, std::min(b, 1.0f));

    float a = 1.0f;
    if (lua_isnumber(L, idx + 3)) {
        a = lua_tonumber(L, idx + 3);
        a = std::max(0.0f, std::min(a, 1.0f));
    }

    color.Set(a, r, g, b);
}

lua_State* FrameScript_GetContext(void) {
    return FrameScript::s_context;
}

const char* FrameScript_GetCurrentObject(lua_State* L, int32_t a2) {
    lua_Debug info;

    if (!lua_getstack(L, a2, &info)) {
        return nullptr;
    }

    lua_getinfo(L, "Sln", &info);

    if (info.source[0] != '*' && SStrCmp(info.namewhat, "method", 0x7FFFFFFFu)) {
        return nullptr;
    }

    if (!lua_getlocal(L, &info, 1)) {
        return nullptr;
    }

    const char* objName = nullptr;

    if (lua_type(L, -1) == LUA_TTABLE) {
        lua_rawgeti(L, -1, 0);
        FrameScript_Object* obj = static_cast<FrameScript_Object*>(lua_touserdata(L, -1));
        lua_settop(L, -2);

        if (obj) {
            objName = obj->GetName();

            if (!objName) {
                objName = "<unnamed>";
            }
        }
    }

    lua_settop(L, -2);

    return objName;
}

char FrameScript_GetPluralIndex(int32_t a1) {
    if (FrameScript::s_pluralRule == PLURAL_RULE_0) {
        return a1 != 1;
    }

    if (FrameScript::s_pluralRule == PLURAL_RULE_1) {
        return a1 > 1;
    }

    if (FrameScript::s_pluralRule != PLURAL_RULE_2) {
        return 0;
    }

    if ((unsigned int)(a1 % 100 - 11) <= 3) {
        return 2;
    }

    if (a1 % 10 == 1) {
        return 0;
    } else {
        return ((unsigned int)(a1 % 10 - 2) > 2) + 1;
    }
}

const char* FrameScript_GetText(const char* a1, int32_t count, FRAMESCRIPT_GENDER gender) {
    const char* text = "";

    char pluralIndex = FrameScript_GetPluralIndex(count);

    // Suffix based on pluralization rules
    char pluralSuffix[4];

    if (pluralIndex - 1 <= 8) {
        pluralSuffix[0] = 95;               // _
        pluralSuffix[1] = 80;               // P
        pluralSuffix[2] = 48 + pluralIndex; // 0-8
        pluralSuffix[3] = 0;                // NULL
    }

    // Gender suffix
    const char* genderSuffix = "";

    if (gender == GENDER_FEMALE) {
        genderSuffix = "_FEMALE";
    }

    uint32_t len = SStrLen((const char*)pluralSuffix) + SStrLen(a1) + SStrLen(genderSuffix) + 1;
    char* v9 = (char*)alloca(len);

    SStrPrintf(v9, len, "%s%s%s", a1, &pluralSuffix, genderSuffix);
    if (FrameScript_GetVariable(v9, &text)) {
        return text;
    }

    SStrPrintf(v9, len, "%s%s", a1, &pluralSuffix);
    if (FrameScript_GetVariable(v9, &text)) {
        return text;
    }

    SStrPrintf(v9, len, "%s%s", a1, genderSuffix);
    if (FrameScript_GetVariable(v9, &text)) {
        return text;
    }

    // Fallback
    FrameScript_GetVariable(a1, &text);
    return text;
}

int32_t FrameScript_GetVariable(const char* a1, const char** a2) {
    lua_State* L = FrameScript::s_context;

    int32_t v3 = 0;

    lua_pushstring(L, a1);
    lua_rawget(L, LUA_GLOBALSINDEX);

    if (lua_isstring(L, -1)) {
        v3 = 1;
        *a2 = lua_tostring(L, -1);
    }

    lua_settop(L, -2);

    return v3;
}

int32_t FrameScript_HandleError(lua_State* L) {
    if (!lua_isstring(L, -1)) {
        lua_pushstring(L, "UNKNOWN ERROR");
        lua_insert(L, -1);
    }

    const char* v1 = lua_tostring(L, -1);
    const char* v2 = SStrStr(v1, "*:");
    const char* objName = FrameScript_GetCurrentObject(L, 1);

    // TODO
    // Remove temporary console debug logging
    if (v2 && objName) {
        printf("Error: %s%s\n", objName, v2 + 1);
    } else {
        printf("Error: %s\n", v1);
    }

    if (v2 && objName) {
        lua_pushlstring(L, v1, v2 - v1);
        lua_pushstring(L, objName);
        lua_pushstring(L, v2 + 1);
        lua_concat(L, 3);
        lua_replace(L, -2);
    }

    // If seterrorhandler() has been called in Lua, invoke that Lua-side function now
    if (FrameScript::s_errorHandlerFun != -1) {
        FrameScript::s_handlingError = 1;

        lua_rawgeti(L, LUA_REGISTRYINDEX, FrameScript::s_errorHandlerFun);
        lua_insert(L, -2);
        lua_call(L, 1, 1);

        FrameScript::s_handlingError = 0;
    }

    return 1;
}

int32_t FrameScript_Initialize(int32_t a1) {
    FrameScript::s_mempool = luaM_initPool();
    FrameScript::s_context = lua_newstate(luaM_reallocPool, FrameScript::s_mempool); // TODO a1
    FrameScript::s_scriptTimeUsed = 0ll;

    int64_t v1 = OsGetAsyncClocksPerSecond();
    FrameScript::s_scriptProfileEnabled = a1;
    FrameScript::s_errorHandlerFun = -1;
    FrameScript::s_scriptTimeDivisor = 1000.0 / (double)v1;

    lua_pushcclosure(FrameScript::s_context, FrameScript_HandleError, 0);
    FrameScript::s_errorHandlerRef = luaL_ref(FrameScript::s_context, LUA_REGISTRYINDEX);
    lua_createtable(FrameScript::s_context, 0, 0);
    FrameScript::s_recursiveTableHash = luaL_ref(FrameScript::s_context, LUA_REGISTRYINDEX);
    lua_gc(FrameScript::s_context, LUA_GCSETPAUSE, 110);
    luaopen_base(FrameScript::s_context);
    lua_settop(FrameScript::s_context, -3);
    luaopen_string(FrameScript::s_context);
    lua_settop(FrameScript::s_context, -2);
    luaopen_table(FrameScript::s_context);
    lua_settop(FrameScript::s_context, -2);
    luaopen_math(FrameScript::s_context);
    lua_settop(FrameScript::s_context, -2);
    luaopen_bit(FrameScript::s_context);
    lua_settop(FrameScript::s_context, -2);
    luaL_register(FrameScript::s_context, "_G", FrameScriptInternal::extra_funcs);
    lua_settop(FrameScript::s_context, -2);

    FrameScript_Execute(FrameScript::s_compat_lua, "compat.lua", 0);

    return 1;
}

void FrameScript_RegisterFunction(const char* name, int32_t (*function)(struct lua_State *)) {
    lua_State* L = FrameScript::s_context;

    lua_pushcclosure(L, function, 0);
    lua_pushstring(L, name);
    lua_insert(L, -2);
    lua_rawset(L, LUA_GLOBALSINDEX);
}

void FrameScript_RegisterScriptEvent(FrameScript_Object* object, FrameScript_EventObject* event) {
    if (event->pendingSignalCount) {
        auto node = event->registerListeners.Head();

        while (node) {
            if (node->listener == object) {
                return;
            }

            node = node->Next();
        }

        node = event->registerListeners.NewNode(2, 0, 0x8);
        node->listener = object;
    } else {
        auto node = event->listeners.NewNode(2, 0, 0x8);
        node->listener = object;
    }
}

void FrameScript_SetPluralRule(PLURAL_RULE rule) {
    FrameScript::s_pluralRule = rule;
}

int32_t FrameScript_ShouldSignalEvent(uint32_t index) {
    // TODO
    return 1;
}

void FrameScript_PushEventName(uint32_t index) {
    // TODO
    // bounds checks

    FrameScript_EventObject* event = FrameScript::s_scriptEvents[index];
    lua_pushstring(FrameScript::s_context, event->GetName());
}

int32_t FrameScript_PushEventArgs(const char* format, va_list args) {
    int32_t argCount = 0;

    if (!format || !*format) {
        return argCount;
    }

    const char* cur = format;

    while (*cur) {
        if (*cur == '%') {
            cur++;

            switch (*cur) {
                case 'b':
                    lua_pushboolean(FrameScript::s_context, va_arg(args, int32_t));
                    argCount++;
                    break;

                case 'd':
                    lua_pushnumber(FrameScript::s_context, va_arg(args, int32_t));
                    argCount++;
                    break;

                case 'f':
                    lua_pushnumber(FrameScript::s_context, va_arg(args, double));
                    argCount++;
                    break;

                case 's':
                    lua_pushstring(FrameScript::s_context, va_arg(args, char*));
                    argCount++;
                    break;

                case 'u':
                    lua_pushnumber(FrameScript::s_context, va_arg(args, uint32_t));
                    argCount++;
                    break;
            }
        }

        cur++;
    }

    return argCount;
}

void FrameScript_SignalEvent(uint32_t index, lua_State* L, int32_t argCount) {
    auto event = FrameScript::s_scriptEvents[index];

    if (!event) {
        return;
    }

    // TODO
    event->signalCount++;
    event->pendingSignalCount++;

    lua_checkstack(L, argCount);

    auto node = event->listeners.Head();

    while (node) {
        auto unregisterNode = event->unregisterListeners.Head();

        while (unregisterNode) {
            if (node->listener == unregisterNode->listener) {
                break;
            }

            unregisterNode = unregisterNode->Next();
        }

        if (unregisterNode) {
            break;
        }

        auto script = &node->listener->m_onEvent;

        if (script->luaRef) {
            for (int32_t i = 0; i < argCount; i++) {
                lua_pushvalue(L, -argCount);
            }

            FrameScript_Execute(script->luaRef, node->listener, argCount, script->unk, event);
        }

        node = node->Next();
    }

    event->pendingSignalCount--;

    auto unregisterNode = event->unregisterListeners.Head();

    while (unregisterNode) {
        FrameScript_UnregisterScriptEvent(unregisterNode->listener, event);
        unregisterNode = event->unregisterListeners.DeleteNode(unregisterNode);
    }

    auto registerNode = event->registerListeners.Head();

    while (registerNode) {
        FrameScript_RegisterScriptEvent(registerNode->listener, event);
        registerNode = event->registerListeners.DeleteNode(registerNode);
    }
}

void FrameScript_SignalEvent(uint32_t index, const char* format, ...) {
    if (!FrameScript_ShouldSignalEvent(index)) {
        return;
    }

    va_list args;
    va_start(args, format);

    // TODO
    // v6 = lua_taintexpected++ == -1;
    // v18 = lua_tainted;
    // if (!v6 && !lua_taintedclosure) {
    //     lua_tainted = 0;
    // }

    FrameScript_PushEventName(index);

    int32_t argCount = 1;
    argCount += FrameScript_PushEventArgs(format, args);

    FrameScript_SignalEvent(index, FrameScript::s_context, argCount);

    lua_settop(FrameScript::s_context, -1 - argCount);

    // TODO
    // if (lua_taintexpected && !lua_taintedclosure) {
    //     lua_tainted = v18;
    // }
    // if (--lua_taintexpected <= 0) {
    //     lua_taintexpected = 0;
    // }

    va_end(args);
}

const char* FrameScript_Sprintf(lua_State* L, int32_t idx, char buffer[], uint32_t bufferLen) {
    auto write = buffer;
    auto availableBytes = bufferLen;

    size_t formatLen;
    auto format = luaL_checklstring(L, idx, &formatLen);
    auto formatEnd = format + formatLen;

    char specifier[128];

    auto curIdx = idx;

    while (format < formatEnd && availableBytes > 1) {
        auto ch = *format++;

        // Escaped %

        if (ch == '%' && *format == '%') {
            *write++ = '%';

            format++;
            availableBytes--;

            continue;
        }

        // Non-specifier

        if (ch != '%') {
            *write++ = ch;

            availableBytes--;

            continue;
        }

        // Specifier

        ch = *format;
        specifier[0] = '%';

        // Position specifier

        if (ch >= '0' && ch <= '9' && format[1] == '$') {
            curIdx = idx + (ch - '0') - 1;
            format += 2;
        }

        curIdx++;

        // Subspecifiers

        auto subspecifierStart = format;

        // Flags

        while (*format == '-' || *format == '+' || *format == ' ' || *format == '#' || *format == '0') {
            format++;
        }

        // Width

        while (*format >= '0' && *format <= '9') {
            format++;
        }

        // Precision

        while (*format == '.' || *format == '-') {
            format++;
        }

        while (*format >= '0' && *format <= '9') {
            format++;
        }

        // Validate subspecifier

        auto subspecifierLen = format - subspecifierStart;

        if (subspecifierLen > 125) {
            luaL_error(L, "invalid format (width or precision too long)");
        }

        // Copy subspecifier and specifier (len + 1)

        memcpy(&specifier[1], subspecifierStart, subspecifierLen + 1);

        // Terminate specifier

        specifier[subspecifierLen + 2] = '\0';

        // Evaluate specifier

        ch = *format++;

        switch (ch) {
            // Floating point / scientific notation
            case 'E':
            case 'G':
            case 'e':
            case 'f':
            case 'g': {
                auto number = luaL_checknumber(L, curIdx);
                auto written = SStrPrintf(write, availableBytes, specifier, number);

                if (written > 0) {
                    write += written;
                    availableBytes -= written;
                }

                break;
            }

            // Floating point with decimal conversion
            case 'F': {
                // Replace with lowercase f
                for (char* replace = specifier; *replace; replace++) {
                    if (*replace == 'F') {
                        *replace = 'f';
                    }
                }

                auto number = luaL_checknumber(L, curIdx);
                auto written = SStrPrintf(write, availableBytes, specifier, number);

                if (written > 0) {
                    // TODO lua_convertdecimal(write);
                    write += written;
                    availableBytes -= written;
                }

                break;
            }

            // Unsigned hex / octal / decimal
            case 'X':
            case 'o':
            case 'u':
            case 'x': {
                auto number = static_cast<uint64_t>(luaL_checknumber(L, curIdx));
                auto written = SStrPrintf(write, availableBytes, specifier, static_cast<uint32_t>(number));

                if (written > 0) {
                    write += written;
                    availableBytes -= written;
                }

                break;
            }

            // Char
            case 'c': {
                *write++ = static_cast<char>(luaL_checknumber(L, curIdx));
                availableBytes--;

                break;
            }

            // Signed decimal
            case 'd':
            case 'i': {
                auto number = luaL_checknumber(L, curIdx);
                auto written = SStrPrintf(write, availableBytes, specifier, static_cast<int32_t>(number));

                if (written > 0) {
                    // TODO lua_convertdecimal(write);
                    write += written;
                    availableBytes -= written;
                }

                break;
            }

            // String
            case 's': {
                size_t stringLen;
                auto string = luaL_checklstring(L, curIdx, &stringLen);
                auto written = SStrPrintf(write, availableBytes, specifier, string);

                if (written > 0) {
                    write += written;
                    availableBytes -= written;
                }

                break;
            }

            default: {
                luaL_error(L, "invalid option in `format'");
            }
        }
    }

    // Terminate

    write[0] = '\0';

    return buffer;
}

void FrameScript_UnregisterFunction(const char* name) {
    auto L = FrameScript::s_context;

    lua_pushnil(L);
    lua_pushstring(L, name);
    lua_insert(L, -2);
    lua_rawset(L, LUA_GLOBALSINDEX);
}

void FrameScript_UnregisterScriptEvent(FrameScript_Object* object, FrameScript_EventObject* event) {
    if (event->pendingSignalCount) {
        auto node = event->unregisterListeners.Head();

        while (node) {
            if (node->listener == object) {
                return;
            }

            node = node->Next();
        }

        node = event->unregisterListeners.NewNode(2, 0, 0x8);
        node->listener = object;
    } else {
        auto node = event->listeners.Head();

        while (node) {
            if (node->listener == object) {
                event->listeners.DeleteNode(node);
                break;
            }

            node = node->Next();
        }
    }
}
