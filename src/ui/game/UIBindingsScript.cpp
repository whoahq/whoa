#include "ui/game/UIBindingsScript.hpp"
#include "ui/FrameScript.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t Script_GetNumBindings(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBinding(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetBinding(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetBindingSpell(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetBindingItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetBindingMacro(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetBindingClick(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetOverrideBinding(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetOverrideBindingSpell(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetOverrideBindingItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetOverrideBindingMacro(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetOverrideBindingClick(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ClearOverrideBindings(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBindingKey(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBindingAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBindingByKey(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RunBinding(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCurrentBindingSet(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_LoadBindings(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SaveBindings(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumModifiedClickActions(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetModifiedClickAction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetModifiedClick(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetModifiedClick(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsModifiedClick(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetClickFrame(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

static FrameScript_Method s_ScriptFunctions[] = {
    { "GetNumBindings",             &Script_GetNumBindings },
    { "GetBinding",                 &Script_GetBinding },
    { "SetBinding",                 &Script_SetBinding },
    { "SetBindingSpell",            &Script_SetBindingSpell },
    { "SetBindingItem",             &Script_SetBindingItem },
    { "SetBindingMacro",            &Script_SetBindingMacro },
    { "SetBindingClick",            &Script_SetBindingClick },
    { "SetOverrideBinding",         &Script_SetOverrideBinding },
    { "SetOverrideBindingSpell",    &Script_SetOverrideBindingSpell },
    { "SetOverrideBindingItem",     &Script_SetOverrideBindingItem },
    { "SetOverrideBindingMacro",    &Script_SetOverrideBindingMacro },
    { "SetOverrideBindingClick",    &Script_SetOverrideBindingClick },
    { "ClearOverrideBindings",      &Script_ClearOverrideBindings },
    { "GetBindingKey",              &Script_GetBindingKey },
    { "GetBindingAction",           &Script_GetBindingAction },
    { "GetBindingByKey",            &Script_GetBindingByKey },
    { "RunBinding",                 &Script_RunBinding },
    { "GetCurrentBindingSet",       &Script_GetCurrentBindingSet },
    { "LoadBindings",               &Script_LoadBindings },
    { "SaveBindings",               &Script_SaveBindings },
    { "GetNumModifiedClickActions", &Script_GetNumModifiedClickActions },
    { "GetModifiedClickAction",     &Script_GetModifiedClickAction },
    { "SetModifiedClick",           &Script_SetModifiedClick },
    { "GetModifiedClick",           &Script_GetModifiedClick },
    { "IsModifiedClick",            &Script_IsModifiedClick },
    { "GetClickFrame",              &Script_GetClickFrame },
};

void UIBindingsRegisterScriptFunctions() {
    for (auto& func : s_ScriptFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
    }
}
