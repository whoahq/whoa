#include "ui/ScriptFunctions.hpp"
#include "client/Client.hpp"
#include "client/ClientServices.hpp"
#include "db/Db.hpp"
#include "glue/CGlueMgr.hpp"
#include "gx/Coordinate.hpp"
#include "net/connection/ClientConnection.hpp"
#include "ui/CSimpleTop.hpp"
#include "ui/Types.hpp"
#include "console/CVar.hpp"
#include "util/Lua.hpp"
#include "util/SFile.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t Script_IsShiftKeyDown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBuildInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetLocale(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSavedAccountName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetSavedAccountName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetUsesToken(lua_State* L) {
    // TODO
    lua_pushboolean(L, 0);

    return 1;
}

int32_t Script_SetUsesToken(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSavedAccountList(lua_State* L) {
    lua_pushstring(L, Client::g_accountListVar->GetString());
    return 1;
}

int32_t Script_SetSavedAccountList(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetCurrentScreen(lua_State* L) {
    if (!lua_isstring(L, 1)) {
        return luaL_error(L, "Usage: SetCurrentScreen(\"screen\")");
    }

    auto screen = lua_tolstring(L, 1, nullptr);
    CGlueMgr::UpdateCurrentScreen(screen);

    // TODO
    // if (!Blizzard::Streaming::IsStreamingMode()) {
    //     Sub4BAE10();
    // }

    return 0;
}

int32_t Script_QuitGame(lua_State* L) {
    CGlueMgr::QuitGame();

    return 0;
}

int32_t Script_QuitGameAndRunLauncher(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PlayGlueMusic(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PlayCreditsMusic(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_StopGlueMusic(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMovieResolution(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetScreenWidth(lua_State* L) {
    float v1 = NDCToDDCWidth(1.0);
    float v2 = CoordinateGetAspectCompensation() * 1024.0 * v1;
    float v3 = DDCToNDCWidth(v2);

    lua_pushnumber(L, v3);

    return 1;
}

int32_t Script_GetScreenHeight(lua_State* L) {
    float v1 = NDCToDDCHeight(1.0);
    float v2 = CoordinateGetAspectCompensation() * 1024.0 * v1;
    float v3 = DDCToNDCWidth(v2);

    lua_pushnumber(L, v3);

    return 1;
}

int32_t Script_LaunchURL(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ShowTOSNotice(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TOSAccepted(lua_State* L) {
    // TODO
    lua_pushboolean(L, 1);

    return 1;
}

int32_t Script_AcceptTOS(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ShowEULANotice(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_EULAAccepted(lua_State* L) {
    // TODO
    lua_pushboolean(L, 1);

    return 1;
}

int32_t Script_AcceptEULA(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ShowTerminationWithoutNoticeNotice(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TerminationWithoutNoticeAccepted(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptTerminationWithoutNotice(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ShowScanningNotice(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ScanningAccepted(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptScanning(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ShowContestNotice(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ContestAccepted(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptContest(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DefaultServerLogin(lua_State* L) {
    if (!lua_isstring(L, 1) || !lua_isstring(L, 2)) {
        luaL_error(L, "Usage: DefaultServerLogin(\"accountName\", \"password\")");
    }

    auto accountName = lua_tolstring(L, 1, nullptr);
    auto password = lua_tolstring(L, 2, nullptr);

    CGlueMgr::LoginServerLogin(accountName, password);

    return 0;
}

int32_t Script_StatusDialogClick(lua_State* L) {
    CGlueMgr::StatusDialogClick();

    return 0;
}

int32_t Script_GetServerName(lua_State* L) {
    auto selectedRealmName = ClientServices::GetSelectedRealmName();
    auto selectedRealm = ClientServices::GetSelectedRealm();

    auto pvp = false;
    auto rp = false;

    // default down to true: if realm config isn't found, consider realm down
    auto down = true;

    if (selectedRealm) {
        for (int32_t i = 0; i < g_cfg_ConfigsDB.GetNumRecords(); i++) {
            auto config = g_cfg_ConfigsDB.GetRecordByIndex(i);

            if (config->m_realmType == selectedRealm->type) {
                pvp = config->m_playerKillingAllowed != 0;
                rp = config->m_roleplaying != 0;
                down = selectedRealm->flags & 0x2;

                break;
            }
        }
    }

    // name
    lua_pushstring(L, selectedRealmName);

    // pvp
    if (pvp) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    // rp
    if (rp) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    // down
    if (down) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    return 4;
}

int32_t Script_DisconnectFromServer(lua_State* L) {
    if (ClientServices::IsConnected()) {
        CGlueMgr::m_disconnectPending = 1;
        ClientServices::Connection()->Disconnect();
    }

    ClientServices::LoginConnection()->Logoff();

    return 0;
}

int32_t Script_IsConnectedToServer(lua_State* L) {
    if (ClientServices::IsConnected()) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

int32_t Script_EnterWorld(lua_State* L) {
    CGlueMgr::EnterWorld();

    return 0;
}

int32_t Script_Screenshot(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PatchDownloadProgress(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PatchDownloadCancel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PatchDownloadApply(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumAddOns(lua_State* L) {
    // TODO
    lua_pushnumber(L, 0);

    return 1;
}

int32_t Script_GetAddOnInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_LaunchAddOnURL(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetAddOnDependencies(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetAddOnEnableState(lua_State* L) {
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

int32_t Script_SaveAddOns(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ResetAddOns(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsAddonVersionCheckEnabled(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetAddonVersionCheck(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCursorPosition(lua_State* L) {
    float cursorX = 0.0f;
    float cursorY = 0.0f;

    NDCToDDC(
        CSimpleTop::s_instance->m_mousePosition.x,
        CSimpleTop::s_instance->m_mousePosition.y,
        &cursorX,
        &cursorY
    );

    float v1 = (CoordinateGetAspectCompensation() * 1024.0f) * cursorX;
    float v2 = DDCToNDCWidth(v1);
    lua_pushnumber(L, v2);

    float v3 = (CoordinateGetAspectCompensation() * 1024.0f) * cursorY;
    float v4 = DDCToNDCWidth(v3);
    lua_pushnumber(L, v4);

    return 2;
}

int32_t Script_ShowCursor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_HideCursor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBillingTimeRemaining(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBillingPlan(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBillingTimeRested(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SurveyNotificationDone(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PINEntered(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PlayGlueAmbience(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_StopGlueAmbience(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCreditsText(lua_State* L) {
    if (!lua_isnumber(L, 1)) {
        luaL_error(L, "Usage: Script_GetCreditText(versionIndex)");
    }

    auto versionIndex = static_cast<int32_t>(lua_tonumber(L, 1));

    const char* filename;
    if (versionIndex == 1) {
        filename = "credits.html";
    } else if (versionIndex == 2) {
        filename = "credits_BC.html";
    } else {
        filename = "credits_LK.html";
    }

    void* data;
    if (SFile::Load(nullptr, filename, &data, nullptr, 1, 0x1, nullptr)) {
        lua_pushstring(L, static_cast<const char*>(data));
        SFile::Unload(data);
    }

    return 1;
}

int32_t Script_GetClientExpansionLevel(lua_State* L) {
    // TODO
    // v1 = sub_402A40();

    int32_t v1 = 2;

    lua_pushnumber(L, v1 + 1);

    return 1;
}

int32_t Script_MatrixEntered(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_MatrixRevert(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_MatrixCommit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMatrixCoordinates(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ScanDLLStart(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ScanDLLContinueAnyway(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsScanDLLFinished(lua_State* L) {
    // TODO
    lua_pushboolean(L, 1);

    return 1;
}

int32_t Script_IsWindowsClient(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsOtherPlatformClient(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetRealmSplitState(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RequestRealmSplitInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CancelLogin(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCVar(lua_State* L) {
    if (!lua_isstring(L, 1)) {
        return luaL_error(L, "Usage: GetCVar(\"cvar\")");
    }

    auto name = lua_tostring(L, 1);
    auto var = CVar::LookupRegistered(name);

    if (!var) {
        return luaL_error(L, "Couldn't find CVar named '%s'", name);
    }

    lua_pushstring(L, var->GetString());

    return 1;
}

int32_t Script_GetCVarBool(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetCVar(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
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

int32_t Script_GetChangedOptionWarnings(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_AcceptChangedOptionWarnings(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ShowChangedOptionWarnings(lua_State* L) {
    // TODO
    lua_pushnil(L);

    return 1;
}

int32_t Script_TokenEntered(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumDeclensionSets(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DeclineName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetNumGameAccounts(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetGameAccountInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetGameAccount(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_StopAllSFX(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetClearConfigData(lua_State* L) {
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

int32_t Script_IsStreamingMode(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsStreamingTrial(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsConsoleActive(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RunScript(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ReadyForAccountDataTimes(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsTrialAccount(lua_State* L) {
    // TODO

    lua_pushnil(L);
    return 1;
}

int32_t Script_IsSystemSupported(lua_State* L) {
    // TODO

    lua_pushboolean(L, 1);
    return 1;
}

FrameScript_Method FrameScript::s_ScriptFunctions_GlueScriptEvents[NUM_SCRIPT_FUNCTIONS_GLUE_SCRIPT_EVENTS] = {
    { "IsShiftKeyDown",             &Script_IsShiftKeyDown },
    { "GetBuildInfo",               &Script_GetBuildInfo },
    { "GetLocale",                  &Script_GetLocale },
    { "GetSavedAccountName",        &Script_GetSavedAccountName },
    { "SetSavedAccountName",        &Script_SetSavedAccountName },
    { "GetUsesToken",               &Script_GetUsesToken },
    { "SetUsesToken",               &Script_SetUsesToken },
    { "GetSavedAccountList",        &Script_GetSavedAccountList },
    { "SetSavedAccountList",        &Script_SetSavedAccountList },
    { "SetCurrentScreen",           &Script_SetCurrentScreen },
    { "QuitGame",                   &Script_QuitGame },
    { "QuitGameAndRunLauncher",     &Script_QuitGameAndRunLauncher },
    { "PlayGlueMusic",              &Script_PlayGlueMusic },
    { "PlayCreditsMusic",           &Script_PlayCreditsMusic },
    { "StopGlueMusic",              &Script_StopGlueMusic },
    { "GetMovieResolution",         &Script_GetMovieResolution },
    { "GetScreenWidth",             &Script_GetScreenWidth },
    { "GetScreenHeight",            &Script_GetScreenHeight },
    { "LaunchURL",                  &Script_LaunchURL },
    { "ShowTOSNotice",              &Script_ShowTOSNotice },
    { "TOSAccepted",                &Script_TOSAccepted },
    { "AcceptTOS",                  &Script_AcceptTOS },
    { "ShowEULANotice",             &Script_ShowEULANotice },
    { "EULAAccepted",               &Script_EULAAccepted },
    { "AcceptEULA",                 &Script_AcceptEULA },
    { "ShowTerminationWithoutNoticeNotice", &Script_ShowTerminationWithoutNoticeNotice },
    { "TerminationWithoutNoticeAccepted", &Script_TerminationWithoutNoticeAccepted },
    { "AcceptTerminationWithoutNotice", &Script_AcceptTerminationWithoutNotice },
    { "ShowScanningNotice",         &Script_ShowScanningNotice },
    { "ScanningAccepted",           &Script_ScanningAccepted },
    { "AcceptScanning",             &Script_AcceptScanning },
    { "ShowContestNotice",          &Script_ShowContestNotice },
    { "ContestAccepted",            &Script_ContestAccepted },
    { "AcceptContest",              &Script_AcceptContest },
    { "DefaultServerLogin",         &Script_DefaultServerLogin },
    { "StatusDialogClick",          &Script_StatusDialogClick },
    { "GetServerName",              &Script_GetServerName },
    { "DisconnectFromServer",       &Script_DisconnectFromServer },
    { "IsConnectedToServer",        &Script_IsConnectedToServer },
    { "EnterWorld",                 &Script_EnterWorld },
    { "Screenshot",                 &Script_Screenshot },
    { "PatchDownloadProgress",      &Script_PatchDownloadProgress },
    { "PatchDownloadCancel",        &Script_PatchDownloadCancel },
    { "PatchDownloadApply",         &Script_PatchDownloadApply },
    { "GetNumAddOns",               &Script_GetNumAddOns },
    { "GetAddOnInfo",               &Script_GetAddOnInfo },
    { "LaunchAddOnURL",             &Script_LaunchAddOnURL },
    { "GetAddOnDependencies",       &Script_GetAddOnDependencies },
    { "GetAddOnEnableState",        &Script_GetAddOnEnableState },
    { "EnableAddOn",                &Script_EnableAddOn },
    { "EnableAllAddOns",            &Script_EnableAllAddOns },
    { "DisableAddOn",               &Script_DisableAddOn },
    { "DisableAllAddOns",           &Script_DisableAllAddOns },
    { "SaveAddOns",                 &Script_SaveAddOns },
    { "ResetAddOns",                &Script_ResetAddOns },
    { "IsAddonVersionCheckEnabled", &Script_IsAddonVersionCheckEnabled },
    { "SetAddonVersionCheck",       &Script_SetAddonVersionCheck },
    { "GetCursorPosition",          &Script_GetCursorPosition },
    { "ShowCursor",                 &Script_ShowCursor },
    { "HideCursor",                 &Script_HideCursor },
    { "GetBillingTimeRemaining",    &Script_GetBillingTimeRemaining },
    { "GetBillingPlan",             &Script_GetBillingPlan },
    { "GetBillingTimeRested",       &Script_GetBillingTimeRested },
    { "SurveyNotificationDone",     &Script_SurveyNotificationDone },
    { "PINEntered",                 &Script_PINEntered },
    { "PlayGlueAmbience",           &Script_PlayGlueAmbience },
    { "StopGlueAmbience",           &Script_StopGlueAmbience },
    { "GetCreditsText",             &Script_GetCreditsText },
    { "GetAccountExpansionLevel",   &Script_GetAccountExpansionLevel }, // shared
    { "GetClientExpansionLevel",    &Script_GetClientExpansionLevel }, // guessed name
    { "MatrixEntered",              &Script_MatrixEntered },
    { "MatrixRevert",               &Script_MatrixRevert },
    { "MatrixCommit",               &Script_MatrixCommit },
    { "GetMatrixCoordinates",       &Script_GetMatrixCoordinates },
    { "ScanDLLStart",               &Script_ScanDLLStart },
    { "ScanDLLContinueAnyway",      &Script_ScanDLLContinueAnyway },
    { "IsScanDLLFinished",          &Script_IsScanDLLFinished },
    { "IsWindowsClient",            &Script_IsWindowsClient },
    { "IsMacClient",                &Script_IsOtherPlatformClient },
    { "IsLinuxClient",              &Script_IsOtherPlatformClient },
    { "SetRealmSplitState",         &Script_SetRealmSplitState },
    { "RequestRealmSplitInfo",      &Script_RequestRealmSplitInfo },
    { "CancelLogin",                &Script_CancelLogin },
    { "GetCVar",                    &Script_GetCVar },
    { "GetCVarBool",                &Script_GetCVarBool },
    { "SetCVar",                    &Script_SetCVar },
    { "GetCVarDefault",             &Script_GetCVarDefault },
    { "GetCVarMin",                 &Script_GetCVarMin }, // guessed name
    { "GetCVarMax",                 &Script_GetCVarMax }, // guessed name
    { "GetCVarAbsoluteMin",         &Script_GetCVarAbsoluteMin }, // guessed name
    { "GetCVarAbsoluteMax",         &Script_GetCVarAbsoluteMax }, // guessed name
    { "GetChangedOptionWarnings",   &Script_GetChangedOptionWarnings },
    { "AcceptChangedOptionWarnings", &Script_AcceptChangedOptionWarnings },
    { "ShowChangedOptionWarnings",  &Script_ShowChangedOptionWarnings },
    { "TokenEntered",               &Script_TokenEntered },
    { "GetNumDeclensionSets",       &Script_GetNumDeclensionSets },
    { "DeclineName",                &Script_DeclineName },
    { "GetNumGameAccounts",         &Script_GetNumGameAccounts },
    { "GetGameAccountInfo",         &Script_GetGameAccountInfo },
    { "SetGameAccount",             &Script_SetGameAccount },
    { "StopAllSFX",                 &Script_StopAllSFX },
    { "SetClearConfigData",         &Script_SetClearConfigData },
    { "RestartGx",                  &Script_RestartGx },
    { "RestoreVideoResolutionDefaults", &Script_RestoreVideoResolutionDefaults }, // guessed name
    { "RestoreVideoEffectsDefaults", &Script_RestoreVideoEffectsDefaults }, // guessed name
    { "RestoreVideoStereoDefaults", &Script_RestoreVideoStereoDefaults }, // guessed name
    { "IsStreamingMode",            &Script_IsStreamingMode },
    { "IsStreamingTrial",           &Script_IsStreamingTrial },
    { "IsConsoleActive",            &Script_IsConsoleActive },
    { "RunScript",                  &Script_RunScript },
    { "ReadyForAccountDataTimes",   &Script_ReadyForAccountDataTimes },
    { "IsTrialAccount",             &Script_IsTrialAccount }, // guessed name
    { "IsSystemSupported",          &Script_IsSystemSupported }
};
