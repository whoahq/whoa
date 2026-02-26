#include "ui/game/CGMinimapFrameScript.hpp"
#include "ui/FrameScript.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t CGMinimapFrame_SetMaskTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_SetIconTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_SetBlipTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_SetClassBlipTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_SetPOIArrowTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_SetStaticPOIArrowTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_SetCorpsePOIArrowTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_SetPlayerTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_SetPlayerTextureHeight(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_SetPlayerTextureWidth(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_GetZoomLevels(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_GetZoom(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_SetZoom(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_PingLocation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGMinimapFrame_GetPingPosition(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

FrameScript_Method CGMinimapFrameMethods[] = {
    { "SetMaskTexture",             &CGMinimapFrame_SetMaskTexture },
    { "SetIconTexture",             &CGMinimapFrame_SetIconTexture },
    { "SetBlipTexture",             &CGMinimapFrame_SetBlipTexture },
    { "SetClassBlipTexture",        &CGMinimapFrame_SetClassBlipTexture },
    { "SetPOIArrowTexture",         &CGMinimapFrame_SetPOIArrowTexture },
    { "SetStaticPOIArrowTexture",   &CGMinimapFrame_SetStaticPOIArrowTexture },
    { "SetCorpsePOIArrowTexture",   &CGMinimapFrame_SetCorpsePOIArrowTexture },
    { "SetPlayerTexture",           &CGMinimapFrame_SetPlayerTexture },
    { "SetPlayerTextureHeight",     &CGMinimapFrame_SetPlayerTextureHeight },
    { "SetPlayerTextureWidth",      &CGMinimapFrame_SetPlayerTextureWidth },
    { "GetZoomLevels",              &CGMinimapFrame_GetZoomLevels },
    { "GetZoom",                    &CGMinimapFrame_GetZoom },
    { "SetZoom",                    &CGMinimapFrame_SetZoom },
    { "PingLocation",               &CGMinimapFrame_PingLocation },
    { "GetPingPosition",            &CGMinimapFrame_GetPingPosition },
};
