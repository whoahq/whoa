#include "ui/ScriptFunctions.hpp"
#include "ui/Types.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t Script_RequestRealmList(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_RealmListUpdateRate(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_CancelRealmListQuery(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_GetNumRealms(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_GetRealmInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_ChangeRealm(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_GetRealmCategories(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_SetPreferredInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_SortRealms(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_GetSelectedCategory(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_RealmListDialogCancelled(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_IsInvalidTournamentRealmCategory(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_IsTournamentRealmCategory(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_IsInvalidLocale(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

FrameScript_Method FrameScript::s_ScriptFunctions_RealmList[NUM_SCRIPT_FUNCTIONS_REALM_LIST] = {
    { "RequestRealmList",               &Script_RequestRealmList },
    { "RealmListUpdateRate",            &Script_RealmListUpdateRate },
    { "CancelRealmListQuery",           &Script_CancelRealmListQuery },
    { "GetNumRealms",                   &Script_GetNumRealms },
    { "GetRealmInfo",                   &Script_GetRealmInfo },
    { "ChangeRealm",                    &Script_ChangeRealm },
    { "GetRealmCategories",             &Script_GetRealmCategories },
    { "SetPreferredInfo",               &Script_SetPreferredInfo },
    { "SortRealms",                     &Script_SortRealms },
    { "GetSelectedCategory",            &Script_GetSelectedCategory },
    { "RealmListDialogCancelled",       &Script_RealmListDialogCancelled },
    { "IsInvalidTournamentRealmCategory", &Script_IsInvalidTournamentRealmCategory },
    { "IsTournamentRealmCategory",      &Script_IsTournamentRealmCategory },
    { "IsInvalidLocale",                &Script_IsInvalidLocale }
};
