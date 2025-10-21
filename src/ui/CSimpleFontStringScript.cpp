#include "ui/CSimpleFontStringScript.hpp"
#include "ui/CSimpleFont.hpp"
#include "ui/CSimpleFontString.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t CSimpleFontString_IsObjectType(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetObjectType(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetDrawLayer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetDrawLayer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetVertexColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetAlpha(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetAlpha(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetAlphaGradient(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_Show(lua_State* L) {
    auto type = CSimpleFontString::GetObjectType();
    auto string = static_cast<CSimpleFontString*>(FrameScript_GetObjectThis(L, type));

    string->Show();

    return 0;
}

int32_t CSimpleFontString_Hide(lua_State* L) {
    auto type = CSimpleFontString::GetObjectType();
    auto string = static_cast<CSimpleFontString*>(FrameScript_GetObjectThis(L, type));

    string->Hide();

    return 0;
}

int32_t CSimpleFontString_IsVisible(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_IsShown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetFontObject(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetFontObject(lua_State* L) {
    auto type = CSimpleFontString::GetObjectType();
    auto string = static_cast<CSimpleFontString*>(FrameScript_GetObjectThis(L, type));

    CSimpleFont* font = nullptr;

    if (lua_type(L, 2) == LUA_TTABLE) {
        lua_rawgeti(L, 2, 0);
        font = static_cast<CSimpleFont*>(lua_touserdata(L, -1));
        lua_settop(L, -2);

        if (!font) {
            luaL_error(L, "%s:SetFontObject(): Couldn't find 'this' in font object", string->GetDisplayName());
        }

        if (!font->IsA(CSimpleFont::GetObjectType())) {
            luaL_error(L, "%s:SetFontObject(): Wrong object type, expected font", string->GetDisplayName());
        }
    } else if (lua_type(L, 2) == LUA_TSTRING) {
        auto fontName = lua_tostring(L, 2);
        font = CSimpleFont::GetFont(fontName, 0);

        if (!font) {
            luaL_error(L, "%s:SetFontObject(): Couldn't find font named %s", string->GetDisplayName(), fontName);
        }
    } else if (lua_type(L, 2) != LUA_TNIL) {
        luaL_error(L, "Usage: %s:SetFontObject(font or \"font\" or nil)", string->GetDisplayName());
    }

    string->SetFontObject(font);

    return 0;
}

int32_t CSimpleFontString_GetFont(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetFont(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetText(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetFieldSize(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetText(lua_State* L) {
    auto type = CSimpleFontString::GetObjectType();
    auto string = static_cast<CSimpleFontString*>(FrameScript_GetObjectThis(L, type));

    if (!string->m_font) {
        luaL_error(L, "%s:SetText(): Font not set", string->GetDisplayName());
    }

    const char* text = lua_tostring(L, 2);
    string->SetText(text, 1);

    return 0;
}

int32_t CSimpleFontString_SetFormattedText(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetTextColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetTextColor(lua_State* L) {
    auto type = CSimpleFontString::GetObjectType();
    auto string = static_cast<CSimpleFontString*>(FrameScript_GetObjectThis(L, type));

    CImVector color = { 0x00, 0x00, 0x00, 0x00 };
    FrameScript_GetColor(L, 2, color);

    string->SetVertexColor(color);
    string->m_fontableFlags &= ~FLAG_COLOR_UPDATE;

    return 0;
}

int32_t CSimpleFontString_GetShadowColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetShadowColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetShadowOffset(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetShadowOffset(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetSpacing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetSpacing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetTextHeight(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetStringWidth(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetStringHeight(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetJustifyH(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetJustifyH(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetJustifyV(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetJustifyV(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_CanNonSpaceWrap(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetNonSpaceWrap(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_CanWordWrap(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetWordWrap(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_GetIndentedWordWrap(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFontString_SetIndentedWordWrap(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

FrameScript_Method SimpleFontStringMethods[NUM_SIMPLE_FONT_STRING_SCRIPT_METHODS] = {
    { "IsObjectType",           &CSimpleFontString_IsObjectType },
    { "GetObjectType",          &CSimpleFontString_GetObjectType },
    { "GetDrawLayer",           &CSimpleFontString_GetDrawLayer },
    { "SetDrawLayer",           &CSimpleFontString_SetDrawLayer },
    { "SetVertexColor",         &CSimpleFontString_SetVertexColor },
    { "GetAlpha",               &CSimpleFontString_GetAlpha },
    { "SetAlpha",               &CSimpleFontString_SetAlpha },
    { "SetAlphaGradient",       &CSimpleFontString_SetAlphaGradient },
    { "Show",                   &CSimpleFontString_Show },
    { "Hide",                   &CSimpleFontString_Hide },
    { "IsVisible",              &CSimpleFontString_IsVisible },
    { "IsShown",                &CSimpleFontString_IsShown },
    { "GetFontObject",          &CSimpleFontString_GetFontObject },
    { "SetFontObject",          &CSimpleFontString_SetFontObject },
    { "GetFont",                &CSimpleFontString_GetFont },
    { "SetFont",                &CSimpleFontString_SetFont },
    { "GetText",                &CSimpleFontString_GetText },
    { "GetFieldSize",           &CSimpleFontString_GetFieldSize },
    { "SetText",                &CSimpleFontString_SetText },
    { "SetFormattedText",       &CSimpleFontString_SetFormattedText },
    { "GetTextColor",           &CSimpleFontString_GetTextColor },
    { "SetTextColor",           &CSimpleFontString_SetTextColor },
    { "GetShadowColor",         &CSimpleFontString_GetShadowColor },
    { "SetShadowColor",         &CSimpleFontString_SetShadowColor },
    { "GetShadowOffset",        &CSimpleFontString_GetShadowOffset },
    { "SetShadowOffset",        &CSimpleFontString_SetShadowOffset },
    { "GetSpacing",             &CSimpleFontString_GetSpacing },
    { "SetSpacing",             &CSimpleFontString_SetSpacing },
    { "SetTextHeight",          &CSimpleFontString_SetTextHeight },
    { "GetStringWidth",         &CSimpleFontString_GetStringWidth },
    { "GetStringHeight",        &CSimpleFontString_GetStringHeight },
    { "GetJustifyH",            &CSimpleFontString_GetJustifyH },
    { "SetJustifyH",            &CSimpleFontString_SetJustifyH },
    { "GetJustifyV",            &CSimpleFontString_GetJustifyV },
    { "SetJustifyV",            &CSimpleFontString_SetJustifyV },
    { "CanNonSpaceWrap",        &CSimpleFontString_CanNonSpaceWrap },
    { "SetNonSpaceWrap",        &CSimpleFontString_SetNonSpaceWrap },
    { "CanWordWrap",            &CSimpleFontString_CanWordWrap },
    { "SetWordWrap",            &CSimpleFontString_SetWordWrap },
    { "GetIndentedWordWrap",    &CSimpleFontString_GetIndentedWordWrap },
    { "SetIndentedWordWrap",    &CSimpleFontString_SetIndentedWordWrap }
};
