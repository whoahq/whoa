#include "ui/CSimpleSliderScript.hpp"
#include "ui/CSimpleSlider.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t CSimpleSlider_GetThumbTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleSlider_SetThumbTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleSlider_GetOrientation(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleSlider_SetOrientation(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleSlider_GetMinMaxValues(lua_State* L) {
    auto type = CSimpleSlider::GetObjectType();
    auto slider = static_cast<CSimpleSlider*>(FrameScript_GetObjectThis(L, type));

    lua_pushnumber(L, slider->m_baseValue);
    lua_pushnumber(L, slider->m_range + slider->m_baseValue);

    return 2;
}

int32_t CSimpleSlider_SetMinMaxValues(lua_State* L) {
    auto type = CSimpleSlider::GetObjectType();
    auto slider = static_cast<CSimpleSlider*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3) || lua_tonumber(L, 2) > lua_tonumber(L, 3)) {
        luaL_error(L, "Usage: %s:SetMinMaxValues(min, max)", slider->GetDisplayName());
    }

    float min = lua_tonumber(L, 2);
    float max = lua_tonumber(L, 3);

    slider->SetMinMaxValues(min, max);

    return 0;
}

int32_t CSimpleSlider_GetValue(lua_State* L) {
    auto type = CSimpleSlider::GetObjectType();
    auto slider = static_cast<CSimpleSlider*>(FrameScript_GetObjectThis(L, type));

    float value = slider->m_value;

    lua_pushnumber(L, value);

    return 1;
}

int32_t CSimpleSlider_SetValue(lua_State* L) {
    auto type = CSimpleSlider::GetObjectType();
    auto slider = static_cast<CSimpleSlider*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isnumber(L, 2)) {
        luaL_error(L, "Usage: %s:SetValue(value)", slider->GetDisplayName());
    }

    float value = lua_tonumber(L, 2);
    slider->SetValue(value);

    return 0;
}

int32_t CSimpleSlider_GetValueStep(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleSlider_SetValueStep(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleSlider_Enable(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleSlider_Disable(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleSlider_IsEnabled(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

FrameScript_Method SimpleSliderMethods[NUM_SIMPLE_SLIDER_SCRIPT_METHODS] = {
    { "GetThumbTexture",    &CSimpleSlider_GetThumbTexture },
    { "SetThumbTexture",    &CSimpleSlider_SetThumbTexture },
    { "GetOrientation",     &CSimpleSlider_GetOrientation },
    { "SetOrientation",     &CSimpleSlider_SetOrientation },
    { "GetMinMaxValues",    &CSimpleSlider_GetMinMaxValues },
    { "SetMinMaxValues",    &CSimpleSlider_SetMinMaxValues },
    { "GetValue",           &CSimpleSlider_GetValue },
    { "SetValue",           &CSimpleSlider_SetValue },
    { "GetValueStep",       &CSimpleSlider_GetValueStep },
    { "SetValueStep",       &CSimpleSlider_SetValueStep },
    { "Enable",             &CSimpleSlider_Enable },
    { "Disable",            &CSimpleSlider_Disable },
    { "IsEnabled",          &CSimpleSlider_IsEnabled }
};
