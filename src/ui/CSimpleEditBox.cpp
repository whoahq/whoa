#include "ui/CSimpleEditBox.hpp"
#include "gx/Gx.hpp"
#include "gx/Coordinate.hpp"
#include "ui/CSimpleEditBoxScript.hpp"
#include "ui/CSimpleFontedFrameFont.hpp"
#include "ui/CSimpleFontString.hpp"
#include "ui/CSimpleTexture.hpp"
#include "ui/CSimpleTop.hpp"
#include "ui/LoadXML.hpp"
#include "util/Byte.hpp"
#include "util/CStatus.hpp"
#include "util/Lua.hpp"
#include "util/StringTo.hpp"
#include <cstring>
#include <common/XML.hpp>
#include <storm/String.hpp>
#include <storm/Unicode.hpp>

CSimpleEditBox* CSimpleEditBox::s_currentFocus;
int32_t CSimpleEditBox::s_metatable;
int32_t CSimpleEditBox::s_objectType;

void CSimpleEditBox::CreateScriptMetaTable() {
    lua_State* L = FrameScript_GetContext();
    int32_t ref = FrameScript_Object::CreateScriptMetaTable(L, &CSimpleEditBox::RegisterScriptMethods);
    CSimpleEditBox::s_metatable = ref;
}

int32_t CSimpleEditBox::GetObjectType() {
    if (!CSimpleEditBox::s_objectType) {
        CSimpleEditBox::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CSimpleEditBox::s_objectType;
}

void CSimpleEditBox::RegisterScriptMethods(lua_State* L) {
    CSimpleFrame::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, SimpleEditBoxMethods, NUM_SIMPLE_EDIT_BOX_SCRIPT_METHODS);
}

void CSimpleEditBox::SetKeyboardFocus(CSimpleEditBox* editBox) {
    if (!editBox->m_visible) {
        return;
    }

    if (CSimpleEditBox::s_currentFocus == editBox) {
        return;
    }

    if (CSimpleEditBox::s_currentFocus) {
        // TODO
        // if (CSimpleEditBox::s_currentFocus->m_password) {
        //     nullsub_3();
        // }

        // TODO
        // if (!(CSimpleEditBox::s_currentFocus->simpleeditbox_dword4 & 0xC)) {
        //     CSimpleEditBox::s_currentFocus->Sub963390();
        // }

        CSimpleEditBox::s_currentFocus->RunOnEditFocusLostScript();
        CSimpleEditBox::s_currentFocus->m_dirtyFlags |= 0x4;
    }

    CSimpleEditBox::s_currentFocus = editBox;

    // TODO
    // if (editBox->m_password) {
    //     nullsub_3();
    // }

    // TODO
    // if (!(editBox->simpleeditbox_dword4 & 0xC)) {
    //     OsIMEEnable(1);
    // }

    editBox->RunOnEditFocusGainedScript();
    editBox->m_dirtyFlags |= 0x4;
}

CSimpleEditBox::CSimpleEditBox(CSimpleFrame* parent) : CSimpleFrame(parent) {
    this->m_autoFocus = 1;
    this->m_multiline = 0;
    this->m_numeric = 0;
    this->m_password = 0;
    this->m_ignoreArrows = 0;
    this->m_countInvisibleLetters = 0;

    char* text = static_cast<char*>(SMemAlloc(this->m_textSize, __FILE__, __LINE__, 0));
    text[0] = 0;
    this->m_text = text;

    uint32_t* textInfo = static_cast<uint32_t*>(SMemAlloc(sizeof(uint32_t) * this->m_textSize, __FILE__, __LINE__, 0));
    memset(textInfo, 0, sizeof(uint32_t) * this->m_textSize);
    this->m_textInfo = textInfo;

    auto fontMem = SMemAlloc(sizeof(CSimpleFontedFrameFont), __FILE__, __LINE__, 0x0);
    auto font = new (fontMem) CSimpleFontedFrameFont(this);
    this->m_font = font;

    this->m_font->m_attributes.SetJustifyH(0x1);

    // TODO
    // auto stringMem = CDataAllocator::GetData(CSimpleFontString::s_allocator, 0, __FILE__, __LINE__);
    auto stringMem = SMemAlloc(sizeof(CSimpleFontString), __FILE__, __LINE__, 0x0);
    auto string = new (stringMem) CSimpleFontString(this, DRAWLAYER_ARTWORK, 1);
    this->m_string = string;

    this->m_string->SetFontObject(this->m_font);

    this->SetMultiLine(0);

    for (int32_t i = 0; i < 3; i++) {
        // TODO
        // auto highlightMem = CDataAllocator::GetData(CSimpleTexture::s_allocator, 0, __FILE__, __LINE__);
        auto highlightMem = SMemAlloc(sizeof(CSimpleTexture), __FILE__, __LINE__, 0x0);
        auto highlight = new (highlightMem) CSimpleTexture(this, DRAWLAYER_ARTWORK, 0);
        this->m_highlight[i] = highlight;
    }

    CImVector highlightColor = { 0x60, 0x60, 0x60, 0xFF };

    for (int32_t i = 0; i < 3; i++) {
        this->m_highlight[i]->SetTexture(highlightColor);
    }

    // TODO
    // auto cursorMem = CDataAllocator::GetData(CSimpleTexture::s_allocator, 0, __FILE__, __LINE__);
    auto cursorMem = SMemAlloc(sizeof(CSimpleTexture), __FILE__, __LINE__, 0x0);
    auto cursor = new (cursorMem) CSimpleTexture(this, DRAWLAYER_ARTWORK_OVERLAY, 1);
    cursor->Hide();
    this->m_cursor = cursor;

    CImVector cursorColor = { 0xFF, 0xFF, 0xFF, 0xFF };
    this->m_cursor->SetTexture(cursorColor);

    for (auto& action : this->m_actions) {
        action.obj = nullptr;
    }

    this->EnableEvent(SIMPLE_EVENT_CHAR, -1);
    this->EnableEvent(SIMPLE_EVENT_KEY, -1);
    this->EnableEvent(SIMPLE_EVENT_MOUSE, -1);
}

int32_t CSimpleEditBox::ConvertCoordinateToIndex(float a2, float a3, int32_t& a4) {
    // TODO
    return 0;
}

void CSimpleEditBox::DeleteBackward(int32_t a2) {
    if (this->m_highlightLeft != this->m_highlightRight) {
        this->DeleteHighlight(a2);
        return;
    }

    if (this->m_cursorPos > 0) {
        int32_t len = this->GetNumToLen(this->m_cursorPos, -1, true);
        this->DeleteSubstring(this->m_cursorPos - len, this->m_cursorPos, a2);
    }
}

void CSimpleEditBox::DeleteForward(int32_t a2) {
    if (this->m_highlightLeft != this->m_highlightRight) {
        this->DeleteHighlight(a2);
        return;
    }

    if (this->m_cursorPos < this->m_textLength) {
        int32_t len = this->GetNumToLen(this->m_cursorPos, 1, true);
        this->DeleteSubstring(this->m_cursorPos, this->m_cursorPos + len, a2);
    }
}

void CSimpleEditBox::DeleteHighlight(int32_t a2) {
    // TODO
}

void CSimpleEditBox::DeleteSubstring(int32_t left, int32_t right, int32_t a4) {
    auto v5 = this->m_textInfo;
    auto v6 = left;

    if ((v5[left] & 0x80000000) != 0) {
        auto v7 = left;

        if (left >= 0) {
LABEL_3:
            auto v8 = BYTE2(v5[v6]);
            auto v9 = &v5[v6];

            if (v8 == 1 || v8 == 5 || v8 == 7) {
                if (v6 >= 0) {
LABEL_12:
                    auto v10 = BYTE2(v5[v6]);
                    auto v11 = &v5[v6];

                    if (v10 == 1 || v10 == 5 || v10 == 7) {
                        v7 = v6;

                        while (1) {
                            v6--;
                            v11--;

                            if (v6 < 0) {
                                break;
                            }

                            if (*v11) {
                                if (v6 >= 0) {
                                    goto LABEL_12;
                                }

                                break;
                            }
                        }
                    }
                }
            } else {
                v7 = v6;

                while (1) {
                    v6--;
                    v9--;

                    if (v6 < 0) {
                        break;
                    }

                    if (*v9) {
                        if (v6 >= 0) {
                            goto LABEL_3;
                        }

                        break;
                    }
                }
            }
        }

        v6 = v7;
    }

    auto v12 = right;
    if ((v5[right] & 0x80000000) != 0) {
        auto v13 = this->m_textLength;

        if (right < v13) {
            do {
                auto v14 = BYTE2(v5[v12]);

                if (v14 == 2 || v14 == 6 || v14 ==  8) {
                    break;
                }

                v12 += v5[v12] & 0xFFFF;
            } while (v12 < v13);

            for (; v12 < v13; v12 += v5[v12] & 0xFFFF) {
                auto v15 = BYTE2(v5[v12]);

                if (v15 != 2 && v15 != 6 && v15 != 8) {
                    break;
                }
            }
        }
    }

    auto v16 = v12 - v6;
    auto v17 = &this->m_text[v6];
    auto newLength = this->m_textLength - (v12 - v6);

    this->m_cursorPos = v6;

    auto v19 = &v5[v6];

    if (newLength >= 0) {
        this->m_textLength = newLength;

        memcpy(v17, &v17[v16], newLength - v6);
        memcpy(v19, &v19[v16], 4 * (this->m_textLength - this->m_cursorPos));

        this->m_text[this->m_textLength] = '\0';
        this->m_textInfo[this->m_textLength] = '\0';

        if (this->m_highlightLeft != this->m_highlightRight) {
            this->m_dirtyFlags |= 0x2;

            this->m_highlightRight = 0;
            this->m_highlightLeft = 0;
        }

        this->m_dirtyFlags |= 0x1 | 0x4;

        if (!a4) {
            this->m_dirtyFlags |= 0x8;
        }

        if (!this->m_textLength) {
            // TODO this->simpleeditbox_dwordC = 0;
        }
    }
}

void CSimpleEditBox::DispatchAction(int32_t action) {
    // TODO
}

void CSimpleEditBox::FontUpdated(CSimpleFontedFrameFont* font, int32_t a3) {
    // TODO
}

int32_t CSimpleEditBox::GetNumToLen(int32_t offset, int32_t amount, bool a4) {
    uint32_t len = 0;
    uint32_t* textInfo = this->m_textInfo + offset;

    if (amount > 0) {
        for (int32_t i = 0; i < amount; i++) {
            if (!*textInfo) {
                break;
            }

            uint32_t v8;
            uint32_t v9;
            uint32_t v10;

            do {
                do {
                    v8 = *((uint8_t*)textInfo + 2);
                    v9 = (uint16_t)*textInfo;
                    v10 = *textInfo & 0xFF000000;

                    len += v9;
                    textInfo += v9;
                } while (v8 == 1);
            } while (v8 == 5 || v8 == 7 || (a4 && v10 < 0 && v8 != 6));

            uint32_t v11;
            uint32_t v12;

            while (1) {
                v11 = *((uint8_t*)textInfo + 2);
                v12 = *textInfo & 0xFFFF;

                if (v11 != 2 && v11 != 6 && v11 != 8) {
                    break;
                }

                len += v12;
                textInfo += v12;
            }
        }
    } else {
        for (int32_t i = 0; i < -amount; i++) {
            if (textInfo <= this->m_textInfo) {
                break;
            }

            uint32_t v14;

            do {
                for (--textInfo; textInfo > this->m_textInfo; --textInfo) {
                    if (*textInfo) {
                        break;
                    }
                }

                v14 = *((uint8_t*)textInfo + 2);
                len += (uint16_t)*textInfo;
            } while (v14 == 2 || v14 == 6 || v14 == 8 || (a4 && (*textInfo & 0x80000000) != 0 && v14 != 5));

            if (textInfo > this->m_textInfo) {
                uint32_t v15;
                uint32_t v16;

                while (1) {
                    for (--textInfo; textInfo > this->m_textInfo; --textInfo) {
                        if (*textInfo) {
                            break;
                        }
                    }

                    v15 = *((uint8_t*)textInfo + 2);
                    v16 = *textInfo & 0xFFFF;

                    if (v15 != 1 && v15 != 5 && v15 != 7) {
                        break;
                    }

                    len += v16;

                    if (textInfo <= this->m_textInfo) {
                        break;
                    }
                }

                if (textInfo > this->m_textInfo) {
                    textInfo += v16;
                }
            }
        }
    }

    return len;
}

FrameScript_Object::ScriptIx* CSimpleEditBox::GetScriptByName(const char* name, ScriptData& data) {
    auto parentScript = CSimpleFrame::GetScriptByName(name, data);

    if (parentScript) {
        return parentScript;
    }

    if (!SStrCmpI(name, "OnEnterPressed")) {
        return &this->m_onEnterPressed;
    }

    if (!SStrCmpI(name, "OnEscapePressed")) {
        return &this->m_onEscapePressed;
    }

    if (!SStrCmpI(name, "OnSpacePressed")) {
        return &this->m_onSpacePressed;
    }

    if (!SStrCmpI(name, "OnTabPressed")) {
        return &this->m_onTabPressed;
    }

    if (!SStrCmpI(name, "OnTextChanged")) {
        data.wrapper = "return function(self, userInput) %s end";
        return &this->m_onTextChanged;
    }

    if (!SStrCmpI(name, "OnTextSet")) {
        return &this->m_onTextSet;
    }

    if (!SStrCmpI(name, "OnCursorChanged")) {
        data.wrapper = "return function(self,x,y,w,h) %s end";
        return &this->m_onCursorChanged;
    }

    if (!SStrCmpI(name, "OnInputLanguageChanged")) {
        data.wrapper = "return function(self,language) %s end";
        return &this->m_onInputLanguageChanged;
    }

    if (!SStrCmpI(name, "OnEditFocusGained")) {
        return &this->m_onEditFocusGained;
    }

    if (!SStrCmpI(name, "OnEditFocusLost")) {
        return &this->m_onEditFocusLost;
    }

    if (!SStrCmpI(name, "OnCharComposition")) {
        data.wrapper = "return function(self,text) %s end";
        return &this->m_onCharComposition;
    }

    return nullptr;
}

int32_t CSimpleEditBox::GetScriptMetaTable() {
    return CSimpleEditBox::s_metatable;
}

void CSimpleEditBox::GrowText(int32_t size) {
    if (size + 1 <= this->m_textSize) {
        return;
    }

    this->m_textSize = (size + 32) & 0xFFFFFFE0;

    this->m_text = static_cast<char*>(
        SMemReAlloc(this->m_text, this->m_textSize, __FILE__, __LINE__, 0)
    );

    this->m_textInfo = static_cast<uint32_t*>(
        SMemReAlloc(this->m_textInfo, this->m_textSize * sizeof(uint32_t), __FILE__, __LINE__, 0)
    );
}

void CSimpleEditBox::Insert(uint32_t chr) {
    if (chr == '\t') {
        this->Insert("    ", nullptr, 1, 0, 0);
        return;
    }

    if (chr == '|') {
        this->Insert("||", nullptr, 1, 0, 0);
        return;
    }

    if ((this->m_multiline && chr == '\n') || (chr >= ' ' && chr != 127)) {
        char charBuf[8];
        SUniSPutUTF8(chr, charBuf);

        this->Insert(charBuf, nullptr, 1, 0, 0);
        return;
    }
}

void CSimpleEditBox::Insert(const char* a2, const char* a3, int32_t a4, int32_t a5, int32_t a6) {
    if (
        (this->m_textInfo[this->m_cursorPos] & 0x80000000)
        && this->m_cursorPos > 0
        && (this->m_textInfo[this->PrevCharOffset(this->m_cursorPos)] & 0x80000000)
    ) {
        return;
    }

    if (this->m_highlightLeft != this->m_highlightRight) {
        this->DeleteHighlight(a6);
    }

    if (!a2) {
        a2 = "";
    }

    const char* v10 = a2;

    if (this->m_numeric && !a5 && *v10) {
        while (*v10) {
            int32_t v33;

            if (SUniSGetUTF8(reinterpret_cast<const uint8_t*>(a2), &v33) - 48 > 9) {
                return;
            }

            v10 += v33;
        }
    }

    int32_t v11 = SStrLen(a2);

    this->GrowText(this->m_textLength + v11);

    char* v14 = this->m_text + this->m_cursorPos;

    if (this->m_cursorPos < this->m_textLength) {
        memcpy(v14 + v11, v14, this->m_textLength - this->m_cursorPos);
    }

    memcpy(v14, a2, v11);

    this->m_textLength += v11;
    *(this->m_text + this->m_textLength) = 0;

    int32_t v31 = this->m_cursorPos;
    this->m_cursorPos += v11;

    this->UpdateTextInfo();

    this->m_dirtyFlags |= (0x1 | 0x4);
    a6 ? this->m_dirtyFlags &= ~0x8 : this->m_dirtyFlags |= 0x8;

    if (this->m_textLengthMax >= 0 && this->m_textLength > this->m_textLengthMax) {
        // TODO
    }

    if (this->m_textLettersMax) {
        // TODO
    }

    if (a5) {
        this->m_highlightLeft = v31;
        this->m_highlightRight = std::min(this->m_textLength, this->m_cursorPos);
        this->m_dirtyFlags |= 2u;
    }

    // TODO
    // if (!this->m_intC && this->m_textLength > 0) {
    //     this->m_intC = a3;
    // }

    if (a4) {
        this->RunOnCharScript(a2);

        // TODO
        // - check for spaces and run onSpacePressed script
    }
}

bool CSimpleEditBox::IsA(int32_t type) {
    return type == CSimpleEditBox::s_objectType
        || type == CSimpleFrame::s_objectType
        || type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}

int32_t CSimpleEditBox::IsCurrentFocus() {
    return this == CSimpleEditBox::s_currentFocus;
}

void CSimpleEditBox::LoadXML(XMLNode* node, CStatus* status) {
    CSimpleFrame::LoadXML(node, status);

    const char* fontAttr = node->GetAttributeByName("font");
    if (fontAttr && *fontAttr) {
        auto font = CSimpleFont::GetFont(fontAttr, 0);

        if (font) {
            this->m_font->SetFontObject(font);
        } else {
            status->Add(
                STATUS_WARNING,
                "%s %s: Couldn't find font object named %s",
                this->GetObjectTypeName(),
                this->GetDisplayName(),
                fontAttr
            );
        }
    }

    // TODO
    // - letters

    const char* countInvisibleLettersAttr = node->GetAttributeByName("countInvisibleLetters");
    if (countInvisibleLettersAttr && *countInvisibleLettersAttr) {
        this->m_countInvisibleLetters = StringToBOOL(countInvisibleLettersAttr);
    }

    const char* blinkSpeedAttr = node->GetAttributeByName("blinkSpeed");
    if (blinkSpeedAttr && *blinkSpeedAttr) {
        float blinkSpeed = std::max(SStrToFloat(blinkSpeedAttr), 0.0f);
        this->m_cursorBlinkSpeed = blinkSpeed;
    }

    const char* autoFocusAttr = node->GetAttributeByName("autoFocus");
    if (autoFocusAttr && *autoFocusAttr) {
        this->m_autoFocus = StringToBOOL(autoFocusAttr);
    }

    const char* multiLineAttr = node->GetAttributeByName("multiLine");
    if (multiLineAttr && *multiLineAttr) {
        this->m_multiline = StringToBOOL(multiLineAttr);
    }

    const char* numericAttr = node->GetAttributeByName("numeric");
    if (numericAttr && *numericAttr) {
        this->m_numeric = StringToBOOL(numericAttr);
    }

    const char* passwordAttr = node->GetAttributeByName("password");
    if (passwordAttr && *passwordAttr) {
        this->m_password = StringToBOOL(passwordAttr);
    }

    const char* ignoreArrowsAttr = node->GetAttributeByName("ignoreArrows");
    if (ignoreArrowsAttr && *ignoreArrowsAttr) {
        this->m_ignoreArrows = StringToBOOL(ignoreArrowsAttr);
    }

    const char* historyLinesAttr = node->GetAttributeByName("historyLines");
    if (historyLinesAttr && *historyLinesAttr) {
        int32_t historyLines = std::max(SStrToInt(historyLinesAttr), 0);
        this->SetHistoryLines(historyLines);
    }

    for (XMLNode* child = node->m_child; child; child = child->m_next) {
        if (!SStrCmpI(child->GetName(), "FontString", STORM_MAX_STR)) {
            CSimpleFont* font = this->m_font;

            if (font->GetFontObject()) {
                status->Add(
                    STATUS_WARNING,
                    "%s: FontString element overriding font %s",
                    this->GetDisplayName(),
                    font->GetFontObject()->GetName()
                );
            }

            this->m_font->LoadXML(child, status);

            const char* bytesAttr = child->GetAttributeByName("bytes");
            if (bytesAttr && *bytesAttr) {
                int32_t bytes = SStrToInt(bytesAttr);

                if (bytes <= 0) {
                    this->m_textLengthMax = -1;
                } else {
                    this->m_textLengthMax = bytes - 1;
                }
            }

        } else if (!SStrCmpI(child->GetName(), "HighlightColor", STORM_MAX_STR)) {
            CImVector highlightColor = {};
            if (LoadXML_Color(child, highlightColor)) {
                for (auto& texture : this->m_highlight) {
                    texture->SetTexture(highlightColor);
                }
            }

        } else if (!SStrCmpI(child->GetName(), "TextInsets", STORM_MAX_STR)) {
            float left, right, top, bottom;

            if (LoadXML_Insets(child, left, right, top, bottom, status)) {
                this->m_editTextInset.minX = left;
                this->m_editTextInset.maxX = right;
                this->m_editTextInset.maxY = top;
                this->m_editTextInset.minY = bottom;

                this->UpdateSizes();
            }
        }
    }
}

void CSimpleEditBox::Move(int32_t distance, int32_t highlight) {
    STORM_ASSERT(distance);

    auto offset = this->GetNumToLen(this->m_cursorPos, distance, true);
    if (distance < 0) {
        offset = -offset;
    }

    if (highlight) {
        if (this->m_highlightLeft == this->m_highlightRight) {
            this->m_highlightLeft = this->m_cursorPos;
            this->m_highlightRight = this->m_cursorPos;
        }

        // TODO this->Sub9631A0(offset);

        this->m_cursorPos += offset;
        this->m_dirtyFlags |= 0x4;
    } else {
        if (this->m_highlightLeft != this->m_highlightRight) {
            this->m_dirtyFlags |= 0x2;
            this->m_highlightLeft = 0;
            this->m_highlightRight = 0;
        }

        this->m_cursorPos += offset;
        this->m_dirtyFlags |= 0x4;
    }
}

void CSimpleEditBox::MoveBackward(int32_t highlight) {
    if (this->m_cursorPos > 0) {
        this->Move(-1, highlight);
    }

    if (!highlight && this->m_highlightLeft != this->m_highlightRight) {
        this->m_dirtyFlags |= 0x2;
        this->m_highlightLeft = 0;
        this->m_highlightRight = 0;
    }
}

void CSimpleEditBox::MoveForward(int32_t highlight) {
    if (this->m_cursorPos < this->m_textLength) {
        this->Move(1, highlight);
    }

    if (!highlight && this->m_highlightLeft != this->m_highlightRight) {
        this->m_dirtyFlags |= 0x2;
        this->m_highlightLeft = 0;
        this->m_highlightRight = 0;
    }
}

void CSimpleEditBox::OnEnterPressed() {
    this->RunOnEnterPressedScript();

    // TODO
    // - invoke action
}

void CSimpleEditBox::OnEscapePressed() {
    this->RunOnEscapePressedScript();
    this->DispatchAction(EVENT_ESCAPE);
}

void CSimpleEditBox::OnTabPressed() {
    this->RunOnTabPressedScript();
    this->DispatchAction(EVENT_TAB);
}

void CSimpleEditBox::OnFrameSizeChanged(float width, float height) {
    CSimpleFrame::OnFrameSizeChanged(width, height);
    this->UpdateSizes();
}

int32_t CSimpleEditBox::OnLayerChar(const CCharEvent& evt) {
    if (!this->m_visible) {
        return 0;
    }

    if (!CSimpleEditBox::s_currentFocus && this->m_autoFocus) {
        CSimpleEditBox::SetKeyboardFocus(this);
    } else if (!this->IsCurrentFocus()) {
        return 0;
    }

    this->m_imeInputMode = 0;

    this->Insert(evt.ch);

    return 1;
}

int32_t CSimpleEditBox::OnLayerKeyDown(const CKeyEvent& evt) {
    if (!this->m_visible) {
        return 0;
    }

    if (evt.key == KEY_PRINTSCREEN
        || (
            this->m_ignoreArrows
            && (evt.key == KEY_LEFT || evt.key == KEY_RIGHT || evt.key == KEY_UP || evt.key == KEY_DOWN)
            && !EventIsKeyDown(KEY_LALT)
            && !EventIsKeyDown(KEY_RALT)
        )
    ) {
        return 0;
    }

    if (!CSimpleEditBox::s_currentFocus && this->m_autoFocus) {
        CSimpleEditBox::SetKeyboardFocus(this);
    } else if (!this->IsCurrentFocus()) {
        return 0;
    }

    this->UpdateDirtyBits();

    switch (evt.key) {
        case KEY_ESCAPE: {
            this->OnEscapePressed();

            break;
        }

        case KEY_ENTER: {
            if (!this->m_multiline || this->m_onEnterPressed.luaRef) {
                // TODO
                // this->m_top->m_int1250 = 1;

                this->OnEnterPressed();

                // TODO
                // this->m_top->m_int1250 = 0;
            } else {
                this->Insert("\n", 0, 1, 0, 0);
            }

            break;
        }

        case KEY_BACKSPACE: {
            if (EventIsControlKeyDown()) {
                // TODO this->DeleteBackwardWord(0);
            } else {
                this->DeleteBackward(0);
            }

            break;
        }

        case KEY_LEFT: {
            if (EventIsControlKeyDown()) {
                // TODO this->MoveBackwardWord(EventIsShiftKeyDown());
            } else {
                this->MoveBackward(EventIsShiftKeyDown());
            }

            break;
        }

        case KEY_RIGHT: {
            if (EventIsControlKeyDown()) {
                // TODO this->MoveForwardWord(EventIsShiftKeyDown());
            } else {
                this->MoveForward(EventIsShiftKeyDown());
            }

            break;
        }

        case KEY_DELETE: {
            if (EventIsShiftKeyDown()) {
                // TODO cut to clipboard
            } else if (EventIsControlKeyDown()) {
                // TODO this->DeleteForwardWord(0);
            } else {
                this->DeleteForward(0);
            }

            break;
        }

        case KEY_TAB: {
            this->OnTabPressed();

            break;
        }

        // TODO
        // - remaining keys

        default:
            break;
    }

    return 1;
}

int32_t CSimpleEditBox::OnLayerKeyDownRepeat(const CKeyEvent& evt) {
    if (!this->m_visible) {
        return 0;
    }

    if (!CSimpleEditBox::s_currentFocus && this->m_autoFocus) {
        CSimpleEditBox::SetKeyboardFocus(this);
        return this->OnLayerKeyDown(evt);
    }

    if (this->IsCurrentFocus()) {
        return this->OnLayerKeyDown(evt);
    }

    return 0;
}

int32_t CSimpleEditBox::OnLayerMouseDown(const CMouseEvent& evt, const char* btn) {
    int32_t eaten = CSimpleFrame::OnLayerMouseDown(evt, btn);

    if (!eaten) {
        this->UpdateDirtyBits();

        int32_t position;

        if (!this->m_imeInputMode && this->ConvertCoordinateToIndex(evt.x, evt.y, position)) {
            if (this->m_highlightLeft != this->m_highlightRight) {
                this->m_dirtyFlags |= 0x2;
                this->m_highlightRight = 0;
                this->m_highlightLeft = 0;
            }

            this->SetCursorPosition(position);

            this->StartHighlight();
            this->m_highlightDrag = 1;

            eaten = 1;
        }

        CSimpleEditBox::SetKeyboardFocus(this);
    }

    return eaten;
}

int32_t CSimpleEditBox::OnLayerMouseUp(const CMouseEvent& evt, const char* btn) {
    int32_t eaten = CSimpleFrame::OnLayerMouseUp(evt, btn);

    if (!eaten) {
        if (this->m_highlightDrag) {
            this->m_highlightDrag = 0;
            eaten = 1;
        }
    }

    return eaten;
}

void CSimpleEditBox::OnLayerUpdate(float elapsedSec) {
    CSimpleFrame::OnLayerUpdate(elapsedSec);

    this->UpdateDirtyBits();

    if (!GxCapsWindowHasFocus(-1)) {
        this->m_cursor->Hide();
        return;
    }

    if (this->m_cursorBlinkSpeed == 0.0f) {
        return;
    }

    bool cursorVisible = this->m_cursorPos >= this->m_visiblePos
        && this->m_cursorPos <= this->m_visiblePos + this->m_visibleLen;

    if (!cursorVisible) {
        return;
    }

    this->m_blinkElapsedTime += elapsedSec;

    if (this->IsCurrentFocus() && this->m_blinkElapsedTime > this->m_cursorBlinkSpeed) {
        if (this->m_cursor->IsShown()) {
            this->m_cursor->Hide();
        } else {
            this->m_cursor->Show();
        }

        this->m_blinkElapsedTime = 0.0f;
    }
}

void CSimpleEditBox::OnTextChanged() {
    this->RunOnTextChangedScript((this->m_dirtyFlags & 0x8) != 0);
    this->DispatchAction(EVENT_CHANGED);
}

int32_t CSimpleEditBox::PrevCharOffset(int32_t offset) {
    int32_t prevCharOffset = offset;

    while (prevCharOffset > 0 && !this->m_textInfo[prevCharOffset - 1]) {
        prevCharOffset--;
    }

    return prevCharOffset;
}

void CSimpleEditBox::RunOnCursorChangedScript(float x, float y, float w, float h) {
    // TODO
}

void CSimpleEditBox::RunOnEditFocusGainedScript() {
    if (this->m_onEditFocusGained.luaRef) {
        this->RunScript(this->m_onEditFocusGained, 0, 0);
    }
}

void CSimpleEditBox::RunOnEditFocusLostScript() {
    if (this->m_onEditFocusLost.luaRef) {
        this->RunScript(this->m_onEditFocusLost, 0, 0);
    }
}

void CSimpleEditBox::RunOnEnterPressedScript() {
    if (this->m_onEnterPressed.luaRef) {
        this->RunScript(this->m_onEnterPressed, 0, 0);
    }
}

void CSimpleEditBox::RunOnEscapePressedScript() {
    if (this->m_onEscapePressed.luaRef) {
        this->RunScript(this->m_onEscapePressed, 0, nullptr);
    }
}

void CSimpleEditBox::RunOnTabPressedScript() {
    if (this->m_onTabPressed.luaRef) {
        this->RunScript(this->m_onTabPressed, 0, nullptr);
    }
}

void CSimpleEditBox::RunOnTextChangedScript(int32_t changed) {
    if (this->m_onTextChanged.luaRef) {
        auto L = FrameScript_GetContext();
        lua_pushboolean(L, changed);

        this->RunScript(this->m_onTextChanged, 1, 0);
    }
}

void CSimpleEditBox::SetCursorPosition(int32_t position) {
    if (position >= 0) {
        this->m_cursorPos = std::min(this->m_textLength, position);
    } else {
        this->m_cursorPos = 0;
    }

    this->m_dirtyFlags |= 0x4;
}

void CSimpleEditBox::SetHistoryLines(int32_t a2) {
    // TODO
}

void CSimpleEditBox::SetMultiLine(int32_t enabled) {
    if (enabled) {
        this->m_multiline = 1;
    } else {
        this->m_multiline = 0;
    }

    this->m_visibleLines.SetCount(2);

    if (this->m_multiline) {
        this->m_string->SetJustifyV(0x8);

        // TODO
        // - unknown style change (remove flag 0x4000)

        this->m_string->SetNonWordWrap(0);
    } else {
        this->m_string->SetJustifyV(0x10);

        // TODO
        // - unknown style change (add flag 0x4000)

        this->m_string->SetNonWordWrap(1);
    }

    this->m_string->SetNonSpaceWrap(1);

    this->UpdateSizes();
}

void CSimpleEditBox::StartHighlight() {
    // TODO
}

void CSimpleEditBox::UpdateDirtyBits() {
    int32_t textChanged = this->m_dirtyFlags & 0x1;

    if (this->m_dirtyFlags & 0x4) {
        // TODO
    }

    if (this->m_dirtyFlags & 0x1) {
        this->m_dirtyFlags &= ~0x1;
        this->UpdateVisibleText();
    }

    if (this->m_dirtyFlags & 0x2) {
        this->m_dirtyFlags &= ~0x2;
        this->UpdateVisibleHighlight();
    }

    if (this->m_dirtyFlags & 0x4) {
        this->m_dirtyFlags &= ~0x4;
        this->UpdateVisibleCursor();
    }

    if (textChanged) {
        this->OnTextChanged();
        this->m_dirtyFlags &= ~0x8;
    }
}

void CSimpleEditBox::UpdateSizes() {
    if (!(CLayoutFrame::m_flags & 0x1)) {
        return;
    }

    this->m_string->ClearAllPoints();

    float stringWidth = ((this->m_rect.maxX - this->m_rect.minX) / this->m_layoutScale)
        - (this->m_editTextInset.maxX + this->m_editTextInset.minX);

    this->m_string->SetWidth(stringWidth);

    float stringHeight = 0.0f;

    if (!this->m_multiline) {
        stringHeight = ((this->m_rect.maxY - this->m_rect.minY) / this->m_layoutScale)
            - (this->m_editTextInset.maxY + this->m_editTextInset.minY);
    }

    this->m_string->SetHeight(stringHeight);

    this->m_string->SetPoint(
        FRAMEPOINT_TOPLEFT,
        this,
        FRAMEPOINT_TOPLEFT,
        this->m_editTextInset.minX,
        -this->m_editTextInset.maxY,
        1
    );

    float fontHeight = this->m_string->GetFontHeight(1);

    for (int32_t i = 0; i < 3; i++) {
        // TODO
        // - highlight sizing
    }

    // TODO
    // - clause highlight? sizing

    float v13 = 4.0f / (CoordinateGetAspectCompensation() * 1024.0f);
    float cursorWidth = NDCToDDCWidth(v13);

    this->m_cursor->ClearAllPoints();
    this->m_cursor->SetWidth(cursorWidth);
    this->m_cursor->SetHeight(fontHeight);

    this->m_dirtyFlags |= (0x1 | 0x2 | 0x4);
}

void CSimpleEditBox::UpdateTextInfo() {
    memset(this->m_textInfo, 0, this->m_textSize * sizeof(uint32_t));

    uint32_t i = 0;
    uint32_t v9 = 0;
    int32_t advance;
    uint32_t code;

    const char* text = this->m_text;

    while (text && *text) {
        QUOTEDCODE quotedCode = GxuDetermineQuotedCode(text, advance, nullptr, 0, code);

        if (quotedCode == CODE_HYPERLINKSTART) {
            v9 |= 0x80000000;
        }

        this->m_textInfo[i] = v9 | (quotedCode << 16) | static_cast<uint16_t>(advance);

        if (quotedCode == CODE_HYPERLINKSTOP) {
            v9 &= 0x7FFFFFFF;
        }

        i += advance;
        text += advance;
    }
}

void CSimpleEditBox::UpdateVisibleCursor() {
    bool cursorVisible = this->m_cursorPos >= this->m_visiblePos
        && this->m_cursorPos <= this->m_visiblePos + this->m_visibleLen;

    if (!cursorVisible) {
        this->m_cursor->Hide();
        return;
    }

    const char* text = this->m_password
        ? this->m_textHidden
        : this->m_text;

    float cursorX = 0.0f;
    float cursorY = 0.0f;

    float v8 = CSimpleTop::RoundToPixelHeight(this->m_string->GetFontHeight(1) * this->m_layoutScale);
    float v10 = CSimpleTop::RoundToPixelHeight(this->m_string->m_spacing * this->m_layoutScale);
    float v13 = (v10 + v8) / this->m_layoutScale;

    int32_t cursorPos = this->m_cursorPos;
    int32_t line = 0;

    while (cursorPos >= this->m_visibleLines[line + 1] && line < this->m_visibleLines.Count() - 2) {
        line++;
        cursorY -= v13;
    }

    this->m_cursor->ClearAllPoints();

    // Left aligned
    if (this->m_string->m_styleFlags & 0x1) {
        float v36;

        if (cursorPos == this->m_visibleLines[line]) {
            v36 = 0.0f;
        } else {
            v36 = this->m_string->GetTextWidth(
                text + this->m_visibleLines[line],
                cursorPos - this->m_visibleLines[line]
            );
        }

        cursorX = v36;

        if (this->m_multiline) {
            this->m_cursor->SetPoint(
                FRAMEPOINT_TOPLEFT,
                this->m_string,
                FRAMEPOINT_TOPLEFT,
                cursorX,
                cursorY,
                1
            );
        } else {
            this->m_cursor->SetPoint(
                FRAMEPOINT_LEFT,
                this->m_string,
                FRAMEPOINT_LEFT,
                cursorX,
                cursorY,
                1
            );
        }

    // Center aligned
    } else if (this->m_string->m_styleFlags & 0x2) {
        float v28;

        if (cursorPos == this->m_visibleLines[line]) {
            v28 = 0.0f;
        } else {
            v28 = this->m_string->GetTextWidth(
                text + this->m_visibleLines[line],
                cursorPos - this->m_visibleLines[line]
            );
        }

        float v29 = this->m_string->GetTextWidth(
              text + this->m_visibleLines[line],
              this->m_visibleLines[line + 1] - this->m_visibleLines[line]
        );

        cursorX = (v28 - v29) / 2.0f;

        if (this->m_multiline) {
            this->m_cursor->SetPoint(
                FRAMEPOINT_TOPLEFT,
                this->m_string,
                FRAMEPOINT_TOP,
                cursorX,
                cursorY,
                1
            );
        } else {
            this->m_cursor->SetPoint(
                FRAMEPOINT_LEFT,
                this->m_string,
                FRAMEPOINT_CENTER,
                cursorX,
                cursorY,
                1
            );
        }

    // Right aligned
    } else if (this->m_string->m_styleFlags & 0x4) {
        // TODO
    }

    this->m_blinkElapsedTime = 0.0f;

    float cursorWidth = this->m_cursor->GetWidth();
    float cursorHeight = this->m_cursor->GetHeight();
    this->RunOnCursorChangedScript(cursorX, cursorY, cursorWidth, cursorHeight);

    if (this->IsCurrentFocus()) {
        this->m_cursor->Show();
    } else {
        this->m_cursor->Hide();
    }
}

void CSimpleEditBox::UpdateVisibleHighlight() {
    // TODO
}

void CSimpleEditBox::UpdateVisibleText() {
    float stringWidth = this->m_string->GetWidth();

    // Update password mask
    if (this->m_password) {
        int32_t textLen = SStrLen(this->m_text);
        this->m_textHidden = static_cast<char*>(
            SMemReAlloc(this->m_textHidden, textLen + 1, __FILE__, __LINE__, 0)
        );
        memset(this->m_textHidden, '*', textLen);
        this->m_textHidden[textLen] = '\0';
    }

    char* text = this->m_password
        ? this->m_textHidden
        : this->m_text;

    if (this->m_multiline) {
        // TODO
    } else {
        float v16 = 4.0f / (CoordinateGetAspectCompensation() * 1024.0f);
        float v17 = stringWidth - NDCToDDCWidth(v16);
        if (v17 <= 0.0) {
            v17 = stringWidth;
        }

        this->m_visibleLen = this->m_string->GetNumCharsWithinWidth(
            text + this->m_visiblePos,
            0,
            v17
        );

        if (this->m_text == text) {
            this->m_visibleLen = this->GetNumToLen(
                this->m_visiblePos,
                this->m_visibleLen,
                0
            );
        }

        this->m_visibleLines[0] = this->m_visiblePos;
        this->m_visibleLines[1] = this->m_visiblePos + this->m_visibleLen;
    }

    if (!this->m_password) {
        // TODO
    }

    int32_t visibleEnd = this->m_visiblePos + this->m_visibleLen;
    auto v42 = text[visibleEnd];
    text[visibleEnd] = '\0';
    this->m_string->SetText(text + this->m_visiblePos, 0);
    text[visibleEnd] = v42;

    if (this->m_multiline) {
        auto height = this->m_string->GetStringHeight();
        if (height == 0.0f) {
            height = this->m_string->GetFontHeight(1);
        }

        this->SetHeight(height + this->m_editTextInset.minY + this->m_editTextInset.maxY);
    }
}
