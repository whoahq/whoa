#include "ui/ScriptFunctionsShared.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t Script_GetAccountExpansionLevel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsLinuxClient(lua_State* L) {
#if defined(WHOA_SYSTEM_LINUX)
    lua_pushnumber(L, 1.0);
#else
    lua_pushnil(L);
#endif

    return 1;
}

int32_t Script_IsMacClient(lua_State* L) {
#if defined(WHOA_SYSTEM_MAC)
    lua_pushnumber(L, 1.0);
#else
    lua_pushnil(L);
#endif

    return 1;
}

int32_t Script_IsWindowsClient(lua_State* L) {
#if defined(WHOA_SYSTEM_WIN)
    lua_pushnumber(L, 1.0);
#else
    lua_pushnil(L);
#endif

    return 1;
}
