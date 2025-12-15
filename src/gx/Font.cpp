#include "gx/Font.hpp"
#include "gx/font/CGxFont.hpp"
#include "gx/font/GxuFont.hpp"
#include <tempest/Math.hpp>
#include <algorithm>
#include <new>

uint32_t g_heightPixels;
uint32_t g_widthPixels;
float g_indentPixelWidth;
float g_indentNormWidth;

float GetCharacterWidth(const char* text, uint32_t flags, uint32_t prevCode, CGxFont* font, float a5) {
    if (!prevCode) {
        return 0.0f;
    }

    float width = 0.0f;

    while (*text) {
        int32_t advance;
        uint32_t code;
        QUOTEDCODE quotedCode = GxuDetermineQuotedCode(text, advance, nullptr, flags, code);

        text += advance;

        if (quotedCode == CODE_INVALIDCODE) {
            if (flags & 0x10) {
                // TODO
                // width = font->ComputeStepFixedWidth(prevCode, code);
            } else {
                width = font->ComputeStep(prevCode, code);
            }

            break;
        }

        if (quotedCode == CODE_NEWLINE) {
            break;
        }
    }

    if (width == 0.0f) {
        auto glyph = font->NewCodeDesc(prevCode);

        if (glyph) {
            width = font->GetGlyphBearing(glyph, flags & 0x80, a5) + glyph->bitmapData.m_glyphCellWidth;
        }
    }

    return width;
}

float GetIndentNormWidth() {
    return g_indentNormWidth;
}

float GetIndentPixelWidth() {
    return g_indentPixelWidth;
}

uint32_t GetScreenPixelHeight() {
    return g_heightPixels;
}

uint32_t GetScreenPixelWidth() {
    return g_widthPixels;
}

float PixelToScreenHeight(int32_t height) {
    return (double)height / (double)g_heightPixels;
}

float PixelToScreenWidth(int32_t width) {
    return (double)width / (double)g_widthPixels;
}

float PixelToScreenWidth(float width) {
    return width / (double)g_widthPixels;
}

float ScreenToPixelHeight(int32_t billboarded, float height) {
    if (billboarded) {
        return height;
    }

    float pixelCoords = static_cast<double>(g_heightPixels) * height;
    return CMath::fint_n(pixelCoords);
}

float ScreenToPixelWidth(int32_t billboarded, float width) {
    if (billboarded) {
        return width;
    }

    float pixelCoords = static_cast<double>(g_widthPixels) * width;
    return CMath::fint_n(pixelCoords);
}

float Sub6C2280(FT_Face face, float height) {
    int32_t numFixedSizes = face->num_fixed_sizes;

    if (numFixedSizes == 0) {
        return height;
    }

    int32_t v4 = INT_MAX;

    auto availableSizes = face->available_sizes;

    for (int32_t i = 0; i < numFixedSizes; i++) {
        if (v4 >= availableSizes->height) {
            v4 = availableSizes->height;
        }

        availableSizes++;
    }

    float v7 = PixelToScreenHeight(v4);

    return v7 >= height ? v7 : height;
}

