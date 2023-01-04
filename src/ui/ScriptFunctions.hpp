#ifndef UI_SCRIPT_FUNCTIONS_HPP
#define UI_SCRIPT_FUNCTIONS_HPP

#include "ui/Types.hpp"
#include <cstdint>

struct lua_State;

#define NUM_SCRIPT_FUNCTIONS_CHAR_CREATE 32
#define NUM_SCRIPT_FUNCTIONS_CHAR_SELECT 13
#define NUM_SCRIPT_FUNCTIONS_GLUE_SCRIPT_EVENTS 113
#define NUM_SCRIPT_FUNCTIONS_SIMPLE_FRAME 7
#define NUM_SCRIPT_FUNCTIONS_SYSTEM 7

namespace FrameScript {
    extern FrameScript_Method s_ScriptFunctions_CharCreate[NUM_SCRIPT_FUNCTIONS_CHAR_CREATE];
    extern FrameScript_Method s_ScriptFunctions_CharSelect[NUM_SCRIPT_FUNCTIONS_CHAR_SELECT];
    extern FrameScript_Method s_ScriptFunctions_GlueScriptEvents[NUM_SCRIPT_FUNCTIONS_GLUE_SCRIPT_EVENTS];
    extern FrameScript_Method s_ScriptFunctions_SimpleFrame[NUM_SCRIPT_FUNCTIONS_SIMPLE_FRAME];
    extern FrameScript_Method s_ScriptFunctions_System[NUM_SCRIPT_FUNCTIONS_SYSTEM];
}

// Utility

void CharacterCreateRegisterScriptFunctions();
void CharSelectRegisterScriptFunctions();
void GlueScriptEventsRegisterFunctions();
void RegisterSimpleFrameScriptMethods();
void SystemRegisterFunctions();

// Shared
int32_t Script_GetAccountExpansionLevel(lua_State*);

// CharCreate
int32_t Script_SetCharCustomizeFrame(lua_State*);
int32_t Script_SetCharCustomizeBackground(lua_State*);
int32_t Script_ResetCharCustomize(lua_State*);
int32_t Script_GetNameForRace(lua_State*);
int32_t Script_GetFactionForRace(lua_State*);
int32_t Script_GetAvailableRaces(lua_State*);
int32_t Script_GetAvailableClasses(lua_State*);
int32_t Script_GetClassesForRace(lua_State*);
int32_t Script_GetHairCustomization(lua_State*);
int32_t Script_GetFacialHairCustomization(lua_State*);
int32_t Script_GetSelectedRace(lua_State*);
int32_t Script_GetSelectedSex(lua_State*);
int32_t Script_GetSelectedClass(lua_State*);
int32_t Script_SetSelectedRace(lua_State*);
int32_t Script_SetSelectedSex(lua_State*);
int32_t Script_SetSelectedClass(lua_State*);
int32_t Script_UpdateCustomizationBackground(lua_State*);
int32_t Script_UpdateCustomizationScene(lua_State*);
int32_t Script_CycleCharCustomization(lua_State*);
int32_t Script_RandomizeCharCustomization(lua_State*);
int32_t Script_GetCharacterCreateFacing(lua_State*);
int32_t Script_SetCharacterCreateFacing(lua_State*);
int32_t Script_GetRandomName(lua_State*);
int32_t Script_CreateCharacter(lua_State*);
int32_t Script_CustomizeExistingCharacter(lua_State*);
int32_t Script_PaidChange_GetPreviousRaceIndex(lua_State*);
int32_t Script_PaidChange_GetCurrentRaceIndex(lua_State*);
int32_t Script_PaidChange_GetCurrentClassIndex(lua_State*);
int32_t Script_PaidChange_GetName(lua_State*);
int32_t Script_IsRaceClassValid(lua_State*);
int32_t Script_IsRaceClassRestricted(lua_State*);
int32_t Script_GetCreateBackgroundModel(lua_State*);

// CharSelect
int32_t Script_SetCharSelectModelFrame(lua_State*);
int32_t Script_SetCharSelectBackground(lua_State*);
int32_t Script_GetCharacterListUpdate(lua_State*);
int32_t Script_GetNumCharacters(lua_State*);
int32_t Script_GetCharacterInfo(lua_State*);
int32_t Script_SelectCharacter(lua_State*);
int32_t Script_DeleteCharacter(lua_State*);
int32_t Script_RenameCharacter(lua_State*);
int32_t Script_DeclineCharacter(lua_State*);
int32_t Script_UpdateSelectionCustomizationScene(lua_State*);
int32_t Script_GetCharacterSelectFacing(lua_State*);
int32_t Script_SetCharacterSelectFacing(lua_State*);
int32_t Script_GetSelectBackgroundModel(lua_State*);

// GlueScriptEvents
int32_t Script_IsShiftKeyDown(lua_State*);
int32_t Script_GetBuildInfo(lua_State*);
int32_t Script_GetLocale(lua_State*);
int32_t Script_GetSavedAccountName(lua_State*);
int32_t Script_SetSavedAccountName(lua_State*);
int32_t Script_GetUsesToken(lua_State*);
int32_t Script_SetUsesToken(lua_State*);
int32_t Script_GetSavedAccountList(lua_State*);
int32_t Script_SetSavedAccountList(lua_State*);
int32_t Script_SetCurrentScreen(lua_State*);
int32_t Script_QuitGame(lua_State*);
int32_t Script_QuitGameAndRunLauncher(lua_State*);
int32_t Script_PlayGlueMusic(lua_State*);
int32_t Script_PlayCreditsMusic(lua_State*);
int32_t Script_StopGlueMusic(lua_State*);
int32_t Script_GetMovieResolution(lua_State*);
int32_t Script_GetScreenWidth(lua_State*);
int32_t Script_GetScreenHeight(lua_State*);
int32_t Script_LaunchURL(lua_State*);
int32_t Script_ShowTOSNotice(lua_State*);
int32_t Script_TOSAccepted(lua_State*);
int32_t Script_AcceptTOS(lua_State*);
int32_t Script_ShowEULANotice(lua_State*);
int32_t Script_EULAAccepted(lua_State*);
int32_t Script_AcceptEULA(lua_State*);
int32_t Script_ShowTerminationWithoutNoticeNotice(lua_State*);
int32_t Script_TerminationWithoutNoticeAccepted(lua_State*);
int32_t Script_AcceptTerminationWithoutNotice(lua_State*);
int32_t Script_ShowScanningNotice(lua_State*);
int32_t Script_ScanningAccepted(lua_State*);
int32_t Script_AcceptScanning(lua_State*);
int32_t Script_ShowContestNotice(lua_State*);
int32_t Script_ContestAccepted(lua_State*);
int32_t Script_AcceptContest(lua_State*);
int32_t Script_DefaultServerLogin(lua_State*);
int32_t Script_StatusDialogClick(lua_State*);
int32_t Script_GetServerName(lua_State*);
int32_t Script_DisconnectFromServer(lua_State*);
int32_t Script_IsConnectedToServer(lua_State*);
int32_t Script_EnterWorld(lua_State*);
int32_t Script_Screenshot(lua_State*);
int32_t Script_PatchDownloadProgress(lua_State*);
int32_t Script_PatchDownloadCancel(lua_State*);
int32_t Script_PatchDownloadApply(lua_State*);
int32_t Script_GetNumAddOns(lua_State*);
int32_t Script_GetAddOnInfo(lua_State*);
int32_t Script_LaunchAddOnURL(lua_State*);
int32_t Script_GetAddOnDependencies(lua_State*);
int32_t Script_GetAddOnEnableState(lua_State*);
int32_t Script_EnableAddOn(lua_State*);
int32_t Script_EnableAllAddOns(lua_State*);
int32_t Script_DisableAddOn(lua_State*);
int32_t Script_DisableAllAddOns(lua_State*);
int32_t Script_SaveAddOns(lua_State*);
int32_t Script_ResetAddOns(lua_State*);
int32_t Script_IsAddonVersionCheckEnabled(lua_State*);
int32_t Script_SetAddonVersionCheck(lua_State*);
int32_t Script_GetCursorPosition(lua_State*);
int32_t Script_ShowCursor(lua_State*);
int32_t Script_HideCursor(lua_State*);
int32_t Script_GetBillingTimeRemaining(lua_State*);
int32_t Script_GetBillingPlan(lua_State*);
int32_t Script_GetBillingTimeRested(lua_State*);
int32_t Script_SurveyNotificationDone(lua_State*);
int32_t Script_PINEntered(lua_State*);
int32_t Script_PlayGlueAmbience(lua_State*);
int32_t Script_StopGlueAmbience(lua_State*);
int32_t Script_GetCreditsText(lua_State*);
int32_t Script_GetClientExpansionLevel(lua_State*);
int32_t Script_MatrixEntered(lua_State*);
int32_t Script_MatrixRevert(lua_State*);
int32_t Script_MatrixCommit(lua_State*);
int32_t Script_GetMatrixCoordinates(lua_State*);
int32_t Script_ScanDLLStart(lua_State*);
int32_t Script_ScanDLLContinueAnyway(lua_State*);
int32_t Script_IsScanDLLFinished(lua_State*);
int32_t Script_IsWindowsClient(lua_State*);
int32_t Script_IsOtherPlatformClient(lua_State*);
int32_t Script_SetRealmSplitState(lua_State*);
int32_t Script_RequestRealmSplitInfo(lua_State*);
int32_t Script_CancelLogin(lua_State*);
int32_t Script_GetCVar(lua_State*);
int32_t Script_GetCVarBool(lua_State*);
int32_t Script_SetCVar(lua_State*);
int32_t Script_GetCVarDefault(lua_State*);
int32_t Script_GetCVarMin(lua_State*);
int32_t Script_GetCVarMax(lua_State*);
int32_t Script_GetCVarAbsoluteMin(lua_State*);
int32_t Script_GetCVarAbsoluteMax(lua_State*);
int32_t Script_GetChangedOptionWarnings(lua_State*);
int32_t Script_AcceptChangedOptionWarnings(lua_State*);
int32_t Script_ShowChangedOptionWarnings(lua_State*);
int32_t Script_TokenEntered(lua_State*);
int32_t Script_GetNumDeclensionSets(lua_State*);
int32_t Script_DeclineName(lua_State*);
int32_t Script_GetNumGameAccounts(lua_State*);
int32_t Script_GetGameAccountInfo(lua_State*);
int32_t Script_SetGameAccount(lua_State*);
int32_t Script_StopAllSFX(lua_State*);
int32_t Script_SetClearConfigData(lua_State*);
int32_t Script_RestartGx(lua_State*);
int32_t Script_RestoreVideoResolutionDefaults(lua_State*);
int32_t Script_RestoreVideoEffectsDefaults(lua_State*);
int32_t Script_RestoreVideoStereoDefaults(lua_State*);
int32_t Script_IsStreamingMode(lua_State*);
int32_t Script_IsStreamingTrial(lua_State*);
int32_t Script_IsConsoleActive(lua_State*);
int32_t Script_RunScript(lua_State*);
int32_t Script_ReadyForAccountDataTimes(lua_State*);
int32_t Script_IsTrialAccount(lua_State*);
int32_t Script_IsSystemSupported(lua_State*);

// SimpleFrame
int32_t Script_GetText(lua_State*);
int32_t Script_GetNumFrames(lua_State*);
int32_t Script_EnumerateFrames(lua_State*);
int32_t Script_CreateFont(lua_State*);
int32_t Script_CreateFrame(lua_State*);
int32_t Script_GetFramesRegisteredForEvent(lua_State*);
int32_t Script_GetCurrentKeyBoardFocus(lua_State*);

// System
int32_t Script_GetTime(lua_State*);
int32_t Script_GetGameTime(lua_State*);
int32_t Script_ConsoleExec(lua_State*);
int32_t Script_AccessDenied(lua_State*);

#endif
