#include "ui/game/ActionBarScript.hpp"
#include "ui/FrameScript.hpp"
#include "ui/game/CGActionBar.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t Script_GetActionInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetActionTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetActionCount(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetActionCooldown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetActionAutocast(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetActionText(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_HasAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UseAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PickupAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PlaceAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsAttackAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsCurrentAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsAutoRepeatAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsUsableAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsConsumableAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsStackableAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsEquippedAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ActionHasRange(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsActionInRange(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBonusBarOffset(lua_State* L) {
    lua_pushnumber(L, CGActionBar::GetBonusBarOffset());

    return 1;
}

int32_t Script_GetMultiCastBarOffset(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ChangeActionBarPage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetActionBarPage(lua_State* L) {
    if (CGActionBar::s_tempPageActiveFlags) {
        lua_pushinteger(L, 1);
    } else {
        lua_pushinteger(L, CGActionBar::s_currentPage + 1);
    }

    return 1;
}

int32_t Script_GetActionBarToggles(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetActionBarToggles(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsPossessBarVisible(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMultiCastTotemSpells(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetMultiCastSpell(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

static FrameScript_Method s_ScriptFunctions[] = {
    { "GetActionInfo",              &Script_GetActionInfo },
    { "GetActionTexture",           &Script_GetActionTexture },
    { "GetActionCount",             &Script_GetActionCount },
    { "GetActionCooldown",          &Script_GetActionCooldown },
    { "GetActionAutocast",          &Script_GetActionAutocast },
    { "GetActionText",              &Script_GetActionText },
    { "HasAction",                  &Script_HasAction },
    { "UseAction",                  &Script_UseAction },
    { "PickupAction",               &Script_PickupAction },
    { "PlaceAction",                &Script_PlaceAction },
    { "IsAttackAction",             &Script_IsAttackAction },
    { "IsCurrentAction",            &Script_IsCurrentAction },
    { "IsAutoRepeatAction",         &Script_IsAutoRepeatAction },
    { "IsUsableAction",             &Script_IsUsableAction },
    { "IsConsumableAction",         &Script_IsConsumableAction },
    { "IsStackableAction",          &Script_IsStackableAction },
    { "IsEquippedAction",           &Script_IsEquippedAction },
    { "ActionHasRange",             &Script_ActionHasRange },
    { "IsActionInRange",            &Script_IsActionInRange },
    { "GetBonusBarOffset",          &Script_GetBonusBarOffset },
    { "GetMultiCastBarOffset",      &Script_GetMultiCastBarOffset },
    { "ChangeActionBarPage",        &Script_ChangeActionBarPage },
    { "GetActionBarPage",           &Script_GetActionBarPage },
    { "GetActionBarToggles",        &Script_GetActionBarToggles },
    { "SetActionBarToggles",        &Script_SetActionBarToggles },
    { "IsPossessBarVisible",        &Script_IsPossessBarVisible },
    { "GetMultiCastTotemSpells",    &Script_GetMultiCastTotemSpells },
    { "SetMultiCastSpell",          &Script_SetMultiCastSpell },
};

void ActionBarRegisterScriptFunctions() {
    for (auto& func : s_ScriptFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
    }
}
