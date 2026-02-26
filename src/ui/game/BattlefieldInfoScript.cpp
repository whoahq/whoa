#include "ui/game/BattlefieldInfoScript.hpp"
#include "ui/game/CGBattlefieldInfo.hpp"
#include "ui/FrameScript.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t Script_GetNumBattlefields(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldInstanceInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsBattlefieldArena(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsActiveBattlefieldArena(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_JoinBattlefield(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetSelectedBattlefield(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSelectedBattlefield(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptBattlefieldPort(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldStatus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldPortExpiration(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldInstanceExpiration(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldInstanceRunTime(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldEstimatedWaitTime(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldTimeWaited(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CloseBattlefield(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RequestBattlefieldScoreData(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumBattlefieldScores(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldScore(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldWinner(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetBattlefieldScoreFaction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_LeaveBattlefield(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumBattlefieldStats(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldStatInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldStatData(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RequestBattlefieldPositions(lua_State* L) {
    CGBattlefieldInfo::RequestPlayerPositions();

    return 0;
}

int32_t Script_GetNumBattlefieldPositions(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldPosition(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumBattlefieldFlagPositions(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldFlagPosition(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumBattlefieldVehicles(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldVehicleInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CanJoinBattlefieldAsGroup(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldMapIconScale(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldTeamInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlefieldArenaFaction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SortBattlefieldScoreData(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_HearthAndResurrectFromArea(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CanHearthAndResurrectFromArea(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumBattlegroundTypes(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBattlegroundInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RequestBattlegroundInstanceInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumArenaOpponents(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BattlefieldMgrEntryInviteResponse(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BattlefieldMgrQueueRequest(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BattlefieldMgrQueueInviteResponse(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BattlefieldMgrExitRequest(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetWorldPVPQueueStatus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetHolidayBGHonorCurrencyBonuses(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRandomBGHonorCurrencyBonuses(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SortBGList(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

static FrameScript_Method s_ScriptFunctions[] = {
    { "GetNumBattlefields",                 &Script_GetNumBattlefields },
    { "GetBattlefieldInfo",                 &Script_GetBattlefieldInfo },
    { "GetBattlefieldInstanceInfo",         &Script_GetBattlefieldInstanceInfo },
    { "IsBattlefieldArena",                 &Script_IsBattlefieldArena },
    { "IsActiveBattlefieldArena",           &Script_IsActiveBattlefieldArena },
    { "JoinBattlefield",                    &Script_JoinBattlefield },
    { "SetSelectedBattlefield",             &Script_SetSelectedBattlefield },
    { "GetSelectedBattlefield",             &Script_GetSelectedBattlefield },
    { "AcceptBattlefieldPort",              &Script_AcceptBattlefieldPort },
    { "GetBattlefieldStatus",               &Script_GetBattlefieldStatus },
    { "GetBattlefieldPortExpiration",       &Script_GetBattlefieldPortExpiration },
    { "GetBattlefieldInstanceExpiration",   &Script_GetBattlefieldInstanceExpiration },
    { "GetBattlefieldInstanceRunTime",      &Script_GetBattlefieldInstanceRunTime },
    { "GetBattlefieldEstimatedWaitTime",    &Script_GetBattlefieldEstimatedWaitTime },
    { "GetBattlefieldTimeWaited",           &Script_GetBattlefieldTimeWaited },
    { "CloseBattlefield",                   &Script_CloseBattlefield },
    { "RequestBattlefieldScoreData",        &Script_RequestBattlefieldScoreData },
    { "GetNumBattlefieldScores",            &Script_GetNumBattlefieldScores },
    { "GetBattlefieldScore",                &Script_GetBattlefieldScore },
    { "GetBattlefieldWinner",               &Script_GetBattlefieldWinner },
    { "SetBattlefieldScoreFaction",         &Script_SetBattlefieldScoreFaction },
    { "LeaveBattlefield",                   &Script_LeaveBattlefield },
    { "GetNumBattlefieldStats",             &Script_GetNumBattlefieldStats },
    { "GetBattlefieldStatInfo",             &Script_GetBattlefieldStatInfo },
    { "GetBattlefieldStatData",             &Script_GetBattlefieldStatData },
    { "RequestBattlefieldPositions",        &Script_RequestBattlefieldPositions },
    { "GetNumBattlefieldPositions",         &Script_GetNumBattlefieldPositions },
    { "GetBattlefieldPosition",             &Script_GetBattlefieldPosition },
    { "GetNumBattlefieldFlagPositions",     &Script_GetNumBattlefieldFlagPositions },
    { "GetBattlefieldFlagPosition",         &Script_GetBattlefieldFlagPosition },
    { "GetNumBattlefieldVehicles",          &Script_GetNumBattlefieldVehicles },
    { "GetBattlefieldVehicleInfo",          &Script_GetBattlefieldVehicleInfo },
    { "CanJoinBattlefieldAsGroup",          &Script_CanJoinBattlefieldAsGroup },
    { "GetBattlefieldMapIconScale",         &Script_GetBattlefieldMapIconScale },
    { "GetBattlefieldTeamInfo",             &Script_GetBattlefieldTeamInfo },
    { "GetBattlefieldArenaFaction",         &Script_GetBattlefieldArenaFaction },
    { "SortBattlefieldScoreData",           &Script_SortBattlefieldScoreData },
    { "HearthAndResurrectFromArea",         &Script_HearthAndResurrectFromArea },
    { "CanHearthAndResurrectFromArea",      &Script_CanHearthAndResurrectFromArea },
    { "GetNumBattlegroundTypes",            &Script_GetNumBattlegroundTypes },
    { "GetBattlegroundInfo",                &Script_GetBattlegroundInfo },
    { "RequestBattlegroundInstanceInfo",    &Script_RequestBattlegroundInstanceInfo },
    { "GetNumArenaOpponents",               &Script_GetNumArenaOpponents },
    { "BattlefieldMgrEntryInviteResponse",  &Script_BattlefieldMgrEntryInviteResponse },
    { "BattlefieldMgrQueueRequest",         &Script_BattlefieldMgrQueueRequest },
    { "BattlefieldMgrQueueInviteResponse",  &Script_BattlefieldMgrQueueInviteResponse },
    { "BattlefieldMgrExitRequest",          &Script_BattlefieldMgrExitRequest },
    { "GetWorldPVPQueueStatus",             &Script_GetWorldPVPQueueStatus },
    { "GetHolidayBGHonorCurrencyBonuses",   &Script_GetHolidayBGHonorCurrencyBonuses },
    { "GetRandomBGHonorCurrencyBonuses",    &Script_GetRandomBGHonorCurrencyBonuses },
    { "SortBGList",                         &Script_SortBGList },
};

void BattlefieldInfoRegisterScriptFunctions() {
    for (auto& func : s_ScriptFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
    }
}
