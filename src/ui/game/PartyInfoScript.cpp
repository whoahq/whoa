#include "ui/game/PartyInfoScript.hpp"
#include "ui/FrameScript.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t Script_GetNumPartyMembers(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRealNumPartyMembers(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPartyMember(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPartyLeaderIndex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsPartyLeader(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsRealPartyLeader(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_LeaveParty(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetLootMethod(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetLootMethod(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetLootThreshold(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetLootThreshold(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetPartyAssignment(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ClearPartyAssignment(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPartyAssignment(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SilenceMember(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnSilenceMember(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetOptOutOfLoot(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetOptOutOfLoot(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CanChangePlayerDifficulty(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ChangePlayerDifficulty(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsPartyLFG(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_HasLFGRestrictions(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

static FrameScript_Method s_ScriptFunctions[] = {
    { "GetNumPartyMembers",         &Script_GetNumPartyMembers },
    { "GetRealNumPartyMembers",     &Script_GetRealNumPartyMembers },
    { "GetPartyMember",             &Script_GetPartyMember },
    { "GetPartyLeaderIndex",        &Script_GetPartyLeaderIndex },
    { "IsPartyLeader",              &Script_IsPartyLeader },
    { "IsRealPartyLeader",          &Script_IsRealPartyLeader },
    { "LeaveParty",                 &Script_LeaveParty },
    { "GetLootMethod",              &Script_GetLootMethod },
    { "SetLootMethod",              &Script_SetLootMethod },
    { "GetLootThreshold",           &Script_GetLootThreshold },
    { "SetLootThreshold",           &Script_SetLootThreshold },
    { "SetPartyAssignment",         &Script_SetPartyAssignment },
    { "ClearPartyAssignment",       &Script_ClearPartyAssignment },
    { "GetPartyAssignment",         &Script_GetPartyAssignment },
    { "SilenceMember",              &Script_SilenceMember },
    { "UnSilenceMember",            &Script_UnSilenceMember },
    { "SetOptOutOfLoot",            &Script_SetOptOutOfLoot },
    { "GetOptOutOfLoot",            &Script_GetOptOutOfLoot },
    { "CanChangePlayerDifficulty",  &Script_CanChangePlayerDifficulty },
    { "ChangePlayerDifficulty",     &Script_ChangePlayerDifficulty },
    { "IsPartyLFG",                 &Script_IsPartyLFG },
    { "HasLFGRestrictions",         &Script_HasLFGRestrictions },
};

void PartyInfoRegisterScriptFunctions() {
    for (auto& func : s_ScriptFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
    }
}
