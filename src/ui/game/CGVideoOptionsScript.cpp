#include "ui/game/CGVideoOptionsScript.hpp"
#include "ui/game/CGVideoOptions.hpp"
#include "ui/Types.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"

int32_t Script_GetScreenResolutions(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}
int32_t Script_GetCurrentResolution(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetScreenResolution(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRefreshRates(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetupFullscreenScale(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMultisampleFormats(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCurrentMultisampleFormat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetMultisampleFormat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetVideoCaps(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetGamma(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetGamma(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetTerrainMip(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetTerrainMip(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsStereoVideoAvailable(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsPlayerResolutionAvailable(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

FrameScript_Method CGVideoOptions::s_ScriptFunctions[NUM_SCRIPT_FUNCTIONS_VIDEO_OPTIONS] = {
    { "GetScreenResolutions",                           &Script_GetScreenResolutions },
    { "GetCurrentResolution",                           &Script_GetCurrentResolution },
    { "SetScreenResolution",                            &Script_SetScreenResolution },
    { "GetRefreshRates",                                &Script_GetRefreshRates },
    { "SetupFullscreenScale",                           &Script_SetupFullscreenScale },
    { "GetMultisampleFormats",                          &Script_GetMultisampleFormats },
    { "GetCurrentMultisampleFormat",                    &Script_GetCurrentMultisampleFormat },
    { "SetMultisampleFormat",                           &Script_SetMultisampleFormat },
    { "GetVideoCaps",                                   &Script_GetVideoCaps },
    { "GetGamma",                                       &Script_GetGamma },
    { "SetGamma",                                       &Script_SetGamma },
    { "GetTerrainMip",                                  &Script_GetTerrainMip },
    { "SetTerrainMip",                                  &Script_SetTerrainMip },
    { "IsStereoVideoAvailable",                         &Script_IsStereoVideoAvailable },
    { "IsPlayerResolutionAvailable",                    &Script_IsPlayerResolutionAvailable }
};
