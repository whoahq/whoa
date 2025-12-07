#include "ui/simple/CSimpleFontStringAttributes.hpp"
#include "gx/Font.hpp"
#include "math/Utils.hpp"
#include "ui/simple/CSimpleFontString.hpp"
#include <storm/Error.hpp>

void CSimpleFontStringAttributes::SetColor(const CImVector& color) {
    if (this->m_color == color && this->m_flags & 0x400) {
        return;
    }

    this->m_color = color;
    this->m_flags |= (0x4 | 0x400);
}

int32_t CSimpleFontStringAttributes::SetFont(const char* fontName, float fontHeight, uint32_t fontFlags) {
    if (!fontName) {
        return 0;
    }

    // Return early if existing attributes match arguments
    if (
        this->m_font.GetString()
        && !SStrCmpI(fontName, this->m_font.m_str, STORM_MAX_STR)
        && AreEqual(fontHeight, this->m_fontHeight, WHOA_EPSILON_1)
        && fontFlags == this->m_fontFlags
    ) {
        return 1;
    }

    if (fontHeight <= 0.0f) {
        return 0;
    }

    auto v6 = TextBlockGenerateFont(fontName, fontFlags, fontHeight);

    if (!v6) {
        return 0;
    }

    HandleClose(v6);

    this->m_font.Copy(fontName);
    this->m_fontHeight = fontHeight;
    this->m_fontFlags = fontFlags;
    this->m_flags |= (0x1 | 0x100);

    return 1;
}

void CSimpleFontStringAttributes::SetIndented(bool indented) {
    uint32_t styleFlags = indented
        ? this->m_styleFlags | 0x20000
        : this->m_styleFlags & ~0x20000;

    if (this->m_styleFlags == styleFlags && this->m_flags & 0x200) {
        return;
    }

    this->m_styleFlags = styleFlags;
    this->m_flags |= (0x2 | 0x200);
}

void CSimpleFontStringAttributes::SetJustifyH(uint8_t justify) {
    uint32_t styleFlags = this->m_styleFlags ^ ((justify ^ this->m_styleFlags) & 0x7);

    if (this->m_styleFlags == styleFlags && this->m_flags & 0x200) {
        return;
    }

    this->m_styleFlags = styleFlags;
    this->m_flags |= (0x2 | 0x200);
}

void CSimpleFontStringAttributes::SetJustifyV(uint8_t justify) {
    uint32_t styleFlags = this->m_styleFlags ^ ((justify ^ this->m_styleFlags) & 0x38);

    if (this->m_styleFlags == styleFlags && this->m_flags & 0x200) {
        return;
    }

    this->m_styleFlags = styleFlags;
    this->m_flags |= (0x2 | 0x200);
}

void CSimpleFontStringAttributes::SetNonSpaceWrap(bool wrap) {
    uint32_t styleFlags = wrap
        ? this->m_styleFlags | 0x1000
        : this->m_styleFlags & ~0x1000;

    if (this->m_styleFlags == styleFlags && this->m_flags & 0x200) {
        return;
    }

    this->m_styleFlags = styleFlags;
    this->m_flags |= (0x2 | 0x200);
}

void CSimpleFontStringAttributes::SetShadow(const CImVector& shadowColor, const C2Vector& shadowOffset) {
    if (this->m_shadowColor == shadowColor && this->m_shadowOffset == shadowOffset && this->m_flags & 0x800) {
        return;
    }

    this->m_shadowColor = shadowColor;
    this->m_shadowOffset = shadowOffset;
    this->m_flags |= (0x8 | 0x800);
}

void CSimpleFontStringAttributes::SetSpacing(float spacing) {
    if (AreEqual(spacing, this->m_spacing, WHOA_EPSILON_1) && this->m_flags & 0x1000) {
        return;
    }

    this->m_spacing = spacing;
    this->m_flags |= (0x10 | 0x1000);
}

void CSimpleFontStringAttributes::SetStyleFlags(uint32_t styleFlags) {
    if (this->m_styleFlags == styleFlags && this->m_flags & 0x200) {
        return;
    }

    this->m_styleFlags = styleFlags;
    this->m_flags |= (0x2 | 0x200);
}

void CSimpleFontStringAttributes::Update(CSimpleFontString* fontString, uint32_t flags) {
    STORM_ASSERT(fontString);

    uint32_t v5 = flags & this->m_flags;

    if (this->m_flags & 0x100 && v5 & 0x1) {
        fontString->SetFont(this->m_font.GetString(), this->m_fontHeight, this->m_fontFlags, 0);
    }

    if (this->m_flags & 0x200 && v5 & 0x2) {
        fontString->SetStyleFlags(this->m_styleFlags);
    }

    if (this->m_flags & 0x400 && v5 & 0x4) {
        fontString->SetVertexColor(this->m_color);
    }

    if (this->m_flags & 0x800 && v5 & 0x8) {
        if (this->m_shadowOffset.x == 0.0f && this->m_shadowOffset.y == 0.0f) {
            fontString->RemoveShadow();
        } else {
            fontString->AddShadow(this->m_shadowColor, this->m_shadowOffset);
        }
    }

    if (this->m_flags & 0x1000 && v5 & 0x10) {
        fontString->SetSpacing(this->m_spacing);
    }
}

void CSimpleFontStringAttributes::Update(CSimpleFontStringAttributes& attributes, uint32_t flags) {
    uint32_t v4 = flags & this->m_flags;

    if (this->m_flags & 0x100 && v4 & 0x1) {
        attributes.SetFont(this->m_font.GetString(), this->m_fontHeight, this->m_fontFlags);
    }

    if (this->m_flags & 0x200 && v4 & 0x2) {
        attributes.SetStyleFlags(this->m_styleFlags);
    }

    if (this->m_flags & 0x400 && v4 & 0x4) {
        attributes.SetColor(this->m_color);
    }

    if (this->m_flags & 0x800 && v4 & 0x8) {
        attributes.SetShadow(this->m_shadowColor, this->m_shadowOffset);
    }

    if (this->m_flags & 0x1000 && v4 & 0x10) {
        attributes.SetSpacing(this->m_spacing);
    }
}
