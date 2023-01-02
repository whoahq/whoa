#include "ui/ScriptFunctions.hpp"
#include "glue/CCharacterSelection.hpp"
#include "ui/Types.hpp"
#include "util/Lua.hpp"
#include <cstdint>

int32_t Script_SetCharSelectModelFrame(lua_State* L) {
    // TODO

    return 0;
}

int32_t Script_SetCharSelectBackground(lua_State* L) {
    // TODO

    return 0;
}

int32_t Script_GetCharacterListUpdate(lua_State* L) {
    // TODO

    return 0;
}

int32_t Script_GetNumCharacters(lua_State* L) {
    lua_pushnumber(L, CCharacterSelection::s_characterList.Count());

    return 1;
}

int32_t Script_GetCharacterInfo(lua_State* L) {
    // TODO

    return 0;
}

int32_t Script_SelectCharacter(lua_State* L) {
    // TODO

    return 0;
}

int32_t Script_DeleteCharacter(lua_State* L) {
    // TODO

    return 0;
}

int32_t Script_RenameCharacter(lua_State* L) {
    // TODO

    return 0;
}

int32_t Script_DeclineCharacter(lua_State* L) {
    // TODO

    return 0;
}

int32_t Script_UpdateSelectionCustomizationScene(lua_State* L) {
    // TODO

    return 0;
}

int32_t Script_GetCharacterSelectFacing(lua_State* L) {
    // TODO

    return 0;
}

int32_t Script_SetCharacterSelectFacing(lua_State* L) {
    // TODO

    return 0;
}

int32_t Script_GetSelectBackgroundModel(lua_State* L) {
    // TODO

    return 0;
}

FrameScript_Method FrameScript::s_ScriptFunctions_CharSelect[NUM_SCRIPT_FUNCTIONS_CHAR_SELECT] = {
    { "SetCharSelectModelFrame",    &Script_SetCharSelectModelFrame },
    { "SetCharSelectBackground",    &Script_SetCharSelectBackground },
    { "GetCharacterListUpdate",     &Script_GetCharacterListUpdate },
    { "GetNumCharacters",           &Script_GetNumCharacters },
    { "GetCharacterInfo",           &Script_GetCharacterInfo },
    { "SelectCharacter",            &Script_SelectCharacter },
    { "DeleteCharacter",            &Script_DeleteCharacter },
    { "RenameCharacter",            &Script_RenameCharacter },
    { "DeclineCharacter",           &Script_DeclineCharacter },
    { "UpdateSelectionCustomizationScene", &Script_UpdateSelectionCustomizationScene },
    { "GetCharacterSelectFacing",   &Script_GetCharacterSelectFacing },
    { "SetCharacterSelectFacing",   &Script_SetCharacterSelectFacing },
    { "GetSelectBackgroundModel",   &Script_GetSelectBackgroundModel }
};
