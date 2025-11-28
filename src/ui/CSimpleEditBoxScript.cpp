#include "ui/CSimpleEditBoxScript.hpp"
#include "ui/CSimpleEditBox.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t CSimpleEditBox_SetFontObject(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetFontObject(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetFont(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetFont(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetTextColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetTextColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetShadowColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetShadowColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetShadowOffset(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetShadowOffset(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetSpacing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetSpacing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetJustifyH(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetJustifyH(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetJustifyV(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetJustifyV(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetIndentedWordWrap(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetIndentedWordWrap(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetAutoFocus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_IsAutoFocus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetCountInvisibleLetters(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_IsCountInvisibleLetters(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetMultiLine(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_IsMultiLine(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetNumeric(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_IsNumeric(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetPassword(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_IsPassword(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetBlinkSpeed(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetBlinkSpeed(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_Insert(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetText(lua_State* L) {
    auto type = CSimpleEditBox::GetObjectType();
    auto editBox = static_cast<CSimpleEditBox*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isstring(L, 2)) {
        return luaL_error(L, "Usage: %s:SetText(\"text\")", editBox->GetDisplayName());
    }

    // TODO lua_tainted
    auto text = lua_tostring(L, 2);
    editBox->SetText(text, nullptr);

    return 0;
}

int32_t CSimpleEditBox_GetText(lua_State* L) {
    int32_t type = CSimpleEditBox::GetObjectType();
    CSimpleEditBox* editBox = static_cast<CSimpleEditBox*>(FrameScript_GetObjectThis(L, type));

    // TODO
    // - taint management
    // if (editBox->m_dwordC && lua_taintexpected && !lua_taintedclosure) {
    //     lua_tainted = editBox->simpleeditbox_dwordC;
    // }

    lua_pushstring(L, editBox->m_text);

    return 1;
}

int32_t CSimpleEditBox_SetNumber(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetNumber(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_HighlightText(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_AddHistoryLine(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_ClearHistory(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetTextInsets(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetTextInsets(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetFocus(lua_State* L) {
    auto type = CSimpleEditBox::GetObjectType();
    auto editBox = static_cast<CSimpleEditBox*>(FrameScript_GetObjectThis(L, type));

    CSimpleEditBox::SetKeyboardFocus(editBox);

    return 0;
}

int32_t CSimpleEditBox_ClearFocus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_HasFocus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetMaxBytes(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetMaxBytes(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetMaxLetters(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetMaxLetters(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetNumLetters(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetHistoryLines(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetHistoryLines(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetInputLanguage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_ToggleInputLanguage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetAltArrowKeyMode(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetAltArrowKeyMode(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_IsInIMECompositionMode(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_SetCursorPosition(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetCursorPosition(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleEditBox_GetUTF8CursorPosition(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

FrameScript_Method SimpleEditBoxMethods[NUM_SIMPLE_EDIT_BOX_SCRIPT_METHODS] = {
    { "SetFontObject",                  &CSimpleEditBox_SetFontObject },
    { "GetFontObject",                  &CSimpleEditBox_GetFontObject },
    { "SetFont",                        &CSimpleEditBox_SetFont },
    { "GetFont",                        &CSimpleEditBox_GetFont },
    { "SetTextColor",                   &CSimpleEditBox_SetTextColor },
    { "GetTextColor",                   &CSimpleEditBox_GetTextColor },
    { "SetShadowColor",                 &CSimpleEditBox_SetShadowColor },
    { "GetShadowColor",                 &CSimpleEditBox_GetShadowColor },
    { "SetShadowOffset",                &CSimpleEditBox_SetShadowOffset },
    { "GetShadowOffset",                &CSimpleEditBox_GetShadowOffset },
    { "SetSpacing",                     &CSimpleEditBox_SetSpacing },
    { "GetSpacing",                     &CSimpleEditBox_GetSpacing },
    { "SetJustifyH",                    &CSimpleEditBox_SetJustifyH },
    { "GetJustifyH",                    &CSimpleEditBox_GetJustifyH },
    { "SetJustifyV",                    &CSimpleEditBox_SetJustifyV },
    { "GetJustifyV",                    &CSimpleEditBox_GetJustifyV },
    { "SetIndentedWordWrap",            &CSimpleEditBox_SetIndentedWordWrap },
    { "GetIndentedWordWrap",            &CSimpleEditBox_GetIndentedWordWrap },
    { "SetAutoFocus",                   &CSimpleEditBox_SetAutoFocus },
    { "IsAutoFocus",                    &CSimpleEditBox_IsAutoFocus },
    { "SetCountInvisibleLetters",       &CSimpleEditBox_SetCountInvisibleLetters },
    { "IsCountInvisibleLetters",        &CSimpleEditBox_IsCountInvisibleLetters },
    { "SetMultiLine",                   &CSimpleEditBox_SetMultiLine },
    { "IsMultiLine",                    &CSimpleEditBox_IsMultiLine },
    { "SetNumeric",                     &CSimpleEditBox_SetNumeric },
    { "IsNumeric",                      &CSimpleEditBox_IsNumeric },
    { "SetPassword",                    &CSimpleEditBox_SetPassword },
    { "IsPassword",                     &CSimpleEditBox_IsPassword },
    { "SetBlinkSpeed",                  &CSimpleEditBox_SetBlinkSpeed },
    { "GetBlinkSpeed",                  &CSimpleEditBox_GetBlinkSpeed },
    { "Insert",                         &CSimpleEditBox_Insert },
    { "SetText",                        &CSimpleEditBox_SetText },
    { "GetText",                        &CSimpleEditBox_GetText },
    { "SetNumber",                      &CSimpleEditBox_SetNumber },
    { "GetNumber",                      &CSimpleEditBox_GetNumber },
    { "HighlightText",                  &CSimpleEditBox_HighlightText },
    { "AddHistoryLine",                 &CSimpleEditBox_AddHistoryLine },
    { "ClearHistory",                   &CSimpleEditBox_ClearHistory },
    { "SetTextInsets",                  &CSimpleEditBox_SetTextInsets },
    { "GetTextInsets",                  &CSimpleEditBox_GetTextInsets },
    { "SetFocus",                       &CSimpleEditBox_SetFocus },
    { "ClearFocus",                     &CSimpleEditBox_ClearFocus },
    { "HasFocus",                       &CSimpleEditBox_HasFocus },
    { "SetMaxBytes",                    &CSimpleEditBox_SetMaxBytes },
    { "GetMaxBytes",                    &CSimpleEditBox_GetMaxBytes },
    { "SetMaxLetters",                  &CSimpleEditBox_SetMaxLetters },
    { "GetMaxLetters",                  &CSimpleEditBox_GetMaxLetters },
    { "GetNumLetters",                  &CSimpleEditBox_GetNumLetters },
    { "GetHistoryLines",                &CSimpleEditBox_GetHistoryLines },
    { "SetHistoryLines",                &CSimpleEditBox_SetHistoryLines },
    { "GetInputLanguage",               &CSimpleEditBox_GetInputLanguage },
    { "ToggleInputLanguage",            &CSimpleEditBox_ToggleInputLanguage },
    { "SetAltArrowKeyMode",             &CSimpleEditBox_SetAltArrowKeyMode },
    { "GetAltArrowKeyMode",             &CSimpleEditBox_GetAltArrowKeyMode },
    { "IsInIMECompositionMode",         &CSimpleEditBox_IsInIMECompositionMode },
    { "SetCursorPosition",              &CSimpleEditBox_SetCursorPosition },
    { "GetCursorPosition",              &CSimpleEditBox_GetCursorPosition },
    { "GetUTF8CursorPosition",          &CSimpleEditBox_GetUTF8CursorPosition }
};
