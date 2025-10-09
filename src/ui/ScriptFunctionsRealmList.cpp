#include "ui/ScriptFunctions.hpp"
#include "client/ClientServices.hpp"
#include "db/Db.hpp"
#include "glue/CGlueMgr.hpp"
#include "glue/CRealmList.hpp"
#include "util/StringTo.hpp"
#include "ui/Types.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>
#include <storm/String.hpp>

int32_t Script_RequestRealmList(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RealmListUpdateRate(lua_State* L) {
    if (ClientServices::LoginConnection() && ClientServices::LoginConnection()->GetLoginServerType() == 1) {
        lua_pushnumber(L, 4.0);
    } else {
        lua_pushnumber(L, 5.0);
    }

    return 1;
}

int32_t Script_CancelRealmListQuery(lua_State* L) {
    CGlueMgr::CancelRealmListQuery();

    return 0;
}

int32_t Script_GetNumRealms(lua_State* L) {
    if (lua_isnumber(L, 1)) {
        int32_t categoryIndex = lua_tonumber(L, 1) - 1;
        categoryIndex = CRealmList::Sub4DE910(categoryIndex);

        int32_t numRealms = 0;
        if (categoryIndex < CRealmList::s_categories.Count()) {
            auto realmCategory = CRealmList::s_categories[categoryIndex];
            if (realmCategory) {
                numRealms = realmCategory->uint14;
            }
        }

        lua_pushnumber(L, numRealms);

        return 1;
    }

    int32_t numRealms = 0;
    for (int32_t i = 0; i < CRealmList::s_categories.Count(); i++) {
        auto realmCategory = CRealmList::s_categories[i];
        if (realmCategory) {
            numRealms += realmCategory->uint14;
        }
    }

    lua_pushnumber(L, numRealms);

    return 1;
}

int32_t Script_GetRealmInfo(lua_State* L) {
    if (!lua_isnumber(L, 1)) {
        return luaL_error(L, "Usage: GetRealmInfo(category, index)");
    }

    REALM_INFO* realmInfo = nullptr;

    if (lua_isnumber(L, 2)) {
        int32_t categoryIndex = lua_tonumber(L, 1) - 1;
        categoryIndex = CRealmList::Sub4DE910(categoryIndex);

        auto realmCategory = categoryIndex < CRealmList::s_categories.Count()
            ? CRealmList::s_categories[categoryIndex]
            : nullptr;
        int32_t realmIndex = lua_tonumber(L, 2) - 1;

        if (realmCategory && realmIndex < realmCategory->uint14) {
            realmInfo = ClientServices::GetRealmInfoByIndex(realmCategory->m_realms[realmIndex]);
        }
    } else {
        int32_t realmIndex = lua_tonumber(L, 1) - 1;

        for (int32_t i = 0; i < CRealmList::s_categories.Count(); i++) {
            auto realmCategory = CRealmList::s_categories[i];
            if (realmCategory) {
                if (realmCategory->uint14 > realmIndex) {
                    realmInfo = ClientServices::GetRealmInfoByIndex(realmCategory->m_realms[realmIndex]);
                } else {
                    realmIndex -= realmCategory->uint14;
                }
            }
        }
    }

    if (!realmInfo) {
        lua_pushnil(L);
        lua_pushnumber(L, 0.0);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnumber(L, 0.0);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);

        return 14;
    }

    // name
    lua_pushstring(L, realmInfo->name);

    // numCharacters
    lua_pushnumber(L, realmInfo->numChars);

    // invalidRealm
    if (/* TODO Sub422140() || */ !(realmInfo->flags & 0x1)) {
        lua_pushnil(L);
    } else {
        lua_pushnumber(L, 1.0);
    }

    // realmDown
    if (realmInfo->flags & 0x2) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    // currentRealm
    auto selectedRealmName = ClientServices::GetSelectedRealmName();
    if (SStrCmpI(realmInfo->name, selectedRealmName, STORM_MAX_STR) /* TODO SStrCmpUTF8I */) {
        lua_pushnil(L);
    } else {
        lua_pushnumber(L, 1.0);
    }

    int32_t realmIsPvp = 0;
    int32_t realmIsRp = 0;

    for (int32_t i = 0; i < g_cfg_ConfigsDB.GetNumRecords(); i++) {
        auto config = g_cfg_ConfigsDB.GetRecordByIndex(i);

        if (config->m_realmType == realmInfo->type) {
            realmIsPvp = config->m_playerKillingAllowed != 0;
            realmIsRp = config->m_roleplaying != 0;
        }
    }

    // pvp
    if (realmIsPvp) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    // rp
    if (realmIsRp) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    // load
    // TODO auto v18 = CRealmList::Sub4DE5B0(realmInfo);
    float v18 = 0.0f;
    lua_pushnumber(L, v18);

    // locked
    if (realmInfo->locked & 0x1) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    // major, minor, revision, build, type
    if (realmInfo->flags & 0x4) {
        lua_pushnumber(L, realmInfo->majorVersion);
        lua_pushnumber(L, realmInfo->minorVersion);
        lua_pushnumber(L, realmInfo->patchVersion);
        lua_pushnumber(L, realmInfo->revision);
        lua_pushnumber(L, realmInfo->type);
    } else {
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnil(L);
    }

    return 14;
}

int32_t Script_ChangeRealm(lua_State* L) {
    if (!lua_isnumber(L, 1)) {
        return luaL_error(L, "Usage: ChangeRealm(category, index)");
    }

    REALM_INFO* realmInfo = nullptr;

    if (lua_isnumber(L, 2)) {
        int32_t categoryIndex = lua_tonumber(L, 1) - 1;
        categoryIndex = CRealmList::Sub4DE910(categoryIndex);

        CRealmList::s_selectedCategory = categoryIndex;

        auto realmCategory = categoryIndex < CRealmList::s_categories.Count()
            ? CRealmList::s_categories[categoryIndex]
            : nullptr;
        int32_t realmIndex = lua_tonumber(L, 2) - 1;

        if (realmCategory && realmIndex < realmCategory->uint14) {
            realmInfo = ClientServices::GetRealmInfoByIndex(realmCategory->m_realms[realmIndex]);
        }
    } else {
        int32_t realmIndex = lua_tonumber(L, 1) - 1;

        for (int32_t i = 0; i < CRealmList::s_categories.Count(); i++) {
            auto realmCategory = CRealmList::s_categories[i];
            if (realmCategory) {
                if (realmCategory->uint14 > realmIndex) {
                    realmInfo = ClientServices::GetRealmInfoByIndex(realmCategory->m_realms[realmIndex]);
                } else {
                    realmIndex -= realmCategory->uint14;
                }
            }
        }
    }

    if (realmInfo) {
        if (CRealmList::s_preferredCategory == -1) {
            CRealmList::s_preferredCategory = 0;
        }

        CGlueMgr::ChangeRealm(realmInfo);
    }

    return 0;
}

int32_t Script_GetRealmCategories(lua_State* L) {
    lua_checkstack(L, CRealmList::s_categories.Count());

    int32_t pushed = 0;

    for (uint32_t i = 0; i < CRealmList::s_categories.Count(); i++) {
        auto realmCategory = CRealmList::s_categories[i];

        if (realmCategory && realmCategory->uint14) {
            auto categoryName = realmCategory->m_category
                ? realmCategory->m_category->m_name
                : "UNKNOWN";

            lua_pushstring(L, categoryName);

            pushed++;
        }
    }

    if (pushed) {
        return pushed;
    }

    auto realmCategory = CRealmList::s_categories[0];
    auto categoryName = realmCategory->m_category
        ? realmCategory->m_category->m_name
        : "UNKNOWN";

    lua_pushstring(L, categoryName);

    return 1;
}

int32_t Script_SetPreferredInfo(lua_State* L) {
    if (!lua_isnumber(L, 1)) {
        return luaL_error(L, "Usage: SetPreferredInfo(index, pvp, rp)");
    }

    uint32_t index = lua_tonumber(L, 1);
    int32_t pvp = StringToBOOL(L, 2, 0);
    int32_t rp = StringToBOOL(L, 3, 0);

    CRealmList::SetPreferredInfo(index, pvp, rp);

    return 0;
}

int32_t Script_SortRealms(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSelectedCategory(lua_State* L) {
    if (CRealmList::s_categories.Count() == 0) {
        lua_pushnumber(L, 1.0);

        return 1;
    }

    int32_t selectedCategory = -1;
    for (uint32_t i = 0; i < CRealmList::s_categories.Count(); i++) {
        auto realmCategory = CRealmList::s_categories[i];
        if (realmCategory && realmCategory->uint14) {
            selectedCategory++;

            if (i >= CRealmList::s_selectedCategory) {
                break;
            }
        }
    }

    lua_pushnumber(L, selectedCategory + 1);

    return 1;
}

int32_t Script_RealmListDialogCancelled(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsInvalidTournamentRealmCategory(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsTournamentRealmCategory(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsInvalidLocale(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
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
