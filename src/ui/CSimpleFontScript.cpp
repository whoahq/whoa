#include "ui/CSimpleFontScript.hpp"
#include "ui/CSimpleFont.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t CSimpleFont_GetObjectType(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_IsObjectType(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_GetName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_SetFontObject(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_GetFontObject(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_CopyFontObject(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_SetFont(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_GetFont(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_SetAlpha(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_GetAlpha(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_SetTextColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_GetTextColor(const char* name, CSimpleFont* font, lua_State* L) {
    auto& color = font->m_attributes.m_color;

    lua_pushnumber(L, color.r / 255.0);
    lua_pushnumber(L, color.g / 255.0);
    lua_pushnumber(L, color.b / 255.0);
    lua_pushnumber(L, color.a / 255.0);

    return 4;
}

int32_t CSimpleFont_GetTextColor(lua_State* L) {
    auto type = CSimpleFont::GetObjectType();
    auto font = static_cast<CSimpleFont*>(FrameScript_GetObjectThis(L, type));

    return CSimpleFont_GetTextColor(font->GetDisplayName(), font, L);
}

int32_t CSimpleFont_SetShadowColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_GetShadowColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_SetShadowOffset(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_GetShadowOffset(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_SetSpacing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_GetSpacing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_SetJustifyH(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_GetJustifyH(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_SetJustifyV(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_GetJustifyV(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_SetIndentedWordWrap(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleFont_GetIndentedWordWrap(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

FrameScript_Method SimpleFontMethods[NUM_SIMPLE_FONT_SCRIPT_METHODS] = {
    { "GetObjectType",          &CSimpleFont_GetObjectType },
    { "IsObjectType",           &CSimpleFont_IsObjectType },
    { "GetName",                &CSimpleFont_GetName },
    { "SetFontObject",          &CSimpleFont_SetFontObject },
    { "GetFontObject",          &CSimpleFont_GetFontObject },
    { "CopyFontObject",         &CSimpleFont_CopyFontObject },
    { "SetFont",                &CSimpleFont_SetFont },
    { "GetFont",                &CSimpleFont_GetFont },
    { "SetAlpha",               &CSimpleFont_SetAlpha },
    { "GetAlpha",               &CSimpleFont_GetAlpha },
    { "SetTextColor",           &CSimpleFont_SetTextColor },
    { "GetTextColor",           &CSimpleFont_GetTextColor },
    { "SetShadowColor",         &CSimpleFont_SetShadowColor },
    { "GetShadowColor",         &CSimpleFont_GetShadowColor },
    { "SetShadowOffset",        &CSimpleFont_SetShadowOffset },
    { "GetShadowOffset",        &CSimpleFont_GetShadowOffset },
    { "SetSpacing",             &CSimpleFont_SetSpacing },
    { "GetSpacing",             &CSimpleFont_GetSpacing },
    { "SetJustifyH",            &CSimpleFont_SetJustifyH },
    { "GetJustifyH",            &CSimpleFont_GetJustifyH },
    { "SetJustifyV",            &CSimpleFont_SetJustifyV },
    { "GetJustifyV",            &CSimpleFont_GetJustifyV },
    { "SetIndentedWordWrap",    &CSimpleFont_SetIndentedWordWrap },
    { "GetIndentedWordWrap",    &CSimpleFont_GetIndentedWordWrap }
};
