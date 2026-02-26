#ifndef UI_SCRIPT_FUNCTIONS_SHARED_HPP
#define UI_SCRIPT_FUNCTIONS_SHARED_HPP

#include "util/Lua.hpp"
#include <cstdint>

int32_t Script_GetAccountExpansionLevel(lua_State* L);

int32_t Script_IsLinuxClient(lua_State* L);

int32_t Script_IsMacClient(lua_State* L);

int32_t Script_IsWindowsClient(lua_State* L);

#endif
