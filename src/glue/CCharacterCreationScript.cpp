#include "glue/CCharacterCreationScript.hpp"
#include "component/CCharacterComponent.hpp"
#include "db/Db.hpp"
#include "glue/CCharacterCreation.hpp"
#include "object/client/Unit_C.hpp"
#include "ui/FrameScript.hpp"
#include "ui/Types.hpp"
#include "ui/game/CGLookingForGroup.hpp"
#include "ui/simple/CSimpleModelFFX.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t Script_SetCharCustomizeFrame(lua_State* L) {
    if (!lua_isstring(L, 1)) {
        luaL_error(L, "Usage: SetCharCustomizeFrame(\"frameName\")");
        return 0;
    }

    auto type = CSimpleModel::GetObjectType();
    auto frame = static_cast<CSimpleModelFFX*>(CScriptObject::GetScriptObjectByName(lua_tostring(L, 1), type));

    if (frame) {
        CCharacterCreation::s_charCustomizeFrame = frame;
    }

    return 0;
}

int32_t Script_SetCharCustomizeBackground(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ResetCharCustomize(lua_State* L) {
    CCharacterCreation::ResetCharCustomizeInfo();

    return 0;
}

int32_t Script_GetNameForRace(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetFactionForRace(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetAvailableRaces(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetAvailableClasses(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetClassesForRace(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetHairCustomization(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetFacialHairCustomization(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSelectedRace(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSelectedSex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSelectedClass(lua_State* L) {
    auto classRec = g_chrClassesDB.GetRecord(CCharacterCreation::s_selectedClassID);

    if (!classRec) {
        return 0;
    }

    auto displayName = CGUnit_C::GetDisplayClassNameFromRecord(
        classRec,
        static_cast<UNIT_SEX>(CCharacterCreation::s_character->m_data.sexID),
        nullptr
    );

    lua_pushstring(L, displayName);
    lua_pushstring(L, classRec->m_filename);

    int32_t classIndex = 0;

    for (auto i = 0; i < g_chrClassesDB.GetNumRecords(); i++) {
        auto rec = g_chrClassesDB.GetRecordByIndex(i);
        if (rec->m_ID == CCharacterCreation::s_selectedClassID) {
            classIndex = i;
        }
    }

    lua_pushnumber(L, classIndex + 1);

    auto classRoles = CGLookingForGroup::GetClassRoles(CCharacterCreation::s_selectedClassID);

    lua_pushboolean(L, classRoles & CLASS_ROLE_TANK);
    lua_pushboolean(L, classRoles & CLASS_ROLE_HEALER);
    lua_pushboolean(L, classRoles & CLASS_ROLE_DAMAGE);

    return 6;
}

int32_t Script_SetSelectedRace(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetSelectedSex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetSelectedClass(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UpdateCustomizationBackground(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UpdateCustomizationScene(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CycleCharCustomization(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RandomizeCharCustomization(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCharacterCreateFacing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetCharacterCreateFacing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRandomName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CreateCharacter(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CustomizeExistingCharacter(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PaidChange_GetPreviousRaceIndex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PaidChange_GetCurrentRaceIndex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PaidChange_GetCurrentClassIndex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PaidChange_GetName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsRaceClassValid(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsRaceClassRestricted(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCreateBackgroundModel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

static FrameScript_Method s_ScriptFunctions[NUM_SCRIPT_FUNCTIONS_CHAR_CREATE] = {
    { "SetCharCustomizeFrame",          &Script_SetCharCustomizeFrame },
    { "SetCharCustomizeBackground",     &Script_SetCharCustomizeBackground },
    { "ResetCharCustomize",             &Script_ResetCharCustomize },
    { "GetNameForRace",                 &Script_GetNameForRace },
    { "GetFactionForRace",              &Script_GetFactionForRace },
    { "GetAvailableRaces",              &Script_GetAvailableRaces },
    { "GetAvailableClasses",            &Script_GetAvailableClasses },
    { "GetClassesForRace",              &Script_GetClassesForRace },
    { "GetHairCustomization",           &Script_GetHairCustomization },
    { "GetFacialHairCustomization",     &Script_GetFacialHairCustomization },
    { "GetSelectedRace",                &Script_GetSelectedRace },
    { "GetSelectedSex",                 &Script_GetSelectedSex },
    { "GetSelectedClass",               &Script_GetSelectedClass },
    { "SetSelectedRace",                &Script_SetSelectedRace },
    { "SetSelectedSex",                 &Script_SetSelectedSex },
    { "SetSelectedClass",               &Script_SetSelectedClass },
    { "UpdateCustomizationBackground",  &Script_UpdateCustomizationBackground },
    { "UpdateCustomizationScene",       &Script_UpdateCustomizationScene },
    { "CycleCharCustomization",         &Script_CycleCharCustomization },
    { "RandomizeCharCustomization",     &Script_RandomizeCharCustomization },
    { "GetCharacterCreateFacing",       &Script_GetCharacterCreateFacing },
    { "SetCharacterCreateFacing",       &Script_SetCharacterCreateFacing },
    { "GetRandomName",                  &Script_GetRandomName },
    { "CreateCharacter",                &Script_CreateCharacter },
    { "CustomizeExistingCharacter",     &Script_CustomizeExistingCharacter },
    { "PaidChange_GetPreviousRaceIndex", &Script_PaidChange_GetPreviousRaceIndex },
    { "PaidChange_GetCurrentRaceIndex", &Script_PaidChange_GetCurrentRaceIndex },
    { "PaidChange_GetCurrentClassIndex", &Script_PaidChange_GetCurrentClassIndex },
    { "PaidChange_GetName",             &Script_PaidChange_GetName },
    { "IsRaceClassValid",               &Script_IsRaceClassValid },
    { "IsRaceClassRestricted",          &Script_IsRaceClassRestricted },
    { "GetCreateBackgroundModel",       &Script_GetCreateBackgroundModel }
};

void CharacterCreateRegisterScriptFunctions() {
    for (int32_t i = 0; i < NUM_SCRIPT_FUNCTIONS_CHAR_CREATE; ++i) {
        FrameScript_RegisterFunction(
            s_ScriptFunctions[i].name,
            s_ScriptFunctions[i].method
        );
    }
}
