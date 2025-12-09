#include "gx/font/GxuFontInternal.hpp"
#include "gx/font/CGxFont.hpp"
#include "gx/font/GxuFont.hpp"
#include <storm/Error.hpp>
#include <tempest/Math.hpp>
#include <cmath>

void CalculateYOffset(uint32_t pixelHeight, uint32_t a2, FT_Face face, uint32_t glyphHeight, int32_t* yOffset, int32_t* yStart) {
    uint32_t v6 = 0;
    int32_t v8 = 0;

    if (glyphHeight <= pixelHeight) {
        uint32_t v9 = face->glyph->bitmap_top;

        if (v9 <= a2) {
            v8 = a2 - v9;
        } else {
            v8 = 0;
            v6 = v9 - a2;
        }
    }

    uint32_t v10 = v8 <= 0 ? 0 : v8;
    uint32_t v11 = pixelHeight - glyphHeight;

    if (v11 >= v10) {
        *yOffset = v6;
        *yStart = v10;
    } else {
        *yOffset = pixelHeight - v10 - glyphHeight;
        *yStart = v11;
    }
}

int32_t IGxuFontGlyphRenderGlyph(FT_Face face, uint32_t pixelHeight, uint32_t code, uint32_t baseline, GLYPHBITMAPDATA* dataPtr, int32_t monochrome, uint32_t a7) {
    STORM_ASSERT(face);
    STORM_ASSERT(pixelHeight);
    STORM_ASSERT(dataPtr);

    if (!FT_Get_Char_Index(face, code)) {
        return 0;
    }

    if (!FREETYPE_RenderGlyph(code, monochrome != 0, face)) {
        return 0;
    }

    auto glyph = face->glyph;

    uint32_t width = glyph->bitmap.width;
    uint32_t height = std::min(static_cast<uint32_t>(glyph->bitmap.rows), pixelHeight);
    size_t dataSize = glyph->bitmap.pitch * glyph->bitmap.rows;
    auto srcData = glyph->bitmap.buffer;
    uint32_t pitch = glyph->bitmap.pitch;

    int32_t dummyGlyph = 0;

    if (!width || !height || !srcData || !pitch || !dataSize) {
        width = (pixelHeight + 3) >> 2;
        height = pixelHeight;

        if (!width) {
            width = pixelHeight;
        }

        if (monochrome) {
            pitch = (width + 7) & 0xFFFFFFF8;
        } else {
            pitch = width;
        }

        dataSize = pixelHeight * pitch;
        dummyGlyph = 1;
    }

    void* data = SMemAlloc(dataSize, __FILE__, __LINE__, 0x0);

    if (data) {
        memset(data, 0, dataSize);
    }

    if (srcData) {
        memcpy(data, srcData, dataSize);
    }

    dataPtr->m_data = data;
    dataPtr->m_dataSize = dataSize;
    dataPtr->m_glyphPitch = pitch;
    dataPtr->m_glyphWidth = width;
    dataPtr->m_glyphHeight = height;
    dataPtr->m_glyphCellWidth = width + a7;
    dataPtr->m_glyphAdvance = (double)(face->glyph->metrics.horiAdvance / 64) + 1.0;
    dataPtr->m_glyphBearing = (double)face->glyph->metrics.horiBearingX * 0.015625;
    dataPtr->m_yOffset = 0;
    dataPtr->m_yStart = 0;

    if (width && height && data && !dummyGlyph) {
        CalculateYOffset(pixelHeight, baseline, face, height, &dataPtr->m_yOffset, &dataPtr->m_yStart);
    }

    return 1;
}

uint32_t InternalGetMaxCharsWithinWidth(CGxFont* face, const char* text, float height, float maxWidth, uint32_t lineBytes, float* extent, uint32_t flags, float a8, float scale, uint32_t* bytesInString, float* widthArray, float* widthArrayGuard) {
    STORM_ASSERT(face);
    STORM_ASSERT(text);
    STORM_ASSERT(extent);

    uint32_t numChars = 0;
    int32_t billboarded = (flags & 0x80) != 0;

    if (!billboarded && (height == 0.0f || flags & 0x4)) {
        height = GxuFontGetOneToOneHeight(face);
    }

    float width = 0.0f;

    if (a8 > 0.0f) {
        width += ceil(ScreenToPixelWidth(0, a8));
    }

    if (face->m_flags & 0x8) {
        width += 4.0f;
    } else if (face->m_flags & 0x1) {
        width += 2.0f;
    }

    float pixelSize = face->GetPixelSize();
    float pixelHeight = ScreenToPixelHeight(billboarded, height);
    float pixelScale = pixelHeight / pixelSize;
    float pixelMaxWidth = CMath::fuint_pi((pixelSize * GetScreenPixelWidth() * maxWidth) / std::max(pixelHeight, 1.0f));

    const char* originalText = text;
    float lastWidth = 0.0f;
    int32_t advance;
    uint32_t code;
    uint32_t prevCode = 0;

    while (*text && lineBytes) {
        QUOTEDCODE quotedCode = GxuDetermineQuotedCode(text, advance, nullptr, flags, code);

        text += advance;
        lineBytes -= advance;

        switch (quotedCode) {
            case CODE_COLORON:
            case CODE_COLORRESTORE:
            case CODE_HYPERLINKSTART:
            case CODE_HYPERLINKSTOP:
            case CODE_TEXTURESTOP:
                break;

            case CODE_NEWLINE: {
                goto DONE;
            }

            case CODE_TEXTURESTART: {
                // TODO
            }

            default: {
                if (face->NewCodeDesc(code)) {
                    float stepWidth = 0.0f;

                    if (prevCode) {
                        stepWidth = flags & 0x10
                            ? face->ComputeStepFixedWidth(prevCode, code)
                            : face->ComputeStep(prevCode, code);
                    }

                    float characterWidth = GetCharacterWidth(text, flags, code, face, height);

                    if (pixelMaxWidth < width + stepWidth + characterWidth) {
                        text -= advance;
                        goto DONE;
                    }

                    width += stepWidth;
                    numChars++;

                    prevCode = code;
                    lastWidth = characterWidth;

                    if (widthArray) {
                        float v34 = (lastWidth + width) * pixelScale;

                        if (billboarded) {
                            *widthArray = v34;
                        } else {
                            *widthArray = v34 / GetScreenPixelWidth();
                        }

                        widthArray++;
                    }
                } else {
                    numChars++;
                }
            }
        }
    }

    DONE:

    float v26 = (lastWidth + width) * pixelScale;

    if (billboarded) {
        *extent = v26;
    } else {
        *extent = v26 / GetScreenPixelWidth();
    }

    if (bytesInString) {
        *bytesInString = text - originalText;
    }

    return numChars;
}

void InternalGetTextExtent(CGxFont* font, const char* text, uint32_t numBytes, float height, float* extent, uint32_t flags, float a7, float a8) {
    STORM_ASSERT(font);
    STORM_ASSERT(text);
    STORM_ASSERT(extent);

    int32_t billboarded = (flags & 0x80) != 0;

    if (height == 0.0f || flags & 0x4) {
        height = GxuFontGetOneToOneHeight(font);
    }

    float width = 0.0f;
    float lastWidth = 0.0f;
    float maxWidth = 0.0f;

    if (a7 > 0.0f) {
        width = ceil(ScreenToPixelWidth(0, a7));
    }

    if (font->m_flags & 0x8) {
        width += 4.0f;
    } else if (font->m_flags & 0x1) {
        width += 2.0f;
    }

    uint32_t prevCode = 0;

    while (numBytes && *text) {
        int32_t advance;
        uint32_t code;
        CImVector quotedColor;

        QUOTEDCODE quotedCode = GxuDetermineQuotedCode(text, advance, nullptr, flags, code);

        numBytes -= advance;
        text += advance;

        if (
            quotedCode == CODE_COLORON
            || quotedCode == CODE_COLORRESTORE
            || quotedCode == CODE_HYPERLINKSTART
            || quotedCode == CODE_HYPERLINKSTOP
            || quotedCode == CODE_TEXTURESTOP
        ) {
            continue;
        }

        if (quotedCode == CODE_NEWLINE || code == '\n') {
            maxWidth = std::max(maxWidth, lastWidth + width);
            lastWidth = 0.0f;
            width = 0.0f;
        } else if (quotedCode == CODE_TEXTURESTART) {
            // TODO
        } else if (font->NewCodeDesc(code)) {
            float stepWidth = 0.0f;

            if (prevCode) {
                stepWidth = flags & 0x10
                    ? font->ComputeStepFixedWidth(prevCode, code)
                    : font->ComputeStep(prevCode, code);
            }

            lastWidth = GetCharacterWidth(text, flags, code, font, height);
            width += stepWidth;

            prevCode = code;
        }
    }

    float pixelSize = font->GetPixelSize();
    float pixelHeight = ScreenToPixelHeight(billboarded, height);
    float pixelScale = pixelHeight / pixelSize;

    maxWidth = std::max(maxWidth, lastWidth + width);
    maxWidth *= pixelScale;

    *extent = billboarded ? maxWidth : PixelToScreenWidth(maxWidth);
}
