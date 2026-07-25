#include "ui/Util.hpp"
#include "util/Lua.hpp"
#include <storm/String.hpp>

const char* LanguageProcess(const char* string) {
    // TODO
    return string;
}

int32_t StringToBlendMode(const char* string, EGxBlend& blend) {
    struct BlendEntry {
        const char* string;
        EGxBlend value;
    };

    static BlendEntry blendMap[] = {
        { "DISABLE",    GxBlend_Opaque },
        { "BLEND",      GxBlend_Alpha },
        { "ALPHAKEY",   GxBlend_AlphaKey },
        { "ADD",        GxBlend_Add },
        { "MOD",        GxBlend_Mod },
    };

    for (const auto& entry : blendMap) {
        if (!SStrCmpI(entry.string, string)) {
            blend = entry.value;
            return true;
        }
    }

    return false;
}

int32_t StringToBOOL(const char* string) {
    return StringToBOOL(string, false);
}

bool StringToBOOL(const char* string, int32_t def) {
    if (!string) {
        return def;
    }

    switch (*string) {
        case '0':
        case 'F':
        case 'N':
        case 'f':
        case 'n':
            return false;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'T':
        case 'Y':
        case 't':
        case 'y':
            return true;

        default:
            if (!SStrCmpI(string, "off") || !SStrCmpI(string, "disabled")) {
                return false;
            }

            if (!SStrCmpI(string, "on") || !SStrCmpI(string, "enabled")) {
                return true;
            }

            return def;
    }
}

bool StringToBOOL(lua_State* L, int32_t idx, int32_t def) {
    switch (lua_type(L, idx)) {
        case LUA_TNIL:
            return false;

        case LUA_TBOOLEAN:
            return lua_toboolean(L, idx);

        case LUA_TNUMBER:
            return lua_tonumber(L, idx) != 0;

        case LUA_TSTRING:
            return StringToBOOL(lua_tostring(L, idx), def);

        default:
            return def;
    }
}

uint64_t StringToClickAction(const char* string) {
    if (!string || !*string) {
        return 0;
    }

    if (!SStrCmpI(string, "LeftButtonDown")) {
        return 1;
    }

    if (!SStrCmpI(string, "LeftButtonUp")) {
        return 0x80000000;
    }

    if (!SStrCmpI(string, "MiddleButtonDown")) {
        return 2;
    }

    if (!SStrCmpI(string, "MiddleButtonUp")) {
        return 0;
    }

    if (!SStrCmpI(string, "RightButtonDown")) {
        return 4;
    }

    if (!SStrCmpI(string, "RightButtonUp")) {
        return 0;
    }

    // TODO remaining buttons

    return 0;
}

int32_t StringToDrawLayer(const char* string, int32_t& layer) {
    struct LayerEntry {
        const char* string;
        int32_t layer;
    };

    static LayerEntry layerMap[] = {
        { "BACKGROUND", DRAWLAYER_BACKGROUND },
        { "BORDER",     DRAWLAYER_BACKGROUND_BORDER },
        { "ARTWORK",    DRAWLAYER_ARTWORK },
        { "OVERLAY",    DRAWLAYER_ARTWORK_OVERLAY },
        { "HIGHLIGHT",  DRAWLAYER_HIGHLIGHT },
    };

    for (const auto& entry : layerMap) {
        if (!SStrCmpI(string, entry.string)) {
            layer = entry.layer;
            return true;
        }
    }

    return false;
}

int32_t StringToFramePoint(const char* string, FRAMEPOINT& point) {
    struct FramePointEntry {
        const char* string;
        FRAMEPOINT value;
    };

    static FramePointEntry framePointMap[] = {
        { "BOTTOM",         FRAMEPOINT_BOTTOM },
        { "BOTTOMLEFT",     FRAMEPOINT_BOTTOMLEFT },
        { "BOTTOMRIGHT",    FRAMEPOINT_BOTTOMRIGHT },
        { "CENTER",         FRAMEPOINT_CENTER },
        { "TOP",            FRAMEPOINT_TOP },
        { "TOPRIGHT",       FRAMEPOINT_TOPRIGHT },
        { "TOPLEFT",        FRAMEPOINT_TOPLEFT },
        { "LEFT",           FRAMEPOINT_LEFT },
        { "RIGHT",          FRAMEPOINT_RIGHT },
    };

    for (const auto& entry : framePointMap) {
        if (!SStrCmpI(entry.string, string)) {
            point = entry.value;
            return true;
        }
    }

    return false;
}

int32_t StringToFrameStrata(const char* string, FRAME_STRATA& strata) {
    struct FrameStrataEntry {
        const char* string;
        FRAME_STRATA value;
    };

    // FRAME_STRATA_WORLD is hardcoded
    static FrameStrataEntry frameStrataMap[] = {
        { "BACKGROUND",         FRAME_STRATA_BACKGROUND },
        { "LOW",                FRAME_STRATA_LOW },
        { "MEDIUM",             FRAME_STRATA_MEDIUM },
        { "HIGH",               FRAME_STRATA_HIGH },
        { "DIALOG",             FRAME_STRATA_DIALOG },
        { "FULLSCREEN",         FRAME_STRATA_FULLSCREEN },
        { "FULLSCREEN_DIALOG",  FRAME_STRATA_FULLSCREEN_DIALOG },
        { "TOOLTIP",            FRAME_STRATA_TOOLTIP },
    };

    for (const auto& entry : frameStrataMap) {
        if (!SStrCmpI(entry.string, string)) {
            strata = entry.value;
            return true;
        }
    }

    return false;
}

int32_t StringToJustify(const char* string, uint32_t& justify) {
    struct JustifyEntry {
        const char* string;
        uint32_t value;
    };

    static JustifyEntry justifyMap[] = {
        { "LEFT",   0x1 },
        { "CENTER", 0x2 },
        { "RIGHT",  0x4 },
        { "TOP",    0x8 },
        { "MIDDLE", 0x10 },
        { "BOTTOM", 0x20 },
    };

    for (const auto& entry : justifyMap) {
        if (!SStrCmpI(entry.string, string)) {
            justify = entry.value;
            return true;
        }
    }

    return false;
}

int32_t StringToOrientation(const char* string, ORIENTATION& orientation) {
    struct OrientationEntry {
        const char* string;
        ORIENTATION value;
    };

    static OrientationEntry orientationMap[] = {
        { "HORIZONTAL", ORIENTATION_HORIZONTAL },
        { "VERTICAL",   ORIENTATION_VERTICAL },
    };

    for (const auto& entry : orientationMap) {
        if (!SStrCmpI(entry.string, string)) {
            orientation = entry.value;
            return true;
        }
    }

    return false;
}
