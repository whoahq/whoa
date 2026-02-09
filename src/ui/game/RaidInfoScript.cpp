#include "ui/game/RaidInfoScript.hpp"
#include "ui/FrameScript.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t Script_GetNumRaidMembers(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRealNumRaidMembers(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRaidRosterInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetRaidRosterSelection(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRaidRosterSelection(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsRaidLeader(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsRealRaidLeader(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsRaidOfficer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetRaidSubgroup(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SwapRaidSubgroup(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ConvertToRaid(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PromoteToLeader(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PromoteToAssistant(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DemoteAssistant(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetRaidTarget(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRaidTargetIndex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DoReadyCheck(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ConfirmReadyCheck(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetReadyCheckTimeLeft(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetReadyCheckStatus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

static FrameScript_Method s_ScriptFunctions[] = {
    { "GetNumRaidMembers",          &Script_GetNumRaidMembers },
    { "GetRealNumRaidMembers",      &Script_GetRealNumRaidMembers },
    { "GetRaidRosterInfo",          &Script_GetRaidRosterInfo },
    { "SetRaidRosterSelection",     &Script_SetRaidRosterSelection },
    { "GetRaidRosterSelection",     &Script_GetRaidRosterSelection },
    { "IsRaidLeader",               &Script_IsRaidLeader },
    { "IsRealRaidLeader",           &Script_IsRealRaidLeader },
    { "IsRaidOfficer",              &Script_IsRaidOfficer },
    { "SetRaidSubgroup",            &Script_SetRaidSubgroup },
    { "SwapRaidSubgroup",           &Script_SwapRaidSubgroup },
    { "ConvertToRaid",              &Script_ConvertToRaid },
    { "PromoteToLeader",            &Script_PromoteToLeader },
    { "PromoteToAssistant",         &Script_PromoteToAssistant },
    { "DemoteAssistant",            &Script_DemoteAssistant },
    { "SetRaidTarget",              &Script_SetRaidTarget },
    { "GetRaidTargetIndex",         &Script_GetRaidTargetIndex },
    { "DoReadyCheck",               &Script_DoReadyCheck },
    { "ConfirmReadyCheck",          &Script_ConfirmReadyCheck },
    { "GetReadyCheckTimeLeft",      &Script_GetReadyCheckTimeLeft },
    { "GetReadyCheckStatus",        &Script_GetReadyCheckStatus },
};

void RaidInfoRegisterScriptFunctions() {
    for (auto& func : s_ScriptFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
    }
}
