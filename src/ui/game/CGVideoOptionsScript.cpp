#include "ui/game/CGVideoOptionsScript.hpp"
#include "console/CVar.hpp"
#include "console/Detect.hpp"
#include "ui/Types.hpp"
#include "ui/game/CGVideoOptions.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <tempest/Vector.hpp>

static TSGrowableArray<C2iVector> s_screenResolutions;

void SetupResolutions() {
    if (s_screenResolutions.Count()) {
        return;
    }

    auto widescreenCvar = CVar::Lookup("widescreen");
    int32_t widescreen = widescreenCvar ? widescreenCvar->GetInt() : 0;

    ConsoleDetectGetResolutions(s_screenResolutions, widescreen);
}

int32_t Script_GetScreenResolutions(lua_State* L) {
    SetupResolutions();

    lua_checkstack(L, s_screenResolutions.Count());

    char resolutionStr[64];

    for (uint32_t i = 0; i < s_screenResolutions.Count(); i++) {
        auto& resolution = s_screenResolutions[i];

        SStrPrintf(resolutionStr, sizeof(resolutionStr), "%dx%d", resolution.x, resolution.y);

        lua_pushstring(L, resolutionStr);
    }

    return s_screenResolutions.Count();
}

int32_t Script_GetCurrentResolution(lua_State* L) {
    // Get available screen resolutions

    SetupResolutions();

    if (s_screenResolutions.Count() == 0) {
        lua_pushnumber(L, 1.0);

        return 1;
    }

    auto resolutionVar = CVar::Lookup("gxResolution");

    if (!resolutionVar) {
        lua_pushnumber(L, 1.0);

        return 1;
    }

    auto resolutionStr = resolutionVar->GetString();

    if (!resolutionStr) {
        lua_pushnumber(L, 1.0);

        return 1;
    }

    // Search for current resolution in available screen resolutions

    char sep;
    int32_t width = 0;
    int32_t height = 0;
    sscanf(resolutionStr, "%d%c%d", &width, &sep, &height);

    for (int32_t i = 0; i < s_screenResolutions.Count(); i++) {
        auto& resolution = s_screenResolutions[i];

        if (resolution.x == width && resolution.y == height) {
            // Resolution found

            lua_pushnumber(L, i + 1);

            return 1;
        }
    }

    // Resolution not found

    lua_pushnumber(L, 1.0);

    return 1;
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
