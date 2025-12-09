#include "gx/Font.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Device.hpp"
#include "gx/font/CGxFont.hpp"
#include "gx/font/GxuFont.hpp"
#include <storm/Error.hpp>
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include <tempest/Math.hpp>
#include <algorithm>
#include <new>

CGxShader* g_fontPixelShader[1];
CGxShader* g_fontVertexShader[2];
TSHashTable<FONTHASHOBJ, HASHKEY_STR> s_fontHash;
uint32_t g_heightPixels;
uint32_t g_widthPixels;
float g_indentPixelWidth;
float g_indentNormWidth;

FONTHASHOBJ::~FONTHASHOBJ() {
    if (this->font) {
       GxuFontDestroyFont(this->font);
    }
}

TEXTBLOCK::~TEXTBLOCK() {
    GxuFontDestroyString(this->string);
}

uint32_t ConvertStringFlags(uint32_t flags) {
    uint32_t convertedFlags = 0x0;

    if (flags & 0x100) {
        convertedFlags |= 0x1;
    }

    if (flags & 0x200) {
        convertedFlags |= 0x4;
    }

    if (flags & 0x400) {
        convertedFlags |= 0x8;
    }

    if (flags & 0x800) {
        convertedFlags |= 0x10;
    }

    if (flags & 0x40) {
        convertedFlags |= 0x2;
    }

    if (flags & 0x1000) {
        convertedFlags |= 0x40;
    }

    if (flags & 0x2000) {
        convertedFlags |= 0x100;
    }

    if (flags & 0x4000) {
        convertedFlags |= 0x200;
    }

    if (flags & 0x8000) {
        convertedFlags |= 0x400;
    }

    if (flags & 0x80) {
        convertedFlags |= 0x800;
    }

    if (flags & 0x10000) {
        convertedFlags |= 0x1000;
    }

    if (flags & 0x20000) {
        convertedFlags |= 0x2000;
    }

    return convertedFlags;
}

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

void TextBlockAddShadow(HTEXTBLOCK string, CImVector color, const C2Vector& offset) {
    STORM_ASSERT(string);

    C2Vector ndcOffset;
    DDCToNDC(offset.x, offset.y, &ndcOffset.x, &ndcOffset.y);

    GxuFontAddShadow(TextBlockGetStringPtr(string), color, offset);
}

HTEXTBLOCK TextBlockCreate(HTEXTFONT font, const char* text, const CImVector& color, const C3Vector& pos, float fontHeight, float blockWidth, float blockHeight, uint32_t flags, float charSpacing, float lineSpacing, float scale) {
    STORM_ASSERT(font);
    STORM_ASSERT(text);

    auto m = SMemAlloc(sizeof(TEXTBLOCK), __FILE__, __LINE__, 0x0);
    auto textBlock = new (m) TEXTBLOCK();

    C3Vector position = { 0.0f, 0.0f, pos.z };
    DDCToNDC(pos.x, pos.y, &position.x, &position.y);

    EGxFontHJusts hjust = GxHJ_Center;

    if (flags & 0x4) {
        hjust = GxHJ_Right;
    } else if (!(flags & 0x2) && flags & 0x1) {
        hjust = GxHJ_Left;
    }

    EGxFontVJusts vjust = GxVJ_Middle;

    if (flags & 0x8) {
        vjust = GxVJ_Top;
    } else if (flags & 0x20) {
        vjust = GxVJ_Bottom;
    }

    uint32_t v16 = ConvertStringFlags(flags);

    float v15 = DDCToNDCWidth(charSpacing);
    float v20 = DDCToNDCHeight(lineSpacing);
    float v21 = DDCToNDCHeight(blockHeight);
    float v22 = DDCToNDCWidth(blockWidth);
    float v23 = DDCToNDCHeight(fontHeight);

    GxuFontCreateString(
        reinterpret_cast<FONTHASHOBJ*>(font)->font,
        text,
        v23,
        position,
        v22,
        v21,
        v20,
        textBlock->string,
        vjust,
        hjust,
        v16,
        color,
        v15,
        scale
    );

    return HandleCreate(textBlock);
}

HTEXTFONT TextBlockGenerateFont(const char* fontName, uint32_t fontFlags, float fontHeight) {
    STORM_ASSERT(fontName);
    STORM_ASSERT(*fontName);

    float fontHeightNDC = DDCToNDCHeight(fontHeight);

    if (fontHeightNDC >= 1.0) {
        fontHeightNDC = 1.0;
    }

    char hashKey[276];

    SStrPrintf(hashKey, 276, "%s-%d-%f", fontName, fontFlags, fontHeightNDC);

    auto v7 = s_fontHash.Ptr(hashKey);

    if (v7) {
        return HandleCreate(v7);
    }

    v7 = s_fontHash.New(hashKey, 0, 0);

    uint32_t v14 = 0;

    if (fontFlags & FONT_OUTLINE) {
        v14 |= 0x1;
    }

    if (fontFlags & FONT_THICKOUTLINE) {
        v14 |= 0x8;
    }

    if (fontFlags & FONT_MONOCHROME) {
        v14 |= 0x2;
    }

    if (GxuFontCreateFont(fontName, fontHeightNDC, v7->font, v14)) {
        return HandleCreate(v7);
    } else {
        // TODO
        // sub_723270(v12);
        // (s_fontHash)(&s_fontHash, v12);

        return nullptr;
    }
}

uint32_t TextBlockGetFontFlags(HTEXTFONT fontHandle) {
    STORM_ASSERT(fontHandle);

    uint32_t gxFlags = GxuFontGetFontFlags(TextBlockGetFontPtr(fontHandle));
    uint32_t flags = 0;

    if (gxFlags & 0x1) {
        flags |= 0x1;
    }

    if (gxFlags & 0x2) {
        flags |= 0x2;
    }

    if (gxFlags & 0x8) {
        flags |= 0x4;
    }

    return flags;
}

const char* TextBlockGetFontName(HTEXTFONT fontHandle) {
    STORM_ASSERT(fontHandle);

    return GxuFontGetFontName(reinterpret_cast<FONTHASHOBJ*>(fontHandle)->font);
}

CGxFont* TextBlockGetFontPtr(HTEXTFONT fontHandle) {
    STORM_ASSERT(fontHandle);

    return reinterpret_cast<FONTHASHOBJ*>(fontHandle)->font;
}

uint32_t TextBlockGetMaxCharsWithinWidth(HTEXTFONT fontHandle, const char* text, float height, float maxWidth, uint32_t lineBytes, float* extent, float a7, float scale, float a9, uint32_t flags) {
    STORM_ASSERT(fontHandle);
    STORM_ASSERT(text);

    return GxuFontGetMaxCharsWithinWidth(
        TextBlockGetFontPtr(fontHandle),
        text,
        DDCToNDCHeight(height),
        DDCToNDCWidth(maxWidth),
        lineBytes,
        extent,
        DDCToNDCWidth(a7),
        scale,
        DDCToNDCWidth(a9),
        ConvertStringFlags(flags)
    );
}

uint32_t TextBlockGetMaxCharsWithinWidthAndHeight(HTEXTFONT fontHandle, const char* text, float height, float maxWidth, float maxHeight, uint32_t lineBytes, float a7, float scale, float a9, uint32_t flags) {
    STORM_ASSERT(fontHandle);
    STORM_ASSERT(text);

    return GxuFontGetMaxCharsWithinWidthAndHeight(
        TextBlockGetFontPtr(fontHandle),
        text,
        DDCToNDCHeight(height),
        DDCToNDCWidth(maxWidth),
        DDCToNDCHeight(maxHeight),
        lineBytes,
        DDCToNDCWidth(a7),
        scale,
        DDCToNDCHeight(a9),
        ConvertStringFlags(flags)
    );
}

CGxString* TextBlockGetStringPtr(HTEXTBLOCK stringHandle) {
    STORM_ASSERT(stringHandle);

    return reinterpret_cast<TEXTBLOCK*>(stringHandle)->string;
}

void TextBlockGetTextExtent(HTEXTFONT fontHandle, const char* text, uint32_t numChars, float fontHeight, float* extent, float a6, float scale, float a8, uint32_t flags) {
    STORM_ASSERT(fontHandle);
    STORM_ASSERT(text);
    STORM_ASSERT(extent);

    *extent = 0.0f;

    GxuFontGetTextExtent(
        TextBlockGetFontPtr(fontHandle),
        text,
        numChars,
        DDCToNDCHeight(fontHeight),
        extent,
        DDCToNDCWidth(a6),
        scale,
        DDCToNDCWidth(a8),
        ConvertStringFlags(flags)
    );

    NDCToDDC(*extent, 0.0f, extent, nullptr);
}

float TextBlockGetWrappedTextHeight(HTEXTFONT fontHandle, const char* text, float a3, float a4, const C2Vector& a5, float a6, float a7, uint32_t flags) {
    STORM_ASSERT(fontHandle);
    STORM_ASSERT(text);

    float shadowWidth;
    float shadowHeight;
    DDCToNDC(a5.x, a5.y, &shadowWidth, &shadowHeight);
    C2Vector shadowSize  = { shadowWidth, shadowHeight };

    float height = GxuFontGetWrappedTextHeight(
        TextBlockGetFontPtr(fontHandle),
        text,
        DDCToNDCHeight(a3),
        DDCToNDCWidth(a4),
        shadowSize,
        a6,
        DDCToNDCHeight(a7),
        ConvertStringFlags(flags)
    );

    return NDCToDDCHeight(height);
}

void TextBlockSetStringPos(HTEXTBLOCK stringHandle, const C3Vector& pos) {
    STORM_ASSERT(stringHandle);

    C3Vector ndcPos = { 0.0f, 0.0f, pos.z };
    DDCToNDC(pos.x, pos.y, &ndcPos.x, &ndcPos.y);
    GxuFontSetStringPosition(TextBlockGetStringPtr(stringHandle), ndcPos);
}

void TextBlockUpdateColor(HTEXTBLOCK stringHandle, const CImVector& color) {
    STORM_ASSERT(stringHandle);

    GxuFontSetStringColor(TextBlockGetStringPtr(stringHandle), color);
}
