#ifndef UI_SIMPLE_C_SIMPLE_FONT_STRING_ATTRIBUTES_HPP
#define UI_SIMPLE_C_SIMPLE_FONT_STRING_ATTRIBUTES_HPP

#include <common/String.hpp>
#include <tempest/Vector.hpp>
#include <cstdint>

class CSimpleFontString;

class CSimpleFontStringAttributes {
    public:
        // Member variables
        uint32_t m_flags = 0;
        RCString m_font;
        float m_fontHeight = 0.0f;
        uint32_t m_fontFlags = 0;
        float m_spacing = 0.0f;
        uint32_t m_styleFlags = 0x2 | 0x10 | 0x200;
        CImVector m_color = { 0, 0, 0, 0 };
        CImVector m_shadowColor = { 0, 0, 0, 0 };
        C2Vector m_shadowOffset;

        // Member functions
        void SetColor(const CImVector& color);
        int32_t SetFont(const char* fontName, float fontHeight, uint32_t fontFlags);
        void SetIndented(bool indented);
        void SetJustifyV(uint8_t justify);
        void SetJustifyH(uint8_t justify);
        void SetNonSpaceWrap(bool wrap);
        void SetShadow(const CImVector& shadowColor, const C2Vector& shadowOffset);
        void SetSpacing(float spacing);
        void SetStyleFlags(uint32_t styleFlags);
        void Update(CSimpleFontString* fontString, uint32_t flags);
        void Update(CSimpleFontStringAttributes& attributes, uint32_t flags);
};

#endif
