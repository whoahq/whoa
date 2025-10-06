#include "ui/CSimpleFontString.hpp"
#include "gx/Coordinate.hpp"
#include "math/Utils.hpp"
#include "ui/CFramePoint.hpp"
#include "ui/CRenderBatch.hpp"
#include "ui/CSimpleFont.hpp"
#include "ui/CSimpleFontStringScript.hpp"
#include "ui/CSimpleFrame.hpp"
#include "ui/CSimpleTop.hpp"
#include "ui/FrameXML.hpp"
#include "ui/LoadXML.hpp"
#include "ui/Types.hpp"
#include "ui/Util.hpp"
#include "util/CStatus.hpp"
#include "util/StringTo.hpp"
#include <common/XML.hpp>
#include <storm/String.hpp>

int32_t CSimpleFontString::s_count;
int32_t CSimpleFontString::s_metatable;
int32_t CSimpleFontString::s_objectType;

void CSimpleFontString::CreateScriptMetaTable() {
    lua_State* L = FrameScript_GetContext();
    int32_t ref = FrameScript_Object::CreateScriptMetaTable(L, &CSimpleFontString::RegisterScriptMethods);
    CSimpleFontString::s_metatable = ref;
}

int32_t CSimpleFontString::GetObjectType() {
    if (!CSimpleFontString::s_objectType) {
        CSimpleFontString::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CSimpleFontString::s_objectType;
}

void CSimpleFontString::RegisterScriptMethods(lua_State* L) {
    CScriptRegion::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, SimpleFontStringMethods, NUM_SIMPLE_FONT_STRING_SCRIPT_METHODS);
}

CSimpleFontString::CSimpleFontString(CSimpleFrame* frame, uint32_t drawlayer, int32_t show) : CSimpleRegion(frame, drawlayer, show) {
    this->m_maxLines = 0;
    this->m_unk12b = 1;

    // TODO

    CSimpleFontString::s_count++;
}

CSimpleFontString::~CSimpleFontString() {
    this->FreeEmbeddedTextures();

    if (this->m_text) {
        SMemFree(this->m_text, __FILE__, __LINE__, 0x0);
        this->m_text = nullptr;
    }

    this->m_fontHeight = 0.0f;

    if (this->m_string) {
        HandleClose(this->m_string);
        this->m_string = nullptr;
    }

    if (this->m_font) {
        HandleClose(this->m_font);
        this->m_font = nullptr;
    }

    CSimpleFontString::s_count--;
}

void CSimpleFontString::AddShadow(const CImVector& shadowColor, const C2Vector& shadowOffset) {
    auto styleFlags = this->m_styleFlags;

    if (
        !(styleFlags & 0x100)
        || shadowColor != this->m_shadowColor
        || this->m_shadowOffset.x != shadowOffset.x
        || this->m_shadowOffset.y != shadowOffset.y
    ) {
        this->m_styleFlags = styleFlags | 0x100;
        this->m_shadowColor = shadowColor;
        this->m_shadowOffset = shadowOffset;

        if (this->m_string) {
            auto shadowColor = this->m_shadowColor;
            // TODO alpha manipulation

            auto shadowOffset = this->m_shadowOffset;
            shadowOffset.x *= this->m_layoutScale;
            shadowOffset.y *= this->m_layoutScale;

            TextBlockAddShadow(this->m_string, shadowColor, shadowOffset);
        }
    }
}

void CSimpleFontString::ClearString() {
    this->m_cachedWidth = 0.0f;
    this->m_cachedHeight = 0.0f;

    if (this->m_string) {
        HandleClose(this->m_string);
        this->m_string = nullptr;
    }

    this->m_flags &= ~0x1;
    this->Resize(0);
}

void CSimpleFontString::Draw(CRenderBatch* batch) {
    if (this->m_font && this->m_text && *this->m_text) {
        batch->QueueFontString(this);
        this->DrawEmbeddedTextures(batch);
    }
}

void CSimpleFontString::DrawEmbeddedTextures(CRenderBatch* batch) {
    // TODO
}

bool CSimpleFontString::IsA(int32_t type) {
    return type == CSimpleFontString::s_objectType
        || type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}

void CSimpleFontString::FontObjectUpdated(CSimpleFontStringAttributes& attributes) {
    attributes.Update(this, this->m_fontableFlags);
}

void CSimpleFontString::FreeEmbeddedTextures() {
    // TODO
}

const char* CSimpleFontString::GetDisplayText(float width, float height) {
    static char buffer[8192];

    if (!this->m_text || !*this->m_text) {
        return nullptr;
    }

    if (!this->m_font || width <= 0.0f || (this->m_maxLines == 0 && height <= 0.0)) {
        return this->m_text;
    }

    auto fontHeight = this->GetFontHeight(true);
    auto lineHeight = (CSimpleTop::RoundToPixelHeight(this->m_spacing) + fontHeight) * this->m_layoutScale;

    if (this->m_maxLines != 0) {
        height = lineHeight * this->m_maxLines;
    } else if (lineHeight >= height) {
        height = lineHeight;
    }

    auto text = this->m_text;
    auto textLen = SStrLen(text);
    auto scaledSpacing = this->m_spacing * this->m_layoutScale;
    auto scaledShadowOffset = this->m_shadowOffset.x * this->m_layoutScale;
    auto scaledFontHeight = fontHeight * this->m_layoutScale;

    auto maxChars = TextBlockGetMaxCharsWithinWidthAndHeight(
        this->m_font,
        text,
        scaledFontHeight,
        width,
        height,
        textLen,
        scaledShadowOffset,
        this->m_layoutScale,
        scaledSpacing,
        this->m_styleFlags
    );

    if (maxChars >= textLen) {
        return text;
    }

    SStrCopy(buffer, text, sizeof(buffer));
    auto end = std::min(textLen, sizeof(buffer) - 4);

    do {
        if (end) {
            do {
                end--;
            } while (end && (buffer[end] & 0xC0) == 0x80);
        }

        SStrCopy(&buffer[end], "...", STORM_MAX_STR);

        maxChars = TextBlockGetMaxCharsWithinWidthAndHeight(
            this->m_font,
            buffer,
            scaledFontHeight,
            width,
            height,
            end + 3,
            scaledShadowOffset,
            this->m_layoutScale,
            scaledSpacing,
            this->m_styleFlags
        );
    } while (end && maxChars < end + 3);

    return buffer;
}

uint32_t CSimpleFontString::GetFontFlags() {
    if (this->m_font) {
        return TextBlockGetFontFlags(this->m_font);
    }

    return 0;
}

float CSimpleFontString::GetFontHeight(bool a2) {
    float fontHeight = this->m_fontHeight;

    if (a2 && this->m_font && this->m_styleFlags & 0x200) {
        auto font = TextBlockGetFontPtr(this->m_font);
        float ndcHeight = GxuFontGetOneToOneHeight(font) / this->m_layoutScale;
        NDCToDDC(0.0f, ndcHeight, nullptr, &fontHeight);
    }

    return fontHeight;
}

const char* CSimpleFontString::GetFontName() {
    return this->m_font
        ? TextBlockGetFontName(this->m_font)
        : nullptr;
}

float CSimpleFontString::GetHeight() {
    float v2 = CLayoutFrame::GetHeight();
    float v9 = v2 == 0.0f ? this->GetStringHeight() : v2;

    float v3 = 1.0f / (CoordinateGetAspectCompensation() * 1024.0f);
    float v4 = NDCToDDCWidth(v3);

    return v9 <= v4 ? v4 : v9;
}

uint32_t CSimpleFontString::GetNumCharsWithinWidth(const char* text, uint32_t textBytes, float maxWidth) {
    if (!this->m_font) {
        return 0;
    }

    if (textBytes == 0) {
        textBytes = SStrLen(text);
    }

    float fontHeight = this->GetFontHeight(true);
    float shadowWidth = this->m_shadowOffset.x;
    float extent[4];

    return TextBlockGetMaxCharsWithinWidth(
        this->m_font,
        text,
        fontHeight * this->m_layoutScale,
        maxWidth * this->m_layoutScale,
        textBytes,
        extent,
        shadowWidth * this->m_layoutScale,
        this->m_layoutScale,
        0.0f,
        this->m_styleFlags
    );
}

int32_t CSimpleFontString::GetScriptMetaTable() {
    return CSimpleFontString::s_metatable;
}

float CSimpleFontString::GetStringHeight() {
    if (this->m_cachedHeight == 0.0f && this->m_font) {
        float width = CLayoutFrame::GetWidth() * this->m_layoutScale;
        float height = CLayoutFrame::GetHeight() * this->m_layoutScale;

        const char* displayText = this->GetDisplayText(width, height);
        if (displayText && *displayText) {
            float fontHeight = this->GetFontHeight(1) * this->m_layoutScale;
            C2Vector shadowSize = {
                this->m_shadowOffset.x * this->m_layoutScale,
                this->m_shadowOffset.y * this->m_layoutScale
            };

            this->m_cachedHeight = TextBlockGetWrappedTextHeight(
                this->m_font,
                displayText,
                fontHeight,
                this->GetWidth() * this->m_layoutScale,
                shadowSize,
                this->m_layoutScale,
                this->m_spacing * this->m_layoutScale,
                this->m_styleFlags
            );
        }
    }

    return this->m_cachedHeight / this->m_layoutScale;
}

float CSimpleFontString::GetStringWidth() {
    if (this->m_cachedWidth == 0.0f && this->m_font) {
        float width = CLayoutFrame::GetWidth() * this->m_layoutScale;
        float height = CLayoutFrame::GetHeight() * this->m_layoutScale;

        const char* displayText = this->GetDisplayText(width, height);
        if (displayText && *displayText) {
            float fontHeight = this->GetFontHeight(1) * this->m_layoutScale;
            float shadowWidth = this->m_shadowOffset.x * this->m_layoutScale;
            size_t displayTextLength = SStrLen(displayText);

            TextBlockGetTextExtent(
                this->m_font,
                displayText,
                displayTextLength,
                fontHeight,
                &this->m_cachedWidth,
                shadowWidth,
                this->m_layoutScale,
                0.0f,
                this->m_styleFlags
            );
        }
    }

    return this->m_cachedWidth / this->m_layoutScale;
}

const char* CSimpleFontString::GetText() {
    return this->m_text;
}

float CSimpleFontString::GetTextWidth(const char* text, uint32_t textBytes) {
    if (!this->m_font) {
        return 0.0f;
    }

    if (textBytes == 0) {
        textBytes = SStrLen(text);
    }

    float fontHeight = this->GetFontHeight(true);
    float extent = 0.0f;

    TextBlockGetTextExtent(
        this->m_font,
        text,
        textBytes,
        fontHeight * this->m_layoutScale,
        &extent,
        this->m_shadowOffset.x * this->m_layoutScale,
        this->m_layoutScale,
        0.0f,
        this->m_styleFlags
    );

    float width = extent / this->m_layoutScale;

    return width;
}

float CSimpleFontString::GetWidth() {
    float v2 = CLayoutFrame::GetWidth();
    float v9 = v2 == 0.0f ? this->GetStringWidth() : v2;

    float v3 = 1.0f / (CoordinateGetAspectCompensation() * 1024.0f);
    float v4 = NDCToDDCWidth(v3);

    return v9 <= v4 ? v4 : v9;
}

void CSimpleFontString::LoadXML(XMLNode* node, CStatus* status) {
    const char* inheritsAttr = node->GetAttributeByName("inherits");
    if (inheritsAttr && *inheritsAttr) {
        auto font = CSimpleFont::GetFont(inheritsAttr, 0);

        if (font) {
            if (this->m_unk12b) {
                this->SetFontObject(font);
            }
        } else {
            const char* tainted;
            bool locked;

            XMLNode* inheritsNode = FrameXML_AcquireHashNode(inheritsAttr, tainted, locked);

            if (inheritsNode) {
                if (locked) {
                    status->Add(STATUS_WARNING, "Recursively inherited node: %s", inheritsAttr);
                } else {
                    this->LoadXML(inheritsNode, status);
                    FrameXML_ReleaseHashNode(inheritsAttr);
                }
            } else {
                status->Add(STATUS_WARNING, "Couldn't find inherited node: %s", inheritsAttr);
            }
        }
    }

    CSimpleRegion::LoadXML(node, status);

    const char* hiddenAttr = node->GetAttributeByName("hidden");
    if (hiddenAttr && *hiddenAttr) {
        bool hide = StringToBOOL(hiddenAttr);

        if (hide) {
            this->Hide();
        } else {
            this->Show();
        }
    }

    const char* textAttr = node->GetAttributeByName("text");
    if (textAttr && *textAttr) {
        const char* text = FrameScript_GetText(textAttr, -1, GENDER_NOT_APPLICABLE);
        this->SetText(text && *text ? text : textAttr, 1);
    }

    const char* nonSpaceWrapAttr = node->GetAttributeByName("nonspacewrap");
    if (nonSpaceWrapAttr && *nonSpaceWrapAttr) {
        this->SetNonSpaceWrap(StringToBOOL(nonSpaceWrapAttr));
    }

    const char* wordWrapAttr = node->GetAttributeByName("wordwrap");
    if (wordWrapAttr && *wordWrapAttr) {
        this->SetNonWordWrap(!StringToBOOL(wordWrapAttr));
    }

    const char* bytesAttr = node->GetAttributeByName("bytes");
    if (bytesAttr && *bytesAttr) {
        uint32_t length = SStrToInt(bytesAttr);
        this->SetTextLength(length);
    }

    if (!this->m_unk12b) {
        return;
    }

    const char* fontAttr = node->GetAttributeByName("font");
    if (fontAttr && *fontAttr) {
        auto font = CSimpleFont::GetFont(fontAttr, 0);

        if (font) {
            this->SetFontObject(font);
        } else {
            float fontHeight = 0.0f;

            XMLNode* fontHeightNode = node->GetChildByName("FontHeight");
            if (fontHeightNode) {
                LoadXML_Value(fontHeightNode, fontHeight, status);
            }

            if (fontHeight == 0.0f) {
                status->Add(
                    STATUS_WARNING,
                    "FontString %s: invalid font height %f in %s element",
                    this->GetDisplayName(),
                    fontHeight,
                    node->GetName()
                );

                return;
            }

            uint32_t fontFlags = 0;

            const char* outlineAttr = node->GetAttributeByName("outline");
            if (outlineAttr && *outlineAttr) {
                if (!SStrCmpI(outlineAttr, "NORMAL", STORM_MAX_STR)) {
                    fontFlags |= FONT_OUTLINE;
                } else if (!SStrCmpI(outlineAttr, "THICK", STORM_MAX_STR)) {
                    fontFlags |= (FONT_OUTLINE | FONT_THICKOUTLINE);
                }
            }

            const char* monochromeAttr = node->GetAttributeByName("monochrome");
            if (monochromeAttr && *monochromeAttr && StringToBOOL(monochromeAttr)) {
                fontFlags |= FONT_MONOCHROME;
            }

            if (this->SetFont(fontAttr, fontHeight, fontFlags, false)) {
                this->m_fontableFlags &= ~FLAG_FONT_UPDATE;
            } else {
                status->Add(
                    STATUS_WARNING,
                    "FontString %s: Unable to load font file %s",
                    this->GetDisplayName(),
                    fontAttr
                );
            }
        }
    }

    const char* spacingAttr = node->GetAttributeByName("spacing");
    if (spacingAttr && *spacingAttr) {
        float spacing = SStrToFloat(spacingAttr);
        float ndcSpacing = spacing / (CoordinateGetAspectCompensation() * 1024.0f);
        float ddcSpacing = NDCToDDCWidth(ndcSpacing);

        this->SetSpacing(ddcSpacing);
        this->m_fontableFlags &= ~FLAG_SPACING_UPDATE;
    }

    const char* justifyVAttr = node->GetAttributeByName("justifyV");
    if (justifyVAttr && *justifyVAttr) {
        uint32_t justify;

        if (StringToJustify(justifyVAttr, justify)) {
            this->SetJustifyV(justify);
            this->m_fontableFlags &= ~FLAG_STYLE_UPDATE;
        }
    }

    const char* justifyHAttr = node->GetAttributeByName("justifyH");
    if (justifyHAttr && *justifyHAttr) {
        uint32_t justify;

        if (StringToJustify(justifyHAttr, justify)) {
            this->SetJustifyH(justify);
            this->m_fontableFlags &= ~FLAG_STYLE_UPDATE;
        }
    }

    const char* indentedAttr = node->GetAttributeByName("indented");
    if (indentedAttr && *indentedAttr) {
        bool indented = StringToBOOL(indentedAttr);
        this->SetIndentedWordWrap(indented);
    }

    for (XMLNode* child = node->m_child; child; child = child->m_next) {
        if (!SStrCmpI(child->GetName(), "Color", STORM_MAX_STR)) {
            CImVector color;
            LoadXML_Color(child, color);
            this->SetVertexColor(color);
            this->m_fontableFlags &= ~FLAG_COLOR_UPDATE;

        } else if (!SStrCmpI(child->GetName(), "Shadow", STORM_MAX_STR)) {
            // TODO
        }
    }

    // TODO
    // - alpha

    // TODO
    // - children
}

void CSimpleFontString::OnColorChanged(bool a2) {
    CSimpleRegion::OnColorChanged(a2);

    if (this->m_string) {
        CImVector color = { 0xFF, 0xFF, 0xFF, 0xFF };
        if (this->m_colorCount == 1) {
            color = this->m_color[0];
        }

        TextBlockUpdateColor(this->m_string, color);

        if (this->m_styleFlags & 0x100) {
            CImVector shadowColor = this->m_shadowColor;

            C2Vector shadowOffset = {
                this->m_shadowOffset.x * this->m_layoutScale,
                this->m_shadowOffset.y * this->m_layoutScale
            };

            // TODO
            // - shadow color alpha calculation

            TextBlockAddShadow(this->m_string, shadowColor, shadowOffset);
        }
    }

    // TODO
    // - embedded texture color updates
}

void CSimpleFontString::OnScreenSizeChanged() {
    this->m_cachedWidth = 0.0f;
    this->m_cachedHeight = 0.0f;
    this->Resize(0);
}

void CSimpleFontString::OnFrameSizeChanged(const CRect& rect) {
    CLayoutFrame::OnFrameSizeChanged(rect);

    if (
        this->m_string
        && AreEqual(rect.maxX - rect.minX, this->m_rect.maxX - this->m_rect.minX, WHOA_EPSILON_2)
        && AreEqual(rect.maxY - rect.minY, this->m_rect.maxY - this->m_rect.minY, WHOA_EPSILON_2)
    ) {
        C3Vector pos = {
            this->m_justificationOffset.x * this->m_layoutScale + this->m_rect.minX,
            this->m_justificationOffset.y * this->m_layoutScale + this->m_rect.minY,
            this->m_layoutDepth
        };

        TextBlockSetStringPos(this->m_string, pos);

        if (rect.Sub4826D0() != this->m_rect.Sub4826D0()) {
            this->OnRegionChanged();
        }
    } else {
        this->UpdateString();
    }

    // TODO logic if region flags & 0x40

    if (this->m_parent) {
        this->m_parent->NotifyScrollParent();
    }
}

void CSimpleFontString::PostLoadXML(XMLNode* node, CStatus* status) {
    if (this->m_parent) {
        int32_t hasPoint = 0;

        for (int32_t i = 0; i < FRAMEPOINT_NUMPOINTS; i++) {
            auto point = this->m_points[i];

            if (point && !(point->m_flags & 0x08)) {
                hasPoint = 1;
                break;
            }
        }

        if (!hasPoint) {
            float offsetX = 0.0f;
            float offsetY = 0.0f;

            if (this->m_styleFlags & 0x01) {
                this->SetPoint(FRAMEPOINT_LEFT, this->m_parent, FRAMEPOINT_LEFT, offsetX, offsetY, 1);
            } else if (this->m_styleFlags & 0x04) {
                this->SetPoint(FRAMEPOINT_RIGHT, this->m_parent, FRAMEPOINT_RIGHT, offsetX, offsetY, 1);
            } else {
                this->SetPoint(FRAMEPOINT_CENTER, this->m_parent, FRAMEPOINT_CENTER, offsetX, offsetY, 1);
            }
        }
    }
}

void CSimpleFontString::RefreshEmbeddedTextures() {
    // TODO
}

void CSimpleFontString::RemoveShadow() {
    // TODO
}

int32_t CSimpleFontString::SetFont(const char* fontName, float fontHeight, uint32_t fontFlags, bool force) {
    if (!force) {
        const char* curFontName = this->GetFontName();

        if (
            fontName
            && curFontName
            && !SStrCmpI(fontName, curFontName, STORM_MAX_STR)
            && AreEqual(fontHeight, this->m_fontHeight, WHOA_EPSILON_1)
            && fontFlags == this->GetFontFlags()
        ) {
            return 1;
        }
    }

    HTEXTFONT font = nullptr;

    if (fontName && fontHeight != 0.0f) {
        font = TextBlockGenerateFont(fontName, fontFlags, fontHeight * this->m_layoutScale);

        if (!font) {
            return 0;
        }
    }

    this->m_fontHeight = fontHeight;

    if (this->m_string) {
        HandleClose(this->m_string);
        this->m_string = nullptr;
    }

    if (this->m_font) {
        HandleClose(this->m_font);
    }

    this->m_font = font;

    if (font) {
        this->m_cachedWidth = 0.0f;
        this->m_cachedHeight = 0.0f;
    }

    this->m_flags &= ~0x1;
    this->Resize(0);

    return 1;
}

void CSimpleFontString::SetIndentedWordWrap(bool a2) {
    // TODO
}

void CSimpleFontString::SetJustificationOffset(float x, float y) {
    if (this->m_justificationOffset.x == x && this->m_justificationOffset.y == y) {
        return;
    }

    this->m_justificationOffset = { x, y };

    if (this->m_string) {
        C3Vector pos = {
            this->m_rect.minX + (this->m_justificationOffset.x * this->m_layoutScale),
            this->m_rect.minY + (this->m_justificationOffset.y * this->m_layoutScale),
            this->m_layoutDepth
        };

        TextBlockSetStringPos(this->m_string, pos);
    }
}

void CSimpleFontString::SetJustifyV(uint8_t justify) {
    this->m_settableStyleFlags &= ~(0x8 | 0x10 | 0x20);
    uint32_t newStyleFlags = this->m_styleFlags ^ ((justify ^ this->m_styleFlags) & (0x8 | 0x10 | 0x20));

    if (this->m_styleFlags != newStyleFlags) {
        this->m_styleFlags = newStyleFlags;

        if (this->m_string) {
            this->UpdateString();
        }
    }
}

void CSimpleFontString::SetJustifyH(uint8_t justify) {
    this->m_settableStyleFlags &= ~(0x1 | 0x2 | 0x4);
    uint32_t newStyleFlags = this->m_styleFlags ^ ((justify ^ this->m_styleFlags) & (0x1 | 0x2 | 0x4));

    if (this->m_styleFlags != newStyleFlags) {
        this->m_styleFlags = newStyleFlags;

        if (this->m_string) {
            this->UpdateString();
        }
    }
}

void CSimpleFontString::SetNonSpaceWrap(int32_t a2) {
    // TODO
}

void CSimpleFontString::SetSpacing(float spacing) {
    if (spacing < 0.0f) {
        spacing = 0.0f;
    }

    if (fabs(spacing - this->m_spacing) >= WHOA_EPSILON_1) {
        this->m_spacing = spacing;
        this->m_cachedHeight = 0.0f;

        if (this->m_string) {
            this->UpdateString();
        }
    }
}

void CSimpleFontString::SetStyleFlags(uint32_t styleFlags) {
    uint32_t newStyleFlags = (styleFlags & this->m_settableStyleFlags) | (this->m_styleFlags & ~this->m_settableStyleFlags);

    if (this->m_styleFlags != newStyleFlags) {
        this->m_styleFlags = newStyleFlags;

        if (this->m_string) {
            this->UpdateString();
        }
    }
}

void CSimpleFontString::SetText(const char* text, int32_t a3) {
    if (text && a3) {
        text = LanguageProcess(text);
    }

    if (text && *text) {
        if (this->m_text && !SStrCmp(text, this->m_text, STORM_MAX_STR)) {
            return;
        }

        if (this->m_textMaxSize & 0xFFFF) {
            SStrCopy(this->m_text, text, this->m_textMaxSize & 0xFFFF);
        } else {
            size_t textSize = SStrLen(text);

            if (textSize <= this->m_textCurSize) {
                SStrCopy(this->m_text, text, STORM_MAX_STR);
            } else {
                if (this->m_text) {
                    SMemFree(this->m_text, __FILE__, __LINE__, 0x0);
                }

                this->m_text = SStrDupA(text, __FILE__, __LINE__);
                this->m_textCurSize = textSize;
            }
        }
    } else {
        if (this->m_text) {
            *this->m_text = '\0';
        }
    }

    this->ClearString();
}

int32_t CSimpleFontString::Sub482AC0() {
    // TODO
    return 0;
}

void CSimpleFontString::SetTextLength(uint32_t a2) {
    // TODO
}

void CSimpleFontString::SetNonWordWrap(int32_t a2) {
    // TODO
}

void CSimpleFontString::UpdateString() {
    if (!(this->m_flags & 0x1)) {
        return;
    }

    if (this->m_string) {
        HandleClose(this->m_string);
        this->m_string = nullptr;
    }

    this->FreeEmbeddedTextures();

    if (this->m_rect.maxY <= this->m_rect.minY || this->m_rect.maxX <= this->m_rect.minX) {
        return;
    }

    if (this->m_font && this->m_text && *this->m_text) {
        C3Vector pos = {
            this->m_rect.minX + this->m_justificationOffset.x * this->m_layoutScale,
            this->m_rect.minY + this->m_justificationOffset.y * this->m_layoutScale,
            this->m_layoutDepth
        };

        float width = this->GetWidth();
        float blockWidth = width == 0.0f ? this->m_cachedWidth : width * this->m_layoutScale;
        if (NotEqual(blockWidth, this->m_rect.maxX - this->m_rect.minX, WHOA_EPSILON_2)) {
            blockWidth = this->m_rect.maxX - this->m_rect.minX;
        }

        float height = this->GetHeight();
        float blockHeight = height == 0.0f ? this->m_cachedHeight : height * this->m_layoutScale;
        if (NotEqual(blockHeight, this->m_rect.maxY - this->m_rect.minY, WHOA_EPSILON_2)) {
            blockHeight = this->m_rect.maxY - this->m_rect.minY;
        }

        const char* displayText = this->GetDisplayText(blockWidth, blockHeight);

        uint32_t styleFlags = this->m_styleFlags;

        if (!(this->m_styleFlags & 0x400)) {
            // TODO

            styleFlags |= 0x400;
        }

        CImVector color = { 0xFF, 0xFF, 0xFF, 0xFF };

        if (this->m_colorCount) {
            color = this->m_color[0];
        }

        float scale = this->m_layoutScale;
        float lineSpacing = this->m_spacing * this->m_layoutScale;
        float fontHeight = this->GetFontHeight(true) * this->m_layoutScale;

        this->m_string = TextBlockCreate(
            this->m_font,
            displayText,
            color,
            pos,
            fontHeight,
            blockWidth,
            blockHeight,
            styleFlags,
            0.0f,
            lineSpacing,
            scale
        );

        if (this->m_styleFlags & 0x100) {
            auto shadowColor = this->m_shadowColor;
            // TODO alpha manipulation

            auto shadowOffset = this->m_shadowOffset;
            shadowOffset.x *= this->m_layoutScale;
            shadowOffset.y *= this->m_layoutScale;

            TextBlockAddShadow(this->m_string, shadowColor, shadowOffset);
        }

        if (this->m_alphaGradientStart > 0) {
            // TODO
        }

        this->RefreshEmbeddedTextures();

        // TODO
        // - logic if region flags & 0x40, CLayoutFrame::AnimData_HasActivePosition?
    }

    this->OnRegionChanged();
}
