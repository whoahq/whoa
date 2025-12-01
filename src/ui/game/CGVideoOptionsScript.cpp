#include "ui/game/CGVideoOptionsScript.hpp"
#include "console/CVar.hpp"
#include "console/Detect.hpp"
#include "gx/Adapter.hpp"
#include "gx/Gx.hpp"
#include "ui/Types.hpp"
#include "ui/game/CGVideoOptions.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <tempest/Vector.hpp>

static TSGrowableArray<C3iVector> s_multisampleFormats;
static TSGrowableArray<C2iVector> s_screenResolutions;

int32_t SortRefreshRates(const void* a, const void* b) {
    auto rateA = static_cast<const uint32_t*>(a);
    auto rateB = static_cast<const uint32_t*>(b);

    if (*rateA >= *rateB) {
        return *rateB < *rateA;
    }

    return -1;
}

void SetupFormats() {
    if (s_multisampleFormats.Count()) {
        return;
    }

    // TODO proper api selection

    TSGrowableArray<CGxFormat> adapterFormats;
    GxAdapterFormats(GxApi_OpenGl, adapterFormats);

    auto gxColorBitsVar = CVar::Lookup("gxColorBits");
    int32_t gxColorBits = gxColorBitsVar ? gxColorBitsVar->GetInt() : 0;

    auto gxDepthBitsVar = CVar::Lookup("gxDepthBits");
    int32_t gxDepthBits = gxDepthBitsVar ? gxDepthBitsVar->GetInt() : 0;

    // TODO what about gxMultisample CVar?

    for (uint32_t i = 0; i < adapterFormats.Count(); i++) {
        auto& adapterFormat = adapterFormats[i];

        auto colorBits = CGxFormat::formatToBitsUint[adapterFormat.colorFormat];
        auto depthBits = CGxFormat::formatToBitsUint[adapterFormat.depthFormat];
        auto multisampleCount = static_cast<int32_t>(adapterFormat.multisampleCount);

        if (colorBits != gxColorBits || depthBits != gxDepthBits) {
            continue;
        }

        auto formatPresent = false;

        for (uint32_t j = 0; j < s_multisampleFormats.Count(); j++) {
            auto& multisampleFormat = s_multisampleFormats[j];

            if (multisampleFormat.x == colorBits && multisampleFormat.y == depthBits && multisampleFormat.z == multisampleCount) {
                formatPresent = true;
                break;
            }
        }

        if (formatPresent) {
            continue;
        }

        *s_multisampleFormats.New() = { colorBits, depthBits, multisampleCount };
    }
}

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
    // Load available resolutions

    SetupResolutions();

    // Find matching resolution

    uint32_t resolutionIndex = 0;

    if (lua_isnumber(L, 1)) {
        if (lua_tonumber(L, 1) - 1 < s_screenResolutions.Count()) {
            resolutionIndex = static_cast<uint32_t>(lua_tonumber(L, 1)) - 1;
        } else {
            resolutionIndex = s_screenResolutions.Count();
        }
    }

    if (resolutionIndex >= s_screenResolutions.Count()) {
        return 0;
    }

    auto& screenResolution = s_screenResolutions[resolutionIndex];

    // Load available monitor modes

    TSGrowableArray<CGxMonitorMode> monitorModes;
    GxAdapterMonitorModes(monitorModes);

    // Find refresh rates matching desired resolution

    uint32_t rateCount = 0;
    auto refreshRates = static_cast<uint32_t*>(alloca(sizeof(uint32_t) * monitorModes.Count()));

    for (uint32_t i = 0; i < monitorModes.Count(); i++) {
        auto& monitorMode = monitorModes[i];

        if (monitorMode.size.x == screenResolution.x && monitorMode.size.y == screenResolution.y) {
            refreshRates[rateCount++] = monitorModes[i].refreshRate;
        }
    }

    // Sort refresh rates

    if (rateCount > 0) {
        qsort(refreshRates, rateCount, sizeof(uint32_t), &SortRefreshRates);
    }

    // Return sorted refresh rates

    int32_t nargs = 0;
    for (uint32_t i = 0; i < rateCount; i++) {
        if (i == 0 || refreshRates[i] != refreshRates[i - 1]) {
            lua_pushnumber(L, refreshRates[i]);
            nargs++;
        }
    }

    return nargs;
}

int32_t Script_SetupFullscreenScale(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMultisampleFormats(lua_State* L) {
    SetupFormats();

    lua_checkstack(L, s_multisampleFormats.Count() * 3);

    for (uint32_t i = 0; i < s_multisampleFormats.Count(); i++) {
        lua_pushnumber(L, s_multisampleFormats[i].x);
        lua_pushnumber(L, s_multisampleFormats[i].y);
        lua_pushnumber(L, s_multisampleFormats[i].z);
    }

    return s_multisampleFormats.Count() * 3;
}

int32_t Script_GetCurrentMultisampleFormat(lua_State* L) {
    // Load available multisample formats

    SetupFormats();

    auto gxColorBitsVar = CVar::Lookup("gxColorBits");
    int32_t gxColorBits = gxColorBitsVar ? gxColorBitsVar->GetInt() : 0;

    auto gxDepthBitsVar = CVar::Lookup("gxDepthBits");
    int32_t gxDepthBits = gxDepthBitsVar ? gxDepthBitsVar->GetInt() : 0;

    auto gxMultisampleVar = CVar::Lookup("gxMultisample");
    int32_t gxMultisample = gxMultisampleVar ? gxMultisampleVar->GetInt() : 0;

    // Search for matching multisample format in available multisample formats

    for (int32_t i = 0; i < s_multisampleFormats.Count(); i++) {
        auto& multisampleFormat = s_multisampleFormats[i];

        if (multisampleFormat.x == gxColorBits && multisampleFormat.y == gxDepthBits && multisampleFormat.z == gxMultisample) {
            // Multisample format found

            lua_pushnumber(L, i + 1);

            return 1;
        }
    }

    // Multisample format not found

    lua_pushnumber(L, 1.0);

    return 1;
}

int32_t Script_SetMultisampleFormat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetVideoCaps(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetGamma(lua_State* L) {
    auto gammaVar = CVar::Lookup("gamma");

    lua_pushnumber(L, 1.0 - gammaVar->GetFloat());

    return 1;
}

int32_t Script_SetGamma(lua_State* L) {
    if (!lua_isnumber(L, 1)) {
        return luaL_error(L, "Usage: SetGamma(value)");
    }

    char gammaStr[16];
    SStrPrintf(gammaStr, sizeof(gammaStr), "%f", 1.0 - lua_tonumber(L, 1));

    auto gammaVar = CVar::Lookup("gamma");
    gammaVar->Set(gammaStr, true, false, false, true);

    return 0;
}

int32_t Script_GetTerrainMip(lua_State* L) {
    auto shadowLevelVar = CVar::Lookup("shadowLevel");

    lua_pushnumber(L, 1.0 - shadowLevelVar->GetInt());

    return 1;
}

int32_t Script_SetTerrainMip(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsStereoVideoAvailable(lua_State* L) {
    if (GxCaps().m_stereoAvailable) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    return 1;
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
