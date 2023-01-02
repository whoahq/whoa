#include "util/StringTo.hpp"
#include "util/Lua.hpp"
#include <storm/String.hpp>

uint64_t StringToClickAction(const char* actionStr) {
    if (!actionStr || !*actionStr) {
        return 0;
    }

    if (!SStrCmpI(actionStr, "LeftButtonDown", STORM_MAX_STR)) {
        return 1;
    }

    if (!SStrCmpI(actionStr, "LeftButtonUp", STORM_MAX_STR)) {
        return 0x80000000;
    }

    if (!SStrCmpI(actionStr, "MiddleButtonDown", STORM_MAX_STR)) {
        return 2;
    }

    if (!SStrCmpI(actionStr, "MiddleButtonUp", STORM_MAX_STR)) {
        return 0;
    }

    if (!SStrCmpI(actionStr, "RightButtonDown", STORM_MAX_STR)) {
        return 4;
    }

    if (!SStrCmpI(actionStr, "RightButtonUp", STORM_MAX_STR)) {
        return 0;
    }

    // TODO remaining buttons

    return 0;
}

int32_t StringToBOOL(const char* str) {
    return StringToBOOL(str, 0);
}

bool StringToBOOL(const char* str, int32_t def) {
    if (!str) {
        return def;
    }

    switch (*str) {
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
            if (!SStrCmpI(str, "off", 0x7FFFFFFFu) || !SStrCmpI(str, "disabled", 0x7FFFFFFFu)) {
                return false;
            }

            if (!SStrCmpI(str, "on", 0x7FFFFFFFu) || !SStrCmpI(str, "enabled", 0x7FFFFFFFu)) {
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
            str = lua_tolstring(L, idx, 0);
            result = StringToBOOL(str, def);
            break;

        default:
            result = def;
            break;
    }

    return result;
}

int32_t StringToDrawLayer(const char* string, int32_t& layer) {
    struct drawlayer {
        int32_t layer;
        const char* string;
    };

    static drawlayer array_drawlayer[5] = {
        { 0, "BACKGROUND" },
        { 1, "BORDER" },
        { 2, "ARTWORK" },
        { 3, "OVERLAY" },
        { 4, "HIGHLIGHT" }
    };

    for (int32_t i = 0; i < 5; i++) {
        if (!SStrCmpI(array_drawlayer[i].string, string, 0x7FFFFFFFu)) {
            layer = array_drawlayer[i].layer;
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

    static JustifyEntry justifyMap[6] = {
        { 0x1,  "LEFT" },
        { 0x2,  "CENTER" },
        { 0x4,  "RIGHT" },
        { 0x8,  "TOP" },
        { 0x10, "MIDDLE" },
        { 0x20, "BOTTOM" }
    };

    for (int32_t i = 0; i < 5; i++) {
        if (!SStrCmpI(justifyMap[i].string, string, 0x7FFFFFFFu)) {
            justify = justifyMap[i].value;
            return 1;
        }
    }

    return 0;
}
