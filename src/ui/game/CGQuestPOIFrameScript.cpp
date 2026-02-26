#include "ui/game/CGQuestPOIFrameScript.hpp"
#include "ui/FrameScript.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t CGQuestPOIFrame_SetFillTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_SetFillAlpha(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_SetBorderTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_SetBorderAlpha(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_SetBorderScalar(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_DrawQuestBlob(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_EnableSmoothing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_EnableMerging(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_SetMergeThreshold(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_SetNumSplinePoints(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_UpdateQuestPOI(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_UpdateMouseOverTooltip(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_GetTooltipIndex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGQuestPOIFrame_GetNumTooltips(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

FrameScript_Method CGQuestPOIFrameMethods[] = {
    { "SetFillTexture",         &CGQuestPOIFrame_SetFillTexture },
    { "SetFillAlpha",           &CGQuestPOIFrame_SetFillAlpha },
    { "SetBorderTexture",       &CGQuestPOIFrame_SetBorderTexture },
    { "SetBorderAlpha",         &CGQuestPOIFrame_SetBorderAlpha },
    { "SetBorderScalar",        &CGQuestPOIFrame_SetBorderScalar },
    { "DrawQuestBlob",          &CGQuestPOIFrame_DrawQuestBlob },
    { "EnableSmoothing",        &CGQuestPOIFrame_EnableSmoothing },
    { "EnableMerging",          &CGQuestPOIFrame_EnableMerging },
    { "SetMergeThreshold",      &CGQuestPOIFrame_SetMergeThreshold },
    { "SetNumSplinePoints",     &CGQuestPOIFrame_SetNumSplinePoints },
    { "UpdateQuestPOI",         &CGQuestPOIFrame_UpdateQuestPOI },
    { "UpdateMouseOverTooltip", &CGQuestPOIFrame_UpdateMouseOverTooltip },
    { "GetTooltipIndex",        &CGQuestPOIFrame_GetTooltipIndex },
    { "GetNumTooltips",         &CGQuestPOIFrame_GetNumTooltips },
};
