#include "ui/game/CGTabardModelFrameScript.hpp"
#include "ui/FrameScript.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t CGTabardModelFrame_InitializeTabardColors(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGTabardModelFrame_Save(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGTabardModelFrame_CycleVariation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGTabardModelFrame_GetUpperBackgroundFileName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGTabardModelFrame_GetLowerBackgroundFileName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGTabardModelFrame_GetUpperEmblemFileName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGTabardModelFrame_GetLowerEmblemFileName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGTabardModelFrame_GetUpperEmblemTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGTabardModelFrame_GetLowerEmblemTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGTabardModelFrame_CanSaveTabardNow(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

FrameScript_Method CGTabardModelFrameMethods[] = {
    { "InitializeTabardColors",     &CGTabardModelFrame_InitializeTabardColors },
    { "Save",                       &CGTabardModelFrame_Save },
    { "CycleVariation",             &CGTabardModelFrame_CycleVariation },
    { "GetUpperBackgroundFileName", &CGTabardModelFrame_GetUpperBackgroundFileName },
    { "GetLowerBackgroundFileName", &CGTabardModelFrame_GetLowerBackgroundFileName },
    { "GetUpperEmblemFileName",     &CGTabardModelFrame_GetUpperEmblemFileName },
    { "GetLowerEmblemFileName",     &CGTabardModelFrame_GetLowerEmblemFileName },
    { "GetUpperEmblemTexture",      &CGTabardModelFrame_GetUpperEmblemTexture },
    { "GetLowerEmblemTexture",      &CGTabardModelFrame_GetLowerEmblemTexture },
    { "CanSaveTabardNow",           &CGTabardModelFrame_CanSaveTabardNow },
};
