#include "ui/CSimpleFrameScript.hpp"
#include "gx/Coordinate.hpp"
#include "ui/CBackdropGenerator.hpp"
#include "ui/CSimpleFrame.hpp"
#include "ui/FrameScript.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <algorithm>
#include <cstdint>
#include <limits>

int32_t CSimpleFrame_GetTitleRegion(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_CreateTitleRegion(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_CreateTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_CreateFontString(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetBoundsRect(lua_State* L) {
    int32_t type = CSimpleFrame::GetObjectType();
    CSimpleFrame* frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    CRect bounds = {
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        0.0f,
        0.0f
    };

    if (!frame->GetBoundsRect(bounds)) {
        return 0;
    }

    float ooScale = 1.0f / frame->m_layoutScale;

    float ddcTop = CoordinateGetAspectCompensation() * 1024.0f * ooScale * bounds.minX;
    float ndcTop = DDCToNDCWidth(ddcTop);
    lua_pushnumber(L, ndcTop);

    float ddcLeft = CoordinateGetAspectCompensation() * 1024.0f * ooScale * bounds.minY;
    float ndcLeft = DDCToNDCWidth(ddcLeft);
    lua_pushnumber(L, ndcLeft);

    float ddcWidth = CoordinateGetAspectCompensation() * 1024.0f * ooScale * (bounds.maxX - bounds.minX);
    float ndcWidth = DDCToNDCWidth(ddcWidth);
    lua_pushnumber(L, ndcWidth);

    float ddcHeight = CoordinateGetAspectCompensation() * 1024.0f * ooScale * (bounds.maxY - bounds.minY);
    float ndcHeight = DDCToNDCWidth(ddcHeight);
    lua_pushnumber(L, ndcHeight);

    return 4;
}

int32_t CSimpleFrame_GetNumRegions(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetRegions(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetNumChildren(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetChildren(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetFrameStrata(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetFrameStrata(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetFrameLevel(lua_State* L) {
    int32_t type = CSimpleFrame::GetObjectType();
    CSimpleFrame* frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    lua_pushnumber(L, frame->m_level);

    return 1;
}

int32_t CSimpleFrame_SetFrameLevel(lua_State* L) {
    int32_t type = CSimpleFrame::GetObjectType();
    CSimpleFrame* frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    if (!frame->ProtectedFunctionsAllowed()) {
        // TODO
        // - disallowed logic

        return 0;
    }

    if (!lua_isnumber(L, 2)) {
        return luaL_error(L, "Usage: %s:SetFrameLevel(level)", frame->GetDisplayName());
    }

    int32_t level = lua_tonumber(L, 2);

    if (level < 0) {
        return luaL_error(L, "%s:SetFrameLevel(): Passed negative frame level: %d", frame->GetDisplayName(), level);
    }

    frame->SetFrameLevel(level, 1);

    return 0;
}

int32_t CSimpleFrame_HasScript(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetScript(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetScript(lua_State* L) {
    auto type = CSimpleFrame::GetObjectType();
    auto frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    return frame->SetScript(L);
}

int32_t CSimpleFrame_HookScript(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_RegisterEvent(lua_State* L) {
    int32_t type = CSimpleFrame::GetObjectType();
    CSimpleFrame* frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isstring(L, 2)) {
        return luaL_error(L, "Usage: %s:RegisterEvent(\"event\")", frame->GetDisplayName());
    }

    const char* event = lua_tostring(L, 2);

    frame->RegisterScriptEvent(event);

    return 0;
}

int32_t CSimpleFrame_UnregisterEvent(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_RegisterAllEvents(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_UnregisterAllEvents(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_IsEventRegistered(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_AllowAttributeChanges(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_CanChangeAttributes(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetAttribute(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetAttribute(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetEffectiveScale(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetScale(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetScale(lua_State* L) {
    auto type = CSimpleFrame::GetObjectType();
    auto frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    if (!frame->ProtectedFunctionsAllowed()) {
        // TODO
        // - disallowed logic

        return 0;
    }

    if (!lua_isnumber(L, 2)) {
        return luaL_error(L, "Usage: %s:SetScale(scale)", frame->GetDisplayName());
    }

    auto scale = lua_tonumber(L, 2);

    if (scale <= 0.0) {
        return luaL_error(L, "%s:SetScale(): Scale must be > 0", frame->GetDisplayName());
    }

    frame->SetFrameScale(scale, false);

    return 0;
}

int32_t CSimpleFrame_GetEffectiveAlpha(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetAlpha(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetAlpha(lua_State* L) {
    auto type = CSimpleFrame::GetObjectType();
    auto frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isnumber(L, 2)) {
        luaL_error(L, "Usage: %s:SetAlpha(alpha 0 to 1)", frame->GetDisplayName());
    }

    float alpha = lua_tonumber(L, 2);
    alpha = std::max(std::min(alpha, 1.0f), 0.0f);
    frame->SetFrameAlpha(static_cast<uint8_t>(alpha * 255.0f));

    return 0;
}

int32_t CSimpleFrame_GetID(lua_State* L) {
    int32_t type = CSimpleFrame::GetObjectType();
    CSimpleFrame* frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    lua_pushnumber(L, frame->m_id);

    return 1;
}

int32_t CSimpleFrame_SetID(lua_State* L) {
    int32_t type = CSimpleFrame::GetObjectType();
    CSimpleFrame* frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    if (!frame->ProtectedFunctionsAllowed()) {
        // TODO
        // - disallowed logic

        return 0;
    }

    if (!lua_isnumber(L, 2)) {
        return luaL_error(L, "Usage: %s:SetID(ID)", frame->GetDisplayName());
    }

    frame->m_id = lua_tonumber(L, 2);

    return 0;
}

int32_t CSimpleFrame_SetToplevel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_IsToplevel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_EnableDrawLayer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_DisableDrawLayer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_Show(lua_State* L) {
    int32_t type = CSimpleFrame::GetObjectType();
    CSimpleFrame* frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    if (frame->ProtectedFunctionsAllowed()) {
        frame->Show();
    } else {
        // TODO
        // - disallowed logic
    }

    return 0;
}

int32_t CSimpleFrame_Hide(lua_State* L) {
    int32_t type = CSimpleFrame::GetObjectType();
    CSimpleFrame* frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    if (frame->ProtectedFunctionsAllowed()) {
        frame->Hide();
    } else {
        // TODO
        // - disallowed logic
    }

    return 0;
}

int32_t CSimpleFrame_IsVisible(lua_State* L) {
    auto type = CSimpleFrame::GetObjectType();
    auto frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    if (frame->m_visible) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

int32_t CSimpleFrame_IsShown(lua_State* L) {
    int32_t type = CSimpleFrame::GetObjectType();
    CSimpleFrame* frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    if (frame->m_shown) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

int32_t CSimpleFrame_Raise(lua_State* L) {
    int32_t type = CSimpleFrame::GetObjectType();
    CSimpleFrame* frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    if (!frame->ProtectedFunctionsAllowed()) {
        // TODO
        // - disallowed logic

        return 0;
    }

    frame->Raise();

    return 0;
}

int32_t CSimpleFrame_Lower(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetHitRectInsets(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetHitRectInsets(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetClampRectInsets(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetClampRectInsets(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetMinResize(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetMinResize(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetMaxResize(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetMaxResize(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetMovable(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_IsMovable(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetDontSavePosition(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetDontSavePosition(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetResizable(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_IsResizable(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_StartMoving(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_StartSizing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_StopMovingOrSizing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetUserPlaced(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_IsUserPlaced(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetClampedToScreen(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_IsClampedToScreen(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_RegisterForDrag(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_EnableKeyboard(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_IsKeyboardEnabled(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_EnableMouse(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_IsMouseEnabled(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_EnableMouseWheel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_IsMouseWheelEnabled(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_EnableJoystick(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_IsJoystickEnabled(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetBackdrop(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetBackdrop(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetBackdropColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetBackdropColor(lua_State* L) {
    auto type = CSimpleFrame::GetObjectType();
    auto frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    CImVector color = { 0x00 };
    FrameScript_GetColor(L, 2, color);

    if (frame->m_backdrop) {
        frame->m_backdrop->SetVertexColor(color);
    }

    return 0;
}

int32_t CSimpleFrame_GetBackdropBorderColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_SetBackdropBorderColor(lua_State* L) {
    auto type = CSimpleFrame::GetObjectType();
    auto frame = static_cast<CSimpleFrame*>(FrameScript_GetObjectThis(L, type));

    CImVector color = { 0x00 };
    FrameScript_GetColor(L, 2, color);

    if (frame->m_backdrop) {
        frame->m_backdrop->SetBorderVertexColor(color);
    }

    return 0;
}

int32_t CSimpleFrame_SetDepth(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetDepth(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_GetEffectiveDepth(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_IgnoreDepth(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFrame_IsIgnoringDepth(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

FrameScript_Method SimpleFrameMethods[NUM_SIMPLE_FRAME_SCRIPT_METHODS] = {
    { "GetTitleRegion",             &CSimpleFrame_GetTitleRegion },
    { "CreateTitleRegion",          &CSimpleFrame_CreateTitleRegion },
    { "CreateTexture",              &CSimpleFrame_CreateTexture },
    { "CreateFontString",           &CSimpleFrame_CreateFontString },
    { "GetBoundsRect",              &CSimpleFrame_GetBoundsRect },
    { "GetNumRegions",              &CSimpleFrame_GetNumRegions },
    { "GetRegions",                 &CSimpleFrame_GetRegions },
    { "GetNumChildren",             &CSimpleFrame_GetNumChildren },
    { "GetChildren",                &CSimpleFrame_GetChildren },
    { "GetFrameStrata",             &CSimpleFrame_GetFrameStrata },
    { "SetFrameStrata",             &CSimpleFrame_SetFrameStrata },
    { "GetFrameLevel",              &CSimpleFrame_GetFrameLevel },
    { "SetFrameLevel",              &CSimpleFrame_SetFrameLevel },
    { "HasScript",                  &CSimpleFrame_HasScript },
    { "GetScript",                  &CSimpleFrame_GetScript },
    { "SetScript",                  &CSimpleFrame_SetScript },
    { "HookScript",                 &CSimpleFrame_HookScript },
    { "RegisterEvent",              &CSimpleFrame_RegisterEvent },
    { "UnregisterEvent",            &CSimpleFrame_UnregisterEvent },
    { "RegisterAllEvents",          &CSimpleFrame_RegisterAllEvents },
    { "UnregisterAllEvents",        &CSimpleFrame_UnregisterAllEvents },
    { "IsEventRegistered",          &CSimpleFrame_IsEventRegistered },
    { "AllowAttributeChanges",      &CSimpleFrame_AllowAttributeChanges },
    { "CanChangeAttribute",         &CSimpleFrame_CanChangeAttributes },
    { "GetAttribute",               &CSimpleFrame_GetAttribute },
    { "SetAttribute",               &CSimpleFrame_SetAttribute },
    { "GetEffectiveScale",          &CSimpleFrame_GetEffectiveScale },
    { "GetScale",                   &CSimpleFrame_GetScale },
    { "SetScale",                   &CSimpleFrame_SetScale },
    { "GetEffectiveAlpha",          &CSimpleFrame_GetEffectiveAlpha },
    { "GetAlpha",                   &CSimpleFrame_GetAlpha },
    { "SetAlpha",                   &CSimpleFrame_SetAlpha },
    { "GetID",                      &CSimpleFrame_GetID },
    { "SetID",                      &CSimpleFrame_SetID },
    { "SetToplevel",                &CSimpleFrame_SetToplevel },
    { "IsToplevel",                 &CSimpleFrame_IsToplevel },
    { "EnableDrawLayer",            &CSimpleFrame_EnableDrawLayer },
    { "DisableDrawLayer",           &CSimpleFrame_DisableDrawLayer },
    { "Show",                       &CSimpleFrame_Show },
    { "Hide",                       &CSimpleFrame_Hide },
    { "IsVisible",                  &CSimpleFrame_IsVisible },
    { "IsShown",                    &CSimpleFrame_IsShown },
    { "Raise",                      &CSimpleFrame_Raise },
    { "Lower",                      &CSimpleFrame_Lower },
    { "GetHitRectInsets",           &CSimpleFrame_GetHitRectInsets },
    { "SetHitRectInsets",           &CSimpleFrame_SetHitRectInsets },
    { "GetClampRectInsets",         &CSimpleFrame_GetClampRectInsets },
    { "SetClampRectInsets",         &CSimpleFrame_SetClampRectInsets },
    { "GetMinResize",               &CSimpleFrame_GetMinResize },
    { "SetMinResize",               &CSimpleFrame_SetMinResize },
    { "GetMaxResize",               &CSimpleFrame_GetMaxResize },
    { "SetMaxResize",               &CSimpleFrame_SetMaxResize },
    { "SetMovable",                 &CSimpleFrame_SetMovable },
    { "IsMovable",                  &CSimpleFrame_IsMovable },
    { "SetDontSavePosition",        &CSimpleFrame_SetDontSavePosition },
    { "GetDontSavePosition",        &CSimpleFrame_GetDontSavePosition },
    { "SetResizable",               &CSimpleFrame_SetResizable },
    { "IsResizable",                &CSimpleFrame_IsResizable },
    { "StartMoving",                &CSimpleFrame_StartMoving },
    { "StartSizing",                &CSimpleFrame_StartSizing },
    { "StopMovingOrSizing",         &CSimpleFrame_StopMovingOrSizing },
    { "SetUserPlaced",              &CSimpleFrame_SetUserPlaced },
    { "IsUserPlaced",               &CSimpleFrame_IsUserPlaced },
    { "SetClampedToScreen",         &CSimpleFrame_SetClampedToScreen },
    { "IsClampedToScreen",          &CSimpleFrame_IsClampedToScreen },
    { "RegisterForDrag",            &CSimpleFrame_RegisterForDrag },
    { "EnableKeyboard",             &CSimpleFrame_EnableKeyboard },
    { "IsKeyboardEnabled",          &CSimpleFrame_IsKeyboardEnabled },
    { "EnableMouse",                &CSimpleFrame_EnableMouse },
    { "IsMouseEnabled",             &CSimpleFrame_IsMouseEnabled },
    { "EnableMouseWheel",           &CSimpleFrame_EnableMouseWheel },
    { "IsMouseWheelEnabled",        &CSimpleFrame_IsMouseWheelEnabled },
    { "EnableJoystick",             &CSimpleFrame_EnableJoystick },
    { "IsJoystickEnabled",          &CSimpleFrame_IsJoystickEnabled },
    { "GetBackdrop",                &CSimpleFrame_GetBackdrop },
    { "SetBackdrop",                &CSimpleFrame_SetBackdrop },
    { "GetBackdropColor",           &CSimpleFrame_GetBackdropColor },
    { "SetBackdropColor",           &CSimpleFrame_SetBackdropColor },
    { "GetBackdropBorderColor",     &CSimpleFrame_GetBackdropBorderColor },
    { "SetBackdropBorderColor",     &CSimpleFrame_SetBackdropBorderColor },
    { "SetDepth",                   &CSimpleFrame_SetDepth },
    { "GetDepth",                   &CSimpleFrame_GetDepth },
    { "GetEffectiveDepth",          &CSimpleFrame_GetEffectiveDepth },
    { "IgnoreDepth",                &CSimpleFrame_IgnoreDepth },
    { "IsIgnoringDepth",            &CSimpleFrame_IsIgnoringDepth }
};
