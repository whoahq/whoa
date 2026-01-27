#include "ui/game/GMTicketInfoScript.hpp"
#include "ui/FrameScript.hpp"
#include "util/Unimplemented.hpp"

int32_t Script_GetGMTicket(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_NewGMTicket(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UpdateGMTicket(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_DeleteGMTicket(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GMResponseNeedMoreHelp(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GMResponseResolve(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetGMStatus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GMSurveyQuestion(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GMSurveyNumAnswers(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GMSurveyAnswer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GMSurveyAnswerSubmit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GMSurveyCommentSubmit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GMSurveySubmit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GMReportLag(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RegisterStaticConstants(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

static FrameScript_Method s_ScriptFunctions[] = {
    { "GetGMTicket",                &Script_GetGMTicket },
    { "NewGMTicket",                &Script_NewGMTicket },
    { "UpdateGMTicket",             &Script_UpdateGMTicket },
    { "DeleteGMTicket",             &Script_DeleteGMTicket },
    { "GMResponseNeedMoreHelp",     &Script_GMResponseNeedMoreHelp },
    { "GMResponseResolve",          &Script_GMResponseResolve },
    { "GetGMStatus",                &Script_GetGMStatus },
    { "GMSurveyQuestion",           &Script_GMSurveyQuestion },
    { "GMSurveyNumAnswers",         &Script_GMSurveyNumAnswers },
    { "GMSurveyAnswer",             &Script_GMSurveyAnswer },
    { "GMSurveyAnswerSubmit",       &Script_GMSurveyAnswerSubmit },
    { "GMSurveyCommentSubmit",      &Script_GMSurveyCommentSubmit },
    { "GMSurveySubmit",             &Script_GMSurveySubmit },
    { "GMReportLag",                &Script_GMReportLag },
    { "RegisterStaticConstants",    &Script_RegisterStaticConstants },
};

void GMTicketInfoRegisterScriptFunctions() {
    for (auto& func : s_ScriptFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
    }
}
