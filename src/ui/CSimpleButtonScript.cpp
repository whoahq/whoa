#include "ui/CSimpleButtonScript.hpp"
#include "gx/Coordinate.hpp"
#include "ui/CSimpleButton.hpp"
#include "ui/CSimpleFontString.hpp"
#include "ui/CSimpleTexture.hpp"
#include "util/Lua.hpp"
#include "util/StringTo.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t CSimpleButton_SetStateTexture(lua_State* L, CSimpleButtonState state, const char* method) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    if (lua_type(L, 2) == LUA_TTABLE) {
        lua_rawgeti(L, 2, 0);
        auto texture = static_cast<CSimpleTexture*>(lua_touserdata(L, -1));
        lua_settop(L, -2);

        if (!texture) {
            luaL_error(L, "%s:%s(): Couldn't find 'this' in texture", button->GetDisplayName(), method);
        }

        if (!texture->IsA(CSimpleTexture::GetObjectType())) {
            luaL_error(L, "%s:%s(): Wrong object type, expected texture", button->GetDisplayName(), method);
        }

        button->SetStateTexture(state, texture);

    } else if (lua_isstring(L, 2)) {
        auto texFile = lua_tolstring(L, 2, nullptr);
        button->SetStateTexture(state, texFile);

    } else if (lua_type(L, 2) == LUA_TNIL) {
        CSimpleTexture* texture = nullptr;
        button->SetStateTexture(state, texture);

    } else {
        luaL_error(L, "Usage: %s:%s(texture or \"texture\" or nil)", button->GetDisplayName(), method);
    }

    return 0;
}

int32_t CSimpleButton_Enable(lua_State* L) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    if (button->ProtectedFunctionsAllowed()) {
        button->Enable(1);
    } else {
        // TODO
        // - disallowed logic
    }

    return 0;
}

int32_t CSimpleButton_Disable(lua_State* L) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    if (button->ProtectedFunctionsAllowed()) {
        button->Enable(0);
    } else {
        // TODO
        // - disallowed logic
    }

    return 0;
}

int32_t CSimpleButton_IsEnabled(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_GetButtonState(lua_State* L) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    auto buttonState = button->GetButtonState();

    if (buttonState == BUTTONSTATE_DISABLED) {
        lua_pushstring(L, "DISABLED");
        return 1;
    } else if (buttonState == BUTTONSTATE_PUSHED) {
        lua_pushstring(L, "PUSHED");
        return 1;
    } else if (buttonState == BUTTONSTATE_NORMAL) {
        lua_pushstring(L, "NORMAL");
        return 1;
    }

    lua_pushstring(L, "UNKNOWN");
    return 1;
}

int32_t CSimpleButton_SetButtonState(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_SetNormalFontObject(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_GetNormalFontObject(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_SetDisabledFontObject(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_GetDisabledFontObject(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_SetHighlightFontObject(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_GetHighlightFontObject(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_SetFontString(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_GetFontString(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_SetText(lua_State* L) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    const char* text = lua_tolstring(L, 2, 0);
    button->SetText(text);

    return 0;
}

int32_t CSimpleButton_SetFormattedText(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_GetText(lua_State* L) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    const char* text = nullptr;

    if (button->m_text && button->m_text->GetText() && *button->m_text->GetText()) {
        text = button->m_text->GetText();
    }

    lua_pushstring(L, text);

    return 1;
}

int32_t CSimpleButton_SetNormalTexture(lua_State* L) {
    return CSimpleButton_SetStateTexture(L, BUTTONSTATE_NORMAL, "SetNormalTexture");
}

int32_t CSimpleButton_GetNormalTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_SetPushedTexture(lua_State* L) {
    return CSimpleButton_SetStateTexture(L, BUTTONSTATE_PUSHED, "SetPushedTexture");
}

int32_t CSimpleButton_GetPushedTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_SetDisabledTexture(lua_State* L) {
    return CSimpleButton_SetStateTexture(L, BUTTONSTATE_DISABLED, "SetDisabledTexture");
}

int32_t CSimpleButton_GetDisabledTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_SetHighlightTexture(lua_State* L) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    EGxBlend blendMode = GxBlend_Add;
    if (lua_isstring(L, 3)) {
        auto blendString = lua_tolstring(L, 3, nullptr);
        StringToBlendMode(blendString, blendMode);
    }

    if (lua_type(L, 2) == LUA_TTABLE) {
        lua_rawgeti(L, 2, 0);
        auto texture = static_cast<CSimpleTexture*>(lua_touserdata(L, -1));
        lua_settop(L, -2);

        if (!texture) {
            luaL_error(L, "%s:SetHighlightTexture(): Couldn't find 'this' in texture", button->GetDisplayName());
        }

        if (!texture->IsA(CSimpleTexture::GetObjectType())) {
            luaL_error(L, "%s:SetHighlightTexture(): Wrong object type, expected texture", button->GetDisplayName());
        }

        button->SetHighlight(texture, blendMode);

    } else if (lua_isstring(L, 2)) {
        auto texFile = lua_tolstring(L, 2, nullptr);
        button->SetHighlight(texFile, blendMode);

    } else if (lua_type(L, 2) == LUA_TNIL) {
        CSimpleTexture* texture = nullptr;
        button->SetHighlight(texture, GxBlend_Add);

    } else {
        luaL_error(L, "Usage: %s:SetHighlightTexture(texture or \"texture\" or nil [, \"blendmode\")", button->GetDisplayName());
    }

    return 0;
}

int32_t CSimpleButton_GetHighlightTexture(lua_State* L) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    auto texture = button->m_highlightTexture;

    if (texture) {
        if (!texture->lua_registered) {
            texture->RegisterScriptObject(nullptr);
        }

        lua_rawgeti(L, LUA_REGISTRYINDEX, texture->lua_objectRef);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

int32_t CSimpleButton_SetPushedTextOffset(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_GetPushedTextOffset(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_GetTextWidth(lua_State* L) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    auto text = button->m_text;

    float width = text ? text->GetWidth() : 0.0f;
    float ddcWidth = CoordinateGetAspectCompensation() * 1024.0f * width;
    float ndcWidth = DDCToNDCWidth(ddcWidth);

    lua_pushnumber(L, ndcWidth);

    return 1;
}

int32_t CSimpleButton_GetTextHeight(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_RegisterForClicks(lua_State* L) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    uint64_t action = 0;

    for (int32_t i = 2; lua_isstring(L, i); i++) {
        auto actionStr = lua_tolstring(L, i, nullptr);
        action |= StringToClickAction(actionStr);
    }

    button->SetClickAction(action);

    return 0;
}

int32_t CSimpleButton_Click(lua_State* L) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    auto inputButton = "LeftButton";
    if (lua_isstring(L, 2)) {
        inputButton = lua_tolstring(L, 2, nullptr);
    }

    auto v6 = StringToBOOL(L, 3, 0);

    button->OnClick(inputButton, v6);

    return 0;
}

int32_t CSimpleButton_LockHighlight(lua_State* L) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    button->LockHighlight(1);

    return 0;
}

int32_t CSimpleButton_UnlockHighlight(lua_State* L) {
    auto type = CSimpleButton::GetObjectType();
    auto button = static_cast<CSimpleButton*>(FrameScript_GetObjectThis(L, type));

    button->LockHighlight(0);

    return 0;
}

int32_t CSimpleButton_GetMotionScriptsWhileDisabled(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleButton_SetMotionScriptsWhileDisabled(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

FrameScript_Method SimpleButtonMethods[NUM_SIMPLE_BUTTON_SCRIPT_METHODS] = {
    { "Enable",                     &CSimpleButton_Enable },
    { "Disable",                    &CSimpleButton_Disable },
    { "IsEnabled",                  &CSimpleButton_IsEnabled },
    { "GetButtonState",             &CSimpleButton_GetButtonState },
    { "SetButtonState",             &CSimpleButton_SetButtonState },
    { "SetNormalFontObject",        &CSimpleButton_SetNormalFontObject },
    { "GetNormalFontObject",        &CSimpleButton_GetNormalFontObject },
    { "SetDisabledFontObject",      &CSimpleButton_SetDisabledFontObject },
    { "GetDisabledFontObject",      &CSimpleButton_GetDisabledFontObject },
    { "SetHighlightFontObject",     &CSimpleButton_SetHighlightFontObject },
    { "GetHighlightFontObject",     &CSimpleButton_GetHighlightFontObject },
    { "SetFontString",              &CSimpleButton_SetFontString },
    { "GetFontString",              &CSimpleButton_GetFontString },
    { "SetText",                    &CSimpleButton_SetText },
    { "SetFormattedText",           &CSimpleButton_SetFormattedText },
    { "GetText",                    &CSimpleButton_GetText },
    { "SetNormalTexture",           &CSimpleButton_SetNormalTexture },
    { "GetNormalTexture",           &CSimpleButton_GetNormalTexture },
    { "SetPushedTexture",           &CSimpleButton_SetPushedTexture },
    { "GetPushedTexture",           &CSimpleButton_GetPushedTexture },
    { "SetDisabledTexture",         &CSimpleButton_SetDisabledTexture },
    { "GetDisabledTexture",         &CSimpleButton_GetDisabledTexture },
    { "SetHighlightTexture",        &CSimpleButton_SetHighlightTexture },
    { "GetHighlightTexture",        &CSimpleButton_GetHighlightTexture },
    { "SetPushedTextOffset",        &CSimpleButton_SetPushedTextOffset },
    { "GetPushedTextOffset",        &CSimpleButton_GetPushedTextOffset },
    { "GetTextWidth",               &CSimpleButton_GetTextWidth },
    { "GetTextHeight",              &CSimpleButton_GetTextHeight },
    { "RegisterForClicks",          &CSimpleButton_RegisterForClicks },
    { "Click",                      &CSimpleButton_Click },
    { "LockHighlight",              &CSimpleButton_LockHighlight },
    { "UnlockHighlight",            &CSimpleButton_UnlockHighlight },
    { "GetMotionScriptsWhileDisabled", &CSimpleButton_GetMotionScriptsWhileDisabled },
    { "SetMotionScriptsWhileDisabled", &CSimpleButton_SetMotionScriptsWhileDisabled }
};
