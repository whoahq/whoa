#include "glue/GlueScript.hpp"

const char* g_glueScriptEvents[41];

void GlueScriptEventsInitialize() {
    g_glueScriptEvents[0] = "SET_GLUE_SCREEN";
    g_glueScriptEvents[1] = "START_GLUE_MUSIC";
    g_glueScriptEvents[2] = "DISCONNECTED_FROM_SERVER";
    g_glueScriptEvents[3] = "OPEN_STATUS_DIALOG";
    g_glueScriptEvents[4] = "UPDATE_STATUS_DIALOG";
    g_glueScriptEvents[5] = "CLOSE_STATUS_DIALOG";
    g_glueScriptEvents[6] = "ADDON_LIST_UPDATE";
    g_glueScriptEvents[7] = "CHARACTER_LIST_UPDATE";
    g_glueScriptEvents[8] = "UPDATE_SELECTED_CHARACTER";
    g_glueScriptEvents[9] = "OPEN_REALM_LIST";
    g_glueScriptEvents[10] = "GET_PREFERRED_REALM_INFO";
    g_glueScriptEvents[11] = "UPDATE_SELECTED_RACE";
    g_glueScriptEvents[12] = "SELECT_LAST_CHARACTER";
    g_glueScriptEvents[13] = "SELECT_FIRST_CHARACTER";
    g_glueScriptEvents[14] = "GLUE_SCREENSHOT_SUCCEEDED";
    g_glueScriptEvents[15] = "GLUE_SCREENSHOT_FAILED";
    g_glueScriptEvents[16] = "PATCH_UPDATE_PROGRESS";
    g_glueScriptEvents[17] = "PATCH_DOWNLOADED";
    g_glueScriptEvents[18] = "SUGGEST_REALM";
    g_glueScriptEvents[19] = "SUGGEST_REALM_WRONG_PVP";
    g_glueScriptEvents[20] = "SUGGEST_REALM_WRONG_CATEGORY";
    g_glueScriptEvents[21] = "SHOW_SERVER_ALERT";
    g_glueScriptEvents[22] = "FRAMES_LOADED";
    g_glueScriptEvents[23] = "FORCE_RENAME_CHARACTER";
    g_glueScriptEvents[24] = "FORCE_DECLINE_CHARACTER";
    g_glueScriptEvents[25] = "SHOW_SURVEY_NOTIFICATION";
    g_glueScriptEvents[26] = "PLAYER_ENTER_PIN";
    g_glueScriptEvents[27] = "CLIENT_ACCOUNT_MISMATCH";
    g_glueScriptEvents[28] = "PLAYER_ENTER_MATRIX";
    g_glueScriptEvents[29] = "SCANDLL_ERROR";
    g_glueScriptEvents[30] = "SCANDLL_DOWNLOADING";
    g_glueScriptEvents[31] = "SCANDLL_FINISHED";
    g_glueScriptEvents[32] = "SERVER_SPLIT_NOTICE";
    g_glueScriptEvents[33] = "TIMER_ALERT";
    g_glueScriptEvents[34] = "ACCOUNT_MESSAGES_AVAILABLE";
    g_glueScriptEvents[35] = "ACCOUNT_MESSAGES_HEADERS_LOADED";
    g_glueScriptEvents[36] = "ACCOUNT_MESSAGES_BODY_LOADED";
    g_glueScriptEvents[37] = "CLIENT_TRIAL";
    g_glueScriptEvents[38] = "PLAYER_ENTER_TOKEN";
    g_glueScriptEvents[39] = "GAME_ACCOUNTS_UPDATED";
    g_glueScriptEvents[40] = "CLIENT_CONVERTED";
}
