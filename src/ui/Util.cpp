#include "ui/Util.hpp"
#include "util/Lua.hpp"
#include <type_traits>
#include <storm/String.hpp>

const char* LanguageProcess(const char* string) {
    // TODO
    return string;
}

int32_t StringToBlendMode(const char* string, EGxBlend& blend) {
    struct BlendEntry {
        EGxBlend value;
        const char* string;
    };

    static BlendEntry blendMap[] = {
        { GxBlend_Opaque,   "DISABLE" },
        { GxBlend_Alpha,    "BLEND" },
        { GxBlend_AlphaKey, "ALPHAKEY" },
        { GxBlend_Add,      "ADD" },
        { GxBlend_Mod,      "MOD" }
    };

    for (int32_t i = 0; i < std::extent<decltype(blendMap)>::value; i++) {
        if (!SStrCmpI(blendMap[i].string, string, 0x7FFFFFFFu)) {
            blend = blendMap[i].value;
            return 1;
        }
    }

    return 0;
}

int32_t StringToBOOL(const char* string) {
    return StringToBOOL(string, 0);
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
            if (!SStrCmpI(string, "off", 0x7FFFFFFFu) || !SStrCmpI(string, "disabled", 0x7FFFFFFFu)) {
                return false;
            }

            if (!SStrCmpI(string, "on", 0x7FFFFFFFu) || !SStrCmpI(string, "enabled", 0x7FFFFFFFu)) {
                return true;
            }

            return def;
    }
}

bool StringToBOOL(lua_State* L, int32_t idx, int32_t def) {
    bool result;
    const char* str;

    switch (lua_type(L, idx)) {
        case LUA_TNIL:
            result = false;
            break;

        case LUA_TBOOLEAN:
            result = lua_toboolean(L, idx);
            break;

        case LUA_TNUMBER:
            result = lua_tonumber(L, idx) != 0;
            break;

        case LUA_TSTRING:
            str = lua_tostring(L, idx);
            result = StringToBOOL(str, def);
            break;

        default:
            result = def;
            break;
    }

    return result;
}

uint64_t StringToClickAction(const char* string) {
    if (!string || !*string) {
        return 0;
    }

    if (!SStrCmpI(string, "LeftButtonDown", STORM_MAX_STR)) {
        return 1;
    }

    if (!SStrCmpI(string, "LeftButtonUp", STORM_MAX_STR)) {
        return 0x80000000;
    }

    if (!SStrCmpI(string, "MiddleButtonDown", STORM_MAX_STR)) {
        return 2;
    }

    if (!SStrCmpI(string, "MiddleButtonUp", STORM_MAX_STR)) {
        return 0;
    }

    if (!SStrCmpI(string, "RightButtonDown", STORM_MAX_STR)) {
        return 4;
    }

    if (!SStrCmpI(string, "RightButtonUp", STORM_MAX_STR)) {
        return 0;
    }

    // TODO remaining buttons

    return 0;
}

int32_t StringToDrawLayer(const char* string, int32_t& layer) {
    struct LayerEntry {
        int32_t layer;
        const char* string;
    };

    static LayerEntry layerMap[] = {
        { 0, "BACKGROUND" },
        { 1, "BORDER" },
        { 2, "ARTWORK" },
        { 3, "OVERLAY" },
        { 4, "HIGHLIGHT" }
    };

    for (const auto& entry : layerMap) {
        if (!SStrCmpI(entry.string, string)) {
            layer = entry.layer;
            return 1;
        }
    }

    return 0;
}

int32_t StringToFramePoint(const char* string, FRAMEPOINT& point) {
    struct FramePointEntry {
        FRAMEPOINT value;
        const char* string;
    };

    static FramePointEntry framePointMap[] = {
        { FRAMEPOINT_BOTTOM,        "BOTTOM" },
        { FRAMEPOINT_BOTTOMLEFT,    "BOTTOMLEFT" },
        { FRAMEPOINT_BOTTOMRIGHT,   "BOTTOMRIGHT" },
        { FRAMEPOINT_CENTER,        "CENTER" },
        { FRAMEPOINT_TOP,           "TOP" },
        { FRAMEPOINT_TOPRIGHT,      "TOPRIGHT" },
        { FRAMEPOINT_TOPLEFT,       "TOPLEFT" },
        { FRAMEPOINT_LEFT,          "LEFT" },
        { FRAMEPOINT_RIGHT,         "RIGHT" }
    };

    for (int32_t i = 0; i < std::extent<decltype(framePointMap)>::value; i++) {
        if (!SStrCmpI(framePointMap[i].string, string, 0x7FFFFFFFu)) {
            point = framePointMap[i].value;
            return 1;
        }
    }

    return 0;
}

int32_t StringToFrameStrata(const char* string, FRAME_STRATA& strata) {
    struct FrameStrataEntry {
        FRAME_STRATA value;
        const char* string;
    };

    // FRAME_STRATA_WORLD is hardcoded
    static FrameStrataEntry frameStrataMap[] = {
        { FRAME_STRATA_BACKGROUND,      "BACKGROUND" },
        { FRAME_STRATA_LOW,             "LOW" },
        { FRAME_STRATA_MEDIUM,          "MEDIUM" },
        { FRAME_STRATA_HIGH,            "HIGH" },
        { FRAME_STRATA_DIALOG,          "DIALOG" },
        { FRAME_STRATA_FULLSCREEN,      "FULLSCREEN" },
        { FRAME_STRATA_FULLSCREEN_DIALOG, "FULLSCREEN_DIALOG" },
        { FRAME_STRATA_TOOLTIP,         "TOOLTIP" }
    };

    for (int32_t i = 0; i < std::extent<decltype(frameStrataMap)>::value; i++) {
        if (!SStrCmpI(frameStrataMap[i].string, string, 0x7FFFFFFFu)) {
            strata = frameStrataMap[i].value;
            return 1;
        }
    }

    return 0;
}

int32_t StringToJustify(const char* string, uint32_t& justify) {
    struct JustifyEntry {
        uint32_t value;
        const char* string;
    };

    static JustifyEntry justifyMap[] = {
        { 0x1,  "LEFT" },
        { 0x2,  "CENTER" },
        { 0x4,  "RIGHT" },
        { 0x8,  "TOP" },
        { 0x10, "MIDDLE" },
        { 0x20, "BOTTOM" }
    };

    for (const auto& entry : justifyMap) {
        if (!SStrCmpI(entry.string, string)) {
            justify = entry.value;
            return 1;
        }
    }

    return 0;
}

int32_t StringToOrientation(const char* string, ORIENTATION& orientation) {
    struct OrientationEntry {
        ORIENTATION value;
        const char* string;
    };

    static OrientationEntry orientationMap[] = {
        { ORIENTATION_HORIZONTAL,   "HORIZONTAL"    },
        { ORIENTATION_VERTICAL,     "VERTICAL"      },
    };

    for (auto& entry : orientationMap) {
        if (!SStrCmpI(entry.string, string)) {
            orientation = entry.value;
            return 1;
        }
    }

    return 0;
}
