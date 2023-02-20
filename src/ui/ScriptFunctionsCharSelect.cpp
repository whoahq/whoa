#include "ui/ScriptFunctions.hpp"
#include "db/Db.hpp"
#include "glue/CCharacterSelection.hpp"
#include "ui/Types.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t Script_SetCharSelectModelFrame(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_SetCharSelectBackground(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_GetCharacterListUpdate(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_GetNumCharacters(lua_State* L) {
    lua_pushnumber(L, CCharacterSelection::s_characterList.Count());

    return 1;
}

int32_t Script_GetCharacterInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_SelectCharacter(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_DeleteCharacter(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_RenameCharacter(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_DeclineCharacter(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_UpdateSelectionCustomizationScene(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_GetCharacterSelectFacing(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_SetCharacterSelectFacing(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t Script_GetSelectBackgroundModel(lua_State* L) {
    if (!lua_isnumber(L, 1)) {
        return luaL_error(L, "Usage: GetSelectBackgroundModel(index)");
    }

    auto characterIndex = static_cast<int32_t>(lua_tonumber(L, 1)) - 1;

    // TODO

    ChrRacesRec* racesRec = nullptr;

    if (characterIndex < 0 || characterIndex >= CCharacterSelection::s_characterList.Count()) {
        racesRec = g_chrRacesDB.GetRecord(2);
    } else {
        // TODO
    }

    if (racesRec) {
        lua_pushstring(L, racesRec->m_clientFileString);
    } else {
        lua_pushstring(L, "");
    }

    return 1;
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
