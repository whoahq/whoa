#include "ui/simple/CSimpleStatusBarScript.hpp"
#include "ui/simple/CSimpleStatusBar.hpp"
#include "ui/FrameScript.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t CSimpleStatusBar_GetOrientation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleStatusBar_SetOrientation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleStatusBar_GetMinMaxValues(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleStatusBar_SetMinMaxValues(lua_State* L) {
    auto type = CSimpleStatusBar::GetObjectType();
    auto statusBar = static_cast<CSimpleStatusBar*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3)) {
        luaL_error(L, "Usage: %s:SetMinMaxValues(min, max)", statusBar->GetDisplayName());
        return 0;
    }

    auto min = lua_tonumber(L, 2);
    auto max = lua_tonumber(L, 3);

    if (min < -1.0e12 || min > 1.0e12 || max < -1.0e12 || max > 1.0e12) {
        luaL_error(L, "Min or Max out of range");
        return 0;
    }

    if (max - min > 1.0e12) {
        luaL_error(L, "Min and Max too far apart");
        return 0;
    }

    statusBar->SetMinMaxValues(static_cast<float>(min), static_cast<float>(max));

    return 0;
}

int32_t CSimpleStatusBar_GetValue(lua_State* L) {
    auto type = CSimpleStatusBar::GetObjectType();
    auto statusBar = static_cast<CSimpleStatusBar*>(FrameScript_GetObjectThis(L, type));

    lua_pushnumber(L, statusBar->GetValue());

    return 1;
}

int32_t CSimpleStatusBar_SetValue(lua_State* L) {
    auto type = CSimpleStatusBar::GetObjectType();
    auto statusBar = static_cast<CSimpleStatusBar*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isnumber(L, 2)) {
        luaL_error(L, "Usage: %s:SetValue(value)", statusBar->GetDisplayName());
        return 0;
    }

    auto value = static_cast<float>(lua_tonumber(L, 2));

    statusBar->SetValue(value);

    return 0;
}

int32_t CSimpleStatusBar_GetStatusBarTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleStatusBar_SetStatusBarTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleStatusBar_GetStatusBarColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleStatusBar_SetStatusBarColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleStatusBar_GetRotatesTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleStatusBar_SetRotatesTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

FrameScript_Method SimpleStatusBarMethods[] = {
    { "GetOrientation",         &CSimpleStatusBar_GetOrientation },
    { "SetOrientation",         &CSimpleStatusBar_SetOrientation },
    { "GetMinMaxValues",        &CSimpleStatusBar_GetMinMaxValues },
    { "SetMinMaxValues",        &CSimpleStatusBar_SetMinMaxValues },
    { "GetValue",               &CSimpleStatusBar_GetValue },
    { "SetValue",               &CSimpleStatusBar_SetValue },
    { "GetStatusBarTexture",    &CSimpleStatusBar_GetStatusBarTexture },
    { "SetStatusBarTexture",    &CSimpleStatusBar_SetStatusBarTexture },
    { "GetStatusBarColor",      &CSimpleStatusBar_GetStatusBarColor },
    { "SetStatusBarColor",      &CSimpleStatusBar_SetStatusBarColor },
    { "GetRotatesTexture",      &CSimpleStatusBar_GetRotatesTexture },
    { "SetRotatesTexture",      &CSimpleStatusBar_SetRotatesTexture },
};
