#include "ui/FrameScript.hpp"
#include "ui/ScriptFunctionsShared.hpp"
#include "ui/Types.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <common/Time.hpp>
#include <cstdint>

int32_t Script_GetTime(lua_State* L) {
    uint64_t ms = OsGetAsyncTimeMs();
    lua_pushnumber(L, static_cast<double>(ms) / 1000.0);

    return 1;
}

int32_t Script_GetGameTime(lua_State* L) {
    // TODO real implementation
    lua_pushnumber(L, 1.0);
    lua_pushnumber(L, 15.0);
    WHOA_UNIMPLEMENTED(2);
}

int32_t Script_ConsoleExec(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AccessDenied(lua_State* L) {
    luaL_error(L, "Access Denied");

    return 0;
}

static FrameScript_Method s_SystemFunctions[] = {
    { "GetTime",                    &Script_GetTime },
    { "GetGameTime",                &Script_GetGameTime },
    { "ConsoleExec",                &Script_ConsoleExec },
    { "ReadFile",                   &Script_AccessDenied },
    { "DeleteFile",                 &Script_AccessDenied },
    { "AppendToFile",               &Script_AccessDenied },
    { "GetAccountExpansionLevel",   &Script_GetAccountExpansionLevel }
};

void SystemRegisterFunctions() {
    for (auto& func : s_SystemFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
    }
}

void SystemUnregisterFunctions() {
    for (auto& func : s_SystemFunctions) {
        FrameScript_UnregisterFunction(func.name);
    }
}
