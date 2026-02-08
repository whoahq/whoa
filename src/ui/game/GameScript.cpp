#include "ui/game/GameScript.hpp"
#include "console/CVar.hpp"
#include "gx/Coordinate.hpp"
#include "ui/FrameScript.hpp"
#include "ui/ScriptFunctionsShared.hpp"
#include "ui/game/CGGameUI.hpp"
#include "ui/game/Types.hpp"
#include "ui/simple/CSimpleTop.hpp"
#include "util/StringTo.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t Script_FrameXML_Debug(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBuildInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ReloadUI(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RegisterForSave(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RegisterForSavePerCharacter(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetLayoutMode(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsModifierKeyDown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsLeftShiftKeyDown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsRightShiftKeyDown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsShiftKeyDown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsLeftControlKeyDown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsRightControlKeyDown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsControlKeyDown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsLeftAltKeyDown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsRightAltKeyDown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsAltKeyDown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsMouseButtonDown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMouseButtonName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMouseButtonClicked(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetConsoleKey(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Screenshot(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetFramerate(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TogglePerformanceDisplay(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TogglePerformancePause(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TogglePerformanceValues(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ResetPerformanceValues(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetDebugStats(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsDebugBuild(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RegisterCVar(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCVarInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetCVar(lua_State* L) {
    if (!lua_isstring(L, 1)) {
        luaL_error(L, "Usage: SetCVar(\"cvar\", value [, \"scriptCvar\")");
        return 0;
    }

    auto varName = lua_tostring(L, 1);
    auto var = CVar::LookupRegistered(varName);

    if (!var || (var->m_flags & 0x40)) {
        luaL_error(L, "Couldn't find CVar named '%s'", varName);
        return 0;
    }

    if (var->m_flags & 0x4 || var->m_flags & 0x100) {
        luaL_error(L, "\"%s\" is read-only", varName);
        return 0;
    }

    if (!(var->m_flags & 0x8)/* TODO || CSimpleTop::GetInstance()->dword124C */) {
        auto value = lua_tostring(L, 2);
        if (!value) {
            value = "0";
        }

        var->Set(value, true, false, false, true);

        if (lua_isstring(L, 3)) {
            auto scriptVarName = lua_tostring(L, 3);
            FrameScript_SignalEvent(SCRIPT_CVAR_UPDATE, "%s%s", scriptVarName, value);
        }
    } else {
        // TODO CGGameUI::ShowBlockedActionFeedback(nullptr, 2);
    }

    return 0;
}

int32_t Script_GetCVar(lua_State* L) {
    if (!lua_isstring(L, 1)) {
        luaL_error(L, "Usage: GetCVar(\"cvar\")");
        return 0;
    }

    auto varName = lua_tostring(L, 1);
    auto var = CVar::LookupRegistered(varName);

    if (var && !(var->m_flags & 0x40)) {
        lua_pushstring(L, var->GetString());
    } else {
        lua_pushnil(L);
    }

    return 1;
}

int32_t Script_GetCVarBool(lua_State* L) {
    if (!lua_isstring(L, 1)) {
        luaL_error(L, "Usage: GetCVarBool(\"cvar\")");
        return 0;
    }

    auto varName = lua_tostring(L, 1);
    auto var = CVar::LookupRegistered(varName);

    if (var && !(var->m_flags & 0x40) && StringToBOOL(var->GetString())) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

int32_t Script_GetCVarDefault(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCVarMin(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCVarMax(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCVarAbsoluteMin(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCVarAbsoluteMax(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetWaterDetail(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetWaterDetail(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetFarclip(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetFarclip(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetTexLodBias(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetTexLodBias(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetBaseMip(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBaseMip(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ToggleTris(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TogglePortals(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ToggleCollision(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ToggleCollisionDisplay(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TogglePlayerBounds(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Stuck(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Logout(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Quit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetCursor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ResetCursor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ClearCursor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CursorHasItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CursorHasSpell(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CursorHasMacro(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CursorHasMoney(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCursorInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_EquipCursorItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DeleteCursorItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_EquipPendingItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CancelPendingEquip(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetUnit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetNearest(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetNearestEnemy(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetNearestEnemyPlayer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetNearestFriend(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetNearestFriendPlayer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetNearestPartyMember(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetNearestRaidMember(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetDirectionEnemy(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetDirectionFriend(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetDirectionFinished(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetLastTarget(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetLastEnemy(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetLastFriend(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AttackTarget(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AssistUnit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_FocusUnit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_FollowUnit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_InteractUnit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ClearTarget(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ClearFocus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AutoEquipCursorItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ToggleSheath(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetZoneText(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRealZoneText(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSubZoneText(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMinimapZoneText(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_InitiateTrade(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CanInspect(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_NotifyInspect(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_InviteUnit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UninviteUnit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RequestTimePlayed(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RepopMe(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptResurrect(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DeclineResurrect(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ResurrectGetOfferer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ResurrectHasSickness(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ResurrectHasTimer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BeginTrade(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CancelTrade(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptGroup(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DeclineGroup(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptGuild(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DeclineGuild(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptArenaTeam(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DeclineArenaTeam(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CancelLogout(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ForceLogout(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ForceQuit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCursorMoney(lua_State* L) {
    lua_pushnumber(L, CGGameUI::GetCursorMoney());

    return 1;
}

int32_t Script_DropCursorMoney(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PickupPlayerMoney(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_HasSoulstone(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UseSoulstone(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_HasKey(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GuildInvite(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GuildUninvite(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GuildPromote(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GuildDemote(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GuildSetLeader(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GuildSetMOTD(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GuildLeave(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GuildDisband(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GuildInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ArenaTeamInviteByName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ArenaTeamLeave(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ArenaTeamUninviteByName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ArenaTeamSetLeaderByName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ArenaTeamDisband(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetScreenWidth(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetScreenHeight(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetDamageBonusStat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetReleaseTimeRemaining(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCorpseRecoveryDelay(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInstanceBootTimeRemaining(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInstanceLockTimeRemaining(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInstanceLockTimeRemainingEncounter(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSummonConfirmTimeLeft(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSummonConfirmSummoner(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSummonConfirmAreaName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ConfirmSummon(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CancelSummon(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCursorPosition(lua_State* L) {
    STORM_ASSERT(CSimpleTop::s_instance);

    float ddcX = 0.0f;
    float ddcY = 0.0f;

    NDCToDDC(
        CSimpleTop::s_instance->m_mousePosition.x,
        CSimpleTop::s_instance->m_mousePosition.y,
        &ddcX,
        &ddcY
    );

    float ndcX = DDCToNDCWidth(CoordinateGetAspectCompensation() * 1024.0f * ddcX);
    lua_pushnumber(L, ndcX);

    float ndcY = DDCToNDCWidth(CoordinateGetAspectCompensation() * 1024.0f * ddcY);
    lua_pushnumber(L, ndcY);

    return 2;
}

int32_t Script_GetNetStats(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SitStandOrDescendStart(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_StopCinematic(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RunScript(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CheckInteractDistance(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RandomRoll(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_OpeningCinematic(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_InCinematic(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptXPLoss(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CheckSpiritHealerDist(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CheckTalentMasterDist(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CheckBinderDist(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RetrieveCorpse(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_BindEnchant(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ReplaceEnchant(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ReplaceTradeEnchant(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_NotWhileDeadError(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRestState(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetXPExhaustion(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetTimeToWellRested(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GMRequestPlayerInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCoinIcon(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCoinText(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCoinTextureString(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsSubZonePVPPOI(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetZonePVPInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TogglePVP(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetPVP(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPVPDesired(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPVPTimer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsPVPTimerRunning(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ConfirmBindOnUse(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetPortraitToTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetLocale(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetGMTicketCategories(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DropItemOnUnit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RestartGx(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RestoreVideoResolutionDefaults(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RestoreVideoEffectsDefaults(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RestoreVideoStereoDefaults(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBindLocation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ConfirmTalentWipe(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ConfirmBinder(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ShowingHelm(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ShowingCloak(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ShowHelm(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ShowCloak(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetEuropeanNumbers(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetAreaSpiritHealerTime(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptAreaSpiritHeal(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CancelAreaSpiritHeal(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMouseFocus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRealmName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetItemQualityColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetItemInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetItemGem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetExtendedItemInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetItemIcon(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetItemFamily(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetItemCount(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetItemSpell(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetItemCooldown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PickupItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsCurrentItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsUsableItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsHelpfulItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsHarmfulItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsConsumableItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsEquippableItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsEquippedItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsEquippedItemType(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsDressableItem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ItemHasRange(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsItemInRange(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumAddOns(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetAddOnInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetAddOnMetadata(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UpdateAddOnMemoryUsage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetAddOnMemoryUsage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetScriptCPUUsage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UpdateAddOnCPUUsage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetAddOnCPUUsage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetFunctionCPUUsage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetFrameCPUUsage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetEventCPUUsage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ResetCPUUsage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetAddOnDependencies(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_EnableAddOn(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_EnableAllAddOns(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DisableAddOn(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DisableAllAddOns(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ResetDisabledAddOns(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsAddOnLoadOnDemand(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsAddOnLoaded(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_LoadAddOn(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PartialPlayTime(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_NoPlayTime(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBillingTimeRested(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CanShowResetInstances(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ResetInstances(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsInInstance(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInstanceDifficulty(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetInstanceInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetDungeonDifficulty(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetDungeonDifficulty(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRaidDifficulty(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetRaidDifficulty(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ReportBug(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ReportSuggestion(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMirrorTimerInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMirrorTimerProgress(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumTitles(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCurrentTitle(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetCurrentTitle(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsTitleKnown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetTitleName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UseItemByName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_EquipItemByName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetExistingLocales(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_InCombatLockdown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_StartAttack(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_StopAttack(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetTaxiBenchmarkMode(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetTaxiBenchmarkMode(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Dismount(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_VoicePushToTalkStart(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_VoicePushToTalkStop(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetUIVisibility(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsReferAFriendLinked(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CanGrantLevel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GrantLevel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CanSummonFriend(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SummonFriend(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSummonFriendCooldown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetTotemInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetTotemTimeLeft(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TargetTotem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DestroyTotem(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumDeclensionSets(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DeclineName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptLevelGrant(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DeclineLevelGrant(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UploadSettings(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DownloadSettings(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMovieResolution(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GameMovieFinished(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsDesaturateSupported(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetThreatStatusColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsThreatWarningEnabled(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ConsoleAddMessage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetItemUniqueness(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_EndRefund(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_EndBoundTradeable(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CanMapChangeDifficulty(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetExpansionLevel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetAllowLowLevelRaid(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetAllowLowLevelRaid(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

static FrameScript_Method s_ScriptFunctions[] = {
    { "FrameXML_Debug",                 &Script_FrameXML_Debug },
    { "GetBuildInfo",                   &Script_GetBuildInfo },
    { "ReloadUI",                       &Script_ReloadUI },
    { "RegisterForSave",                &Script_RegisterForSave },
    { "RegisterForSavePerCharacter",    &Script_RegisterForSavePerCharacter },
    { "SetLayoutMode",                  &Script_SetLayoutMode },
    { "IsModifierKeyDown",              &Script_IsModifierKeyDown },
    { "IsLeftShiftKeyDown",             &Script_IsLeftShiftKeyDown },
    { "IsRightShiftKeyDown",            &Script_IsRightShiftKeyDown },
    { "IsShiftKeyDown",                 &Script_IsShiftKeyDown },
    { "IsLeftControlKeyDown",           &Script_IsLeftControlKeyDown },
    { "IsRightControlKeyDown",          &Script_IsRightControlKeyDown },
    { "IsControlKeyDown",               &Script_IsControlKeyDown },
    { "IsLeftAltKeyDown",               &Script_IsLeftAltKeyDown },
    { "IsRightAltKeyDown",              &Script_IsRightAltKeyDown },
    { "IsAltKeyDown",                   &Script_IsAltKeyDown },
    { "IsMouseButtonDown",              &Script_IsMouseButtonDown },
    { "GetMouseButtonName",             &Script_GetMouseButtonName },
    { "GetMouseButtonClicked",          &Script_GetMouseButtonClicked },
    { "SetConsoleKey",                  &Script_SetConsoleKey },
    { "Screenshot",                     &Script_Screenshot },
    { "GetFramerate",                   &Script_GetFramerate },
    { "TogglePerformanceDisplay",       &Script_TogglePerformanceDisplay },
    { "TogglePerformancePause",         &Script_TogglePerformancePause },
    { "TogglePerformanceValues",        &Script_TogglePerformanceValues },
    { "ResetPerformanceValues",         &Script_ResetPerformanceValues },
    { "GetDebugStats",                  &Script_GetDebugStats },
    { "IsDebugBuild",                   &Script_IsDebugBuild },
    { "RegisterCVar",                   &Script_RegisterCVar },
    { "GetCVarInfo",                    &Script_GetCVarInfo },
    { "SetCVar",                        &Script_SetCVar },
    { "GetCVar",                        &Script_GetCVar },
    { "GetCVarBool",                    &Script_GetCVarBool },
    { "GetCVarDefault",                 &Script_GetCVarDefault },
    { "GetCVarMin",                     &Script_GetCVarMin },
    { "GetCVarMax",                     &Script_GetCVarMax },
    { "GetCVarAbsoluteMin",             &Script_GetCVarAbsoluteMin },
    { "GetCVarAbsoluteMax",             &Script_GetCVarAbsoluteMax },
    { "GetWaterDetail",                 &Script_GetWaterDetail },
    { "SetWaterDetail",                 &Script_SetWaterDetail },
    { "GetFarclip",                     &Script_GetFarclip },
    { "SetFarclip",                     &Script_SetFarclip },
    { "GetTexLodBias",                  &Script_GetTexLodBias },
    { "SetTexLodBias",                  &Script_SetTexLodBias },
    { "SetBaseMip",                     &Script_SetBaseMip },
    { "GetBaseMip",                     &Script_GetBaseMip },
    { "ToggleTris",                     &Script_ToggleTris },
    { "TogglePortals",                  &Script_TogglePortals },
    { "ToggleCollision",                &Script_ToggleCollision },
    { "ToggleCollisionDisplay",         &Script_ToggleCollisionDisplay },
    { "TogglePlayerBounds",             &Script_TogglePlayerBounds },
    { "Stuck",                          &Script_Stuck },
    { "Logout",                         &Script_Logout },
    { "Quit",                           &Script_Quit },
    { "SetCursor",                      &Script_SetCursor },
    { "ResetCursor",                    &Script_ResetCursor },
    { "ClearCursor",                    &Script_ClearCursor },
    { "CursorHasItem",                  &Script_CursorHasItem },
    { "CursorHasSpell",                 &Script_CursorHasSpell },
    { "CursorHasMacro",                 &Script_CursorHasMacro },
    { "CursorHasMoney",                 &Script_CursorHasMoney },
    { "GetCursorInfo",                  &Script_GetCursorInfo },
    { "EquipCursorItem",                &Script_EquipCursorItem },
    { "DeleteCursorItem",               &Script_DeleteCursorItem },
    { "EquipPendingItem",               &Script_EquipPendingItem },
    { "CancelPendingEquip",             &Script_CancelPendingEquip },
    { "TargetUnit",                     &Script_TargetUnit },
    { "TargetNearest",                  &Script_TargetNearest },
    { "TargetNearestEnemy",             &Script_TargetNearestEnemy },
    { "TargetNearestEnemyPlayer",       &Script_TargetNearestEnemyPlayer },
    { "TargetNearestFriend",            &Script_TargetNearestFriend },
    { "TargetNearestFriendPlayer",      &Script_TargetNearestFriendPlayer },
    { "TargetNearestPartyMember",       &Script_TargetNearestPartyMember },
    { "TargetNearestRaidMember",        &Script_TargetNearestRaidMember },
    { "TargetDirectionEnemy",           &Script_TargetDirectionEnemy },
    { "TargetDirectionFriend",          &Script_TargetDirectionFriend },
    { "TargetDirectionFinished",        &Script_TargetDirectionFinished },
    { "TargetLastTarget",               &Script_TargetLastTarget },
    { "TargetLastEnemy",                &Script_TargetLastEnemy },
    { "TargetLastFriend",               &Script_TargetLastFriend },
    { "AttackTarget",                   &Script_AttackTarget },
    { "AssistUnit",                     &Script_AssistUnit },
    { "FocusUnit",                      &Script_FocusUnit },
    { "FollowUnit",                     &Script_FollowUnit },
    { "InteractUnit",                   &Script_InteractUnit },
    { "ClearTarget",                    &Script_ClearTarget },
    { "ClearFocus",                     &Script_ClearFocus },
    { "AutoEquipCursorItem",            &Script_AutoEquipCursorItem },
    { "ToggleSheath",                   &Script_ToggleSheath },
    { "GetZoneText",                    &Script_GetZoneText },
    { "GetRealZoneText",                &Script_GetRealZoneText },
    { "GetSubZoneText",                 &Script_GetSubZoneText },
    { "GetMinimapZoneText",             &Script_GetMinimapZoneText },
    { "InitiateTrade",                  &Script_InitiateTrade },
    { "CanInspect",                     &Script_CanInspect },
    { "NotifyInspect",                  &Script_NotifyInspect },
    { "InviteUnit",                     &Script_InviteUnit },
    { "UninviteUnit",                   &Script_UninviteUnit },
    { "RequestTimePlayed",              &Script_RequestTimePlayed },
    { "RepopMe",                        &Script_RepopMe },
    { "AcceptResurrect",                &Script_AcceptResurrect },
    { "DeclineResurrect",               &Script_DeclineResurrect },
    { "ResurrectGetOfferer",            &Script_ResurrectGetOfferer },
    { "ResurrectHasSickness",           &Script_ResurrectHasSickness },
    { "ResurrectHasTimer",              &Script_ResurrectHasTimer },
    { "BeginTrade",                     &Script_BeginTrade },
    { "CancelTrade",                    &Script_CancelTrade },
    { "AcceptGroup",                    &Script_AcceptGroup },
    { "DeclineGroup",                   &Script_DeclineGroup },
    { "AcceptGuild",                    &Script_AcceptGuild },
    { "DeclineGuild",                   &Script_DeclineGuild },
    { "AcceptArenaTeam",                &Script_AcceptArenaTeam },
    { "DeclineArenaTeam",               &Script_DeclineArenaTeam },
    { "CancelLogout",                   &Script_CancelLogout },
    { "ForceLogout",                    &Script_ForceLogout },
    { "ForceQuit",                      &Script_ForceQuit },
    { "GetCursorMoney",                 &Script_GetCursorMoney },
    { "DropCursorMoney",                &Script_DropCursorMoney },
    { "PickupPlayerMoney",              &Script_PickupPlayerMoney },
    { "HasSoulstone",                   &Script_HasSoulstone },
    { "UseSoulstone",                   &Script_UseSoulstone },
    { "HasKey",                         &Script_HasKey },
    { "GuildInvite",                    &Script_GuildInvite },
    { "GuildUninvite",                  &Script_GuildUninvite },
    { "GuildPromote",                   &Script_GuildPromote },
    { "GuildDemote",                    &Script_GuildDemote },
    { "GuildSetLeader",                 &Script_GuildSetLeader },
    { "GuildSetMOTD",                   &Script_GuildSetMOTD },
    { "GuildLeave",                     &Script_GuildLeave },
    { "GuildDisband",                   &Script_GuildDisband },
    { "GuildInfo",                      &Script_GuildInfo },
    { "ArenaTeamInviteByName",          &Script_ArenaTeamInviteByName },
    { "ArenaTeamLeave",                 &Script_ArenaTeamLeave },
    { "ArenaTeamUninviteByName",        &Script_ArenaTeamUninviteByName },
    { "ArenaTeamSetLeaderByName",       &Script_ArenaTeamSetLeaderByName },
    { "ArenaTeamDisband",               &Script_ArenaTeamDisband },
    { "GetScreenWidth",                 &Script_GetScreenWidth },
    { "GetScreenHeight",                &Script_GetScreenHeight },
    { "GetDamageBonusStat",             &Script_GetDamageBonusStat },
    { "GetReleaseTimeRemaining",        &Script_GetReleaseTimeRemaining },
    { "GetCorpseRecoveryDelay",         &Script_GetCorpseRecoveryDelay },
    { "GetInstanceBootTimeRemaining",   &Script_GetInstanceBootTimeRemaining },
    { "GetInstanceLockTimeRemaining",   &Script_GetInstanceLockTimeRemaining },
    { "GetInstanceLockTimeRemainingEncounter", &Script_GetInstanceLockTimeRemainingEncounter },
    { "GetSummonConfirmTimeLeft",       &Script_GetSummonConfirmTimeLeft },
    { "GetSummonConfirmSummoner",       &Script_GetSummonConfirmSummoner },
    { "GetSummonConfirmAreaName",       &Script_GetSummonConfirmAreaName },
    { "ConfirmSummon",                  &Script_ConfirmSummon },
    { "CancelSummon",                   &Script_CancelSummon },
    { "GetCursorPosition",              &Script_GetCursorPosition },
    { "GetNetStats",                    &Script_GetNetStats },
    { "SitStandOrDescendStart",         &Script_SitStandOrDescendStart },
    { "StopCinematic",                  &Script_StopCinematic },
    { "RunScript",                      &Script_RunScript },
    { "CheckInteractDistance",          &Script_CheckInteractDistance },
    { "RandomRoll",                     &Script_RandomRoll },
    { "OpeningCinematic",               &Script_OpeningCinematic },
    { "InCinematic",                    &Script_InCinematic },
    { "IsWindowsClient",                &Script_IsWindowsClient },
    { "IsMacClient",                    &Script_IsMacClient },
    { "IsLinuxClient",                  &Script_IsLinuxClient },
    { "AcceptXPLoss",                   &Script_AcceptXPLoss },
    { "CheckSpiritHealerDist",          &Script_CheckSpiritHealerDist },
    { "CheckTalentMasterDist",          &Script_CheckTalentMasterDist },
    { "CheckBinderDist",                &Script_CheckBinderDist },
    { "RetrieveCorpse",                 &Script_RetrieveCorpse },
    { "BindEnchant",                    &Script_BindEnchant },
    { "ReplaceEnchant",                 &Script_ReplaceEnchant },
    { "ReplaceTradeEnchant",            &Script_ReplaceTradeEnchant },
    { "NotWhileDeadError",              &Script_NotWhileDeadError },
    { "GetRestState",                   &Script_GetRestState },
    { "GetXPExhaustion",                &Script_GetXPExhaustion },
    { "GetTimeToWellRested",            &Script_GetTimeToWellRested },
    { "GMRequestPlayerInfo",            &Script_GMRequestPlayerInfo },
    { "GetCoinIcon",                    &Script_GetCoinIcon },
    { "GetCoinText",                    &Script_GetCoinText },
    { "GetCoinTextureString",           &Script_GetCoinTextureString },
    { "IsSubZonePVPPOI",                &Script_IsSubZonePVPPOI },
    { "GetZonePVPInfo",                 &Script_GetZonePVPInfo },
    { "TogglePVP",                      &Script_TogglePVP },
    { "SetPVP",                         &Script_SetPVP },
    { "GetPVPDesired",                  &Script_GetPVPDesired },
    { "GetPVPTimer",                    &Script_GetPVPTimer },
    { "IsPVPTimerRunning",              &Script_IsPVPTimerRunning },
    { "ConfirmBindOnUse",               &Script_ConfirmBindOnUse },
    { "SetPortraitToTexture",           &Script_SetPortraitToTexture },
    { "GetLocale",                      &Script_GetLocale },
    { "GetGMTicketCategories",          &Script_GetGMTicketCategories },
    { "DropItemOnUnit",                 &Script_DropItemOnUnit },
    { "RestartGx",                      &Script_RestartGx },
    { "RestoreVideoResolutionDefaults", &Script_RestoreVideoResolutionDefaults },
    { "RestoreVideoEffectsDefaults",    &Script_RestoreVideoEffectsDefaults },
    { "RestoreVideoStereoDefaults",     &Script_RestoreVideoStereoDefaults },
    { "GetBindLocation",                &Script_GetBindLocation },
    { "ConfirmTalentWipe",              &Script_ConfirmTalentWipe },
    { "ConfirmBinder",                  &Script_ConfirmBinder },
    { "ShowingHelm",                    &Script_ShowingHelm },
    { "ShowingCloak",                   &Script_ShowingCloak },
    { "ShowHelm",                       &Script_ShowHelm },
    { "ShowCloak",                      &Script_ShowCloak },
    { "SetEuropeanNumbers",             &Script_SetEuropeanNumbers },
    { "GetAreaSpiritHealerTime",        &Script_GetAreaSpiritHealerTime },
    { "AcceptAreaSpiritHeal",           &Script_AcceptAreaSpiritHeal },
    { "CancelAreaSpiritHeal",           &Script_CancelAreaSpiritHeal },
    { "GetMouseFocus",                  &Script_GetMouseFocus },
    { "GetRealmName",                   &Script_GetRealmName },
    { "GetItemQualityColor",            &Script_GetItemQualityColor },
    { "GetItemInfo",                    &Script_GetItemInfo },
    { "GetItemGem",                     &Script_GetItemGem },
    { "GetExtendedItemInfo",            &Script_GetExtendedItemInfo },
    { "GetItemIcon",                    &Script_GetItemIcon },
    { "GetItemFamily",                  &Script_GetItemFamily },
    { "GetItemCount",                   &Script_GetItemCount },
    { "GetItemSpell",                   &Script_GetItemSpell },
    { "GetItemCooldown",                &Script_GetItemCooldown },
    { "PickupItem",                     &Script_PickupItem },
    { "IsCurrentItem",                  &Script_IsCurrentItem },
    { "IsUsableItem",                   &Script_IsUsableItem },
    { "IsHelpfulItem",                  &Script_IsHelpfulItem },
    { "IsHarmfulItem",                  &Script_IsHarmfulItem },
    { "IsConsumableItem",               &Script_IsConsumableItem },
    { "IsEquippableItem",               &Script_IsEquippableItem },
    { "IsEquippedItem",                 &Script_IsEquippedItem },
    { "IsEquippedItemType",             &Script_IsEquippedItemType },
    { "IsDressableItem",                &Script_IsDressableItem },
    { "ItemHasRange",                   &Script_ItemHasRange },
    { "IsItemInRange",                  &Script_IsItemInRange },
    { "GetNumAddOns",                   &Script_GetNumAddOns },
    { "GetAddOnInfo",                   &Script_GetAddOnInfo },
    { "GetAddOnMetadata",               &Script_GetAddOnMetadata },
    { "UpdateAddOnMemoryUsage",         &Script_UpdateAddOnMemoryUsage },
    { "GetAddOnMemoryUsage",            &Script_GetAddOnMemoryUsage },
    { "GetScriptCPUUsage",              &Script_GetScriptCPUUsage },
    { "UpdateAddOnCPUUsage",            &Script_UpdateAddOnCPUUsage },
    { "GetAddOnCPUUsage",               &Script_GetAddOnCPUUsage },
    { "GetFunctionCPUUsage",            &Script_GetFunctionCPUUsage },
    { "GetFrameCPUUsage",               &Script_GetFrameCPUUsage },
    { "GetEventCPUUsage",               &Script_GetEventCPUUsage },
    { "ResetCPUUsage",                  &Script_ResetCPUUsage },
    { "GetAddOnDependencies",           &Script_GetAddOnDependencies },
    { "EnableAddOn",                    &Script_EnableAddOn },
    { "EnableAllAddOns",                &Script_EnableAllAddOns },
    { "DisableAddOn",                   &Script_DisableAddOn },
    { "DisableAllAddOns",               &Script_DisableAllAddOns },
    { "ResetDisabledAddOns",            &Script_ResetDisabledAddOns },
    { "IsAddOnLoadOnDemand",            &Script_IsAddOnLoadOnDemand },
    { "IsAddOnLoaded",                  &Script_IsAddOnLoaded },
    { "LoadAddOn",                      &Script_LoadAddOn },
    { "PartialPlayTime",                &Script_PartialPlayTime },
    { "NoPlayTime",                     &Script_NoPlayTime },
    { "GetBillingTimeRested",           &Script_GetBillingTimeRested },
    { "CanShowResetInstances",          &Script_CanShowResetInstances },
    { "ResetInstances",                 &Script_ResetInstances },
    { "IsInInstance",                   &Script_IsInInstance },
    { "GetInstanceDifficulty",          &Script_GetInstanceDifficulty },
    { "GetInstanceInfo",                &Script_GetInstanceInfo },
    { "GetDungeonDifficulty",           &Script_GetDungeonDifficulty },
    { "SetDungeonDifficulty",           &Script_SetDungeonDifficulty },
    { "GetRaidDifficulty",              &Script_GetRaidDifficulty },
    { "SetRaidDifficulty",              &Script_SetRaidDifficulty },
    { "ReportBug",                      &Script_ReportBug },
    { "ReportSuggestion",               &Script_ReportSuggestion },
    { "GetMirrorTimerInfo",             &Script_GetMirrorTimerInfo },
    { "GetMirrorTimerProgress",         &Script_GetMirrorTimerProgress },
    { "GetNumTitles",                   &Script_GetNumTitles },
    { "GetCurrentTitle",                &Script_GetCurrentTitle },
    { "SetCurrentTitle",                &Script_SetCurrentTitle },
    { "IsTitleKnown",                   &Script_IsTitleKnown },
    { "GetTitleName",                   &Script_GetTitleName },
    { "UseItemByName",                  &Script_UseItemByName },
    { "EquipItemByName",                &Script_EquipItemByName },
    { "GetExistingLocales",             &Script_GetExistingLocales },
    { "InCombatLockdown",               &Script_InCombatLockdown },
    { "StartAttack",                    &Script_StartAttack },
    { "StopAttack",                     &Script_StopAttack },
    { "SetTaxiBenchmarkMode",           &Script_SetTaxiBenchmarkMode },
    { "GetTaxiBenchmarkMode",           &Script_GetTaxiBenchmarkMode },
    { "Dismount",                       &Script_Dismount },
    { "VoicePushToTalkStart",           &Script_VoicePushToTalkStart },
    { "VoicePushToTalkStop",            &Script_VoicePushToTalkStop },
    { "SetUIVisibility",                &Script_SetUIVisibility },
    { "IsReferAFriendLinked",           &Script_IsReferAFriendLinked },
    { "CanGrantLevel",                  &Script_CanGrantLevel },
    { "GrantLevel",                     &Script_GrantLevel },
    { "CanSummonFriend",                &Script_CanSummonFriend },
    { "SummonFriend",                   &Script_SummonFriend },
    { "GetSummonFriendCooldown",        &Script_GetSummonFriendCooldown },
    { "GetTotemInfo",                   &Script_GetTotemInfo },
    { "GetTotemTimeLeft",               &Script_GetTotemTimeLeft },
    { "TargetTotem",                    &Script_TargetTotem },
    { "DestroyTotem",                   &Script_DestroyTotem },
    { "GetNumDeclensionSets",           &Script_GetNumDeclensionSets },
    { "DeclineName",                    &Script_DeclineName },
    { "AcceptLevelGrant",               &Script_AcceptLevelGrant },
    { "DeclineLevelGrant",              &Script_DeclineLevelGrant },
    { "UploadSettings",                 &Script_UploadSettings },
    { "DownloadSettings",               &Script_DownloadSettings },
    { "GetMovieResolution",             &Script_GetMovieResolution },
    { "GameMovieFinished",              &Script_GameMovieFinished },
    { "IsDesaturateSupported",          &Script_IsDesaturateSupported },
    { "GetThreatStatusColor",           &Script_GetThreatStatusColor },
    { "IsThreatWarningEnabled",         &Script_IsThreatWarningEnabled },
    { "ConsoleAddMessage",              &Script_ConsoleAddMessage },
    { "GetItemUniqueness",              &Script_GetItemUniqueness },
    { "EndRefund",                      &Script_EndRefund },
    { "EndBoundTradeable",              &Script_EndBoundTradeable },
    { "CanMapChangeDifficulty",         &Script_CanMapChangeDifficulty },
    { "GetExpansionLevel",              &Script_GetExpansionLevel },
    { "GetAllowLowLevelRaid",           &Script_GetAllowLowLevelRaid },
    { "SetAllowLowLevelRaid",           &Script_SetAllowLowLevelRaid },
};

void GameScriptRegisterFunctions() {
    for (auto& func : s_ScriptFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
    }
}
