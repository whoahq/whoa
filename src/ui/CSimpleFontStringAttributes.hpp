#ifndef UI_C_SIMPLE_FONT_STRING_ATTRIBUTES_HPP
#define UI_C_SIMPLE_FONT_STRING_ATTRIBUTES_HPP

#include <cstdint>
#include <common/String.hpp>
#include <tempest/Vector.hpp>

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
        void SetColor(const CImVector&);
        int32_t SetFont(const char*, float, uint32_t);
        void SetIndented(bool);
        void SetJustifyV(uint8_t);
        void SetJustifyH(uint8_t);
        void SetNonSpaceWrap(bool);
        void SetShadow(const CImVector&, const C2Vector&);
        void SetSpacing(float);
        void SetStyleFlags(uint32_t);
        void Update(CSimpleFontString*, uint32_t);
        void Update(CSimpleFontStringAttributes&, uint32_t);
};

#endif
