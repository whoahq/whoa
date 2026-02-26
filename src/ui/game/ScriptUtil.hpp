#ifndef UI_GAME_SCRIPT_UTIL_HPP
#define UI_GAME_SCRIPT_UTIL_HPP

#include "util/GUID.hpp"

class CGUnit_C;

CGUnit_C* Script_GetUnitFromName(const char* name);

bool Script_GetGUIDFromString(const char*& token, WOWGUID& guid);

bool Script_GetGUIDFromToken(const char* token, WOWGUID& guid, bool defaultToTarget);

#endif
