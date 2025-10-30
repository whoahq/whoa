#include "ui/ScriptFunctions.hpp"
#include "db/Db.hpp"
#include "glue/CCharacterSelection.hpp"
#include "object/client/Unit_C.hpp"
#include "ui/CSimpleModelFFX.hpp"
#include "ui/Types.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t Script_SetCharSelectModelFrame(lua_State* L) {
    if (!lua_isstring(L, 1)) {
        return luaL_error(L, "Usage: SetCharSelectModelFrame(\"frameName\")");
    }

    auto type = CSimpleModel::GetObjectType();
    auto name = lua_tostring(L, 1);
    auto frame = CScriptObject::GetScriptObjectByName(name, type);

    if (frame) {
        CCharacterSelection::s_modelFrame = static_cast<CSimpleModelFFX*>(frame);
    }

    return 0;
}

int32_t Script_SetCharSelectBackground(lua_State* L) {
    if (!lua_isstring(L, 1)) {
        return luaL_error(L, "Usage: SetCharSelectBackground(\"filename\")");
    }

    auto modelPath = lua_tostring(L, 1);
    CCharacterSelection::SetBackgroundModel(modelPath);

    return 0;
}

int32_t Script_GetCharacterListUpdate(lua_State* L) {
    CCharacterSelection::OnGetCharacterList();

    return 0;
}

int32_t Script_GetNumCharacters(lua_State* L) {
    lua_pushnumber(L, CCharacterSelection::s_characterList.Count());

    return 1;
}

int32_t Script_GetCharacterInfo(lua_State* L) {
    if (!lua_isnumber(L, 1)) {
        return luaL_error(L, "Usage: GetCharacterInfo(index)");
    }

    auto index = static_cast<int32_t>(lua_tonumber(L, 1)) - 1;

    // Out of bounds

    if (index < 0 || index >= CCharacterSelection::s_characterList.Count()) {
        // Name
        lua_pushnil(L);

        // Race
        lua_pushnil(L);

        // Class
        lua_pushnil(L);

        // Level
        lua_pushnumber(L, 0.0);

        // Zone (why isn't this nil?)
        lua_pushnumber(L, 0.0);

        // Sex
        lua_pushnil(L);

        // Ghost
        lua_pushnil(L);

        // Paid class change
        lua_pushnil(L);

        // Paid race change
        lua_pushnil(L);

        // Paid faction change
        lua_pushnil(L);

        return 10;
    }

    // Character exists at index

    auto& display = CCharacterSelection::s_characterList[index];
    auto sex = static_cast<UNIT_SEX>(display.m_info.sexID);

    // Name
    lua_pushstring(L, display.m_info.name);

    // Race
    auto raceRec = g_chrRacesDB.GetRecord(display.m_info.raceID);
    auto raceName = CGUnit_C::GetDisplayRaceNameFromRecord(raceRec, sex, nullptr);
    lua_pushstring(L, raceName ? raceName : "");

    // Class
    auto classRec = g_chrClassesDB.GetRecord(display.m_info.classID);
    auto className = CGUnit_C::GetDisplayClassNameFromRecord(classRec, sex, nullptr);
    lua_pushstring(L, className ? className : "");

    // Level
    lua_pushnumber(L, display.m_info.experienceLevel);

    // Zone
    auto areaRec = g_areaTableDB.GetRecord(display.m_info.zoneID);
    if (areaRec) {
        lua_pushstring(L, areaRec->m_areaName);
    } else {
        lua_pushnil(L);
    }

    // Sex
    lua_pushnumber(L, g_glueFrameScriptGenders[display.m_info.sexID]);

    // Ghost
    lua_pushboolean(L, display.m_info.flags & 0x2000);

    // Paid class change
    lua_pushboolean(L, display.m_info.customizeFlags & 0x1);

    // Paid race change
    lua_pushboolean(L, display.m_info.customizeFlags & 0x100000);

    // Paid faction change
    lua_pushboolean(L, display.m_info.customizeFlags & 0x10000);

    return 10;
}

int32_t Script_SelectCharacter(lua_State* L) {
    if (!lua_isnumber(L, 1)) {
        return luaL_error(L, "Usage: SelectCharacter(index)");
    }

    int32_t index = static_cast<int32_t>(lua_tonumber(L, 1)) - 1;

    if (index < 0 || index >= CCharacterSelection::s_characterList.Count()) {
        index = 0;
    }

    CCharacterSelection::s_selectionIndex = index;
    CCharacterSelection::ShowCharacter();

    FrameScript_SignalEvent(8, "%d", CCharacterSelection::s_selectionIndex + 1);

    return 0;
}

int32_t Script_DeleteCharacter(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RenameCharacter(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DeclineCharacter(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UpdateSelectionCustomizationScene(lua_State* L) {
    CCharacterSelection::RenderPrep();

    return 0;
}

int32_t Script_GetCharacterSelectFacing(lua_State* L) {
    lua_pushnumber(L, CCharacterSelection::s_charFacing * 57.29578);

    return 1;
}

int32_t Script_SetCharacterSelectFacing(lua_State* L) {
    if (!lua_isnumber(L, 1)) {
        return luaL_error(L, "Usage: SetCharacterSelectFacing(degrees)");
    }

    float facing = lua_tonumber(L, 1) * 0.017453292;
    CCharacterSelection::SetFacing(facing);

    return 1;
}

int32_t Script_GetSelectBackgroundModel(lua_State* L) {
    if (!lua_isnumber(L, 1)) {
        return luaL_error(L, "Usage: GetSelectBackgroundModel(index)");
    }

    auto characterIndex = static_cast<int32_t>(lua_tonumber(L, 1)) - 1;

    if (SFile::IsStreamingTrial()) {
        lua_pushstring(L, "CharacterSelect");

        return 1;
    }

    // Out of bounds

    if (characterIndex < 0 || characterIndex >= CCharacterSelection::s_characterList.Count()) {
        auto defaultRaceRec = g_chrRacesDB.GetRecord(2);

        if (defaultRaceRec) {
            lua_pushstring(L, defaultRaceRec->m_clientFileString);
        } else {
            lua_pushstring(L, "");
        }

        return 1;
    }

    // Character exists at index

    auto character = CCharacterSelection::GetCharacterDisplay(characterIndex);

    // Class background

    auto classID = character->m_info.classID;

    if (classID == 6) {
        auto classRec = g_chrClassesDB.GetRecord(character->m_info.classID);

        if (classRec) {
            lua_pushstring(L, classRec->m_filename);
        } else {
            lua_pushstring(L, "");
        }

        return 1;
    }

    // Race background

    auto raceID = character->m_info.raceID;

    if (raceID == 7) {
        raceID = 3;
    } else if (raceID == 8) {
        raceID = 2;
    }

    auto raceRec = g_chrRacesDB.GetRecord(raceID);

    if (raceRec) {
        lua_pushstring(L, raceRec->m_clientFileString);
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
