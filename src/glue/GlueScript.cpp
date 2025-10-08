#include "glue/GlueScript.hpp"
#include "glue/Types.hpp"

const char* g_glueScriptEvents[NUM_GLUESCRIPTEVENTS];

void GlueScriptEventsInitialize() {
    g_glueScriptEvents[SET_GLUE_SCREEN]                 = "SET_GLUE_SCREEN";
    g_glueScriptEvents[START_GLUE_MUSIC]                = "START_GLUE_MUSIC";
    g_glueScriptEvents[DISCONNECTED_FROM_SERVER]        = "DISCONNECTED_FROM_SERVER";
    g_glueScriptEvents[OPEN_STATUS_DIALOG]              = "OPEN_STATUS_DIALOG";
    g_glueScriptEvents[UPDATE_STATUS_DIALOG]            = "UPDATE_STATUS_DIALOG";
    g_glueScriptEvents[CLOSE_STATUS_DIALOG]             = "CLOSE_STATUS_DIALOG";
    g_glueScriptEvents[ADDON_LIST_UPDATE]               = "ADDON_LIST_UPDATE";
    g_glueScriptEvents[CHARACTER_LIST_UPDATE]           = "CHARACTER_LIST_UPDATE";
    g_glueScriptEvents[UPDATE_SELECTED_CHARACTER]       = "UPDATE_SELECTED_CHARACTER";
    g_glueScriptEvents[OPEN_REALM_LIST]                 = "OPEN_REALM_LIST";
    g_glueScriptEvents[GET_PREFERRED_REALM_INFO]        = "GET_PREFERRED_REALM_INFO";
    g_glueScriptEvents[UPDATE_SELECTED_RACE]            = "UPDATE_SELECTED_RACE";
    g_glueScriptEvents[SELECT_LAST_CHARACTER]           = "SELECT_LAST_CHARACTER";
    g_glueScriptEvents[SELECT_FIRST_CHARACTER]          = "SELECT_FIRST_CHARACTER";
    g_glueScriptEvents[GLUE_SCREENSHOT_SUCCEEDED]       = "GLUE_SCREENSHOT_SUCCEEDED";
    g_glueScriptEvents[GLUE_SCREENSHOT_FAILED]          = "GLUE_SCREENSHOT_FAILED";
    g_glueScriptEvents[PATCH_UPDATE_PROGRESS]           = "PATCH_UPDATE_PROGRESS";
    g_glueScriptEvents[PATCH_DOWNLOADED]                = "PATCH_DOWNLOADED";
    g_glueScriptEvents[SUGGEST_REALM]                   = "SUGGEST_REALM";
    g_glueScriptEvents[SUGGEST_REALM_WRONG_PVP]         = "SUGGEST_REALM_WRONG_PVP";
    g_glueScriptEvents[SUGGEST_REALM_WRONG_CATEGORY]    = "SUGGEST_REALM_WRONG_CATEGORY";
    g_glueScriptEvents[SHOW_SERVER_ALERT]               = "SHOW_SERVER_ALERT";
    g_glueScriptEvents[FRAMES_LOADED]                   = "FRAMES_LOADED";
    g_glueScriptEvents[FORCE_RENAME_CHARACTER]          = "FORCE_RENAME_CHARACTER";
    g_glueScriptEvents[FORCE_DECLINE_CHARACTER]         = "FORCE_DECLINE_CHARACTER";
    g_glueScriptEvents[SHOW_SURVEY_NOTIFICATION]        = "SHOW_SURVEY_NOTIFICATION";
    g_glueScriptEvents[PLAYER_ENTER_PIN]                = "PLAYER_ENTER_PIN";
    g_glueScriptEvents[CLIENT_ACCOUNT_MISMATCH]         = "CLIENT_ACCOUNT_MISMATCH";
    g_glueScriptEvents[PLAYER_ENTER_MATRIX]             = "PLAYER_ENTER_MATRIX";
    g_glueScriptEvents[SCANDLL_ERROR]                   = "SCANDLL_ERROR";
    g_glueScriptEvents[SCANDLL_DOWNLOADING]             = "SCANDLL_DOWNLOADING";
    g_glueScriptEvents[SCANDLL_FINISHED]                = "SCANDLL_FINISHED";
    g_glueScriptEvents[SERVER_SPLIT_NOTICE]             = "SERVER_SPLIT_NOTICE";
    g_glueScriptEvents[TIMER_ALERT]                     = "TIMER_ALERT";
    g_glueScriptEvents[ACCOUNT_MESSAGES_AVAILABLE]      = "ACCOUNT_MESSAGES_AVAILABLE";
    g_glueScriptEvents[ACCOUNT_MESSAGES_HEADERS_LOADED] = "ACCOUNT_MESSAGES_HEADERS_LOADED";
    g_glueScriptEvents[ACCOUNT_MESSAGES_BODY_LOADED]    = "ACCOUNT_MESSAGES_BODY_LOADED";
    g_glueScriptEvents[CLIENT_TRIAL]                    = "CLIENT_TRIAL";
    g_glueScriptEvents[PLAYER_ENTER_TOKEN]              = "PLAYER_ENTER_TOKEN";
    g_glueScriptEvents[GAME_ACCOUNTS_UPDATED]           = "GAME_ACCOUNTS_UPDATED";
    g_glueScriptEvents[CLIENT_CONVERTED]                = "CLIENT_CONVERTED";
}
