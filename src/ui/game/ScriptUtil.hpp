#ifndef UI_GAME_SCRIPT_UTIL_HPP
#define UI_GAME_SCRIPT_UTIL_HPP

#include "util/GUID.hpp"

bool Script_GetGUIDFromString(const char*& token, WOWGUID& guid);

bool Script_GetGUIDFromToken(const char* token, WOWGUID& guid, bool defaultToTarget);

#endif
