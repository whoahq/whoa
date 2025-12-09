#include "gx/font/GxuFont.hpp"
#include "gx/font/CGxFont.hpp"
#include "gx/font/CGxStringBatch.hpp"
#include "gx/font/FreeType.hpp"
#include "gx/font/GxuFontInternal.hpp"
#include "gx/font/Wrap.hpp"
#include "gx/Device.hpp"
#include "gx/Gx.hpp"
#include "gx/Types.hpp"
#include <storm/Memory.hpp>
#include <storm/Unicode.hpp>
#include <tempest/Math.hpp>
#include <tempest/Rect.hpp>

STORM_LIST(CGxFont) g_fonts;
STORM_LIST(CGxString) g_freeStrings;
STORM_LIST(CGxString) g_strings;

static TSList<CGxStringBatch, TSGetLink<CGxStringBatch>> s_unusedBatches;

QUOTEDCODE GxuDetermineQuotedCode(const char* text, int32_t& advance, CImVector* color, uint32_t flags, uint32_t& wide) {
    wide = SUniSGetUTF8(reinterpret_cast<const uint8_t*>(text), &advance);

    switch (wide) {
        case 0x0:
        case 0xFFFFFFFF:
            return CODE_INVALIDCODE;

        case '\r':
            advance = 2 - (SUniSGetUTF8(reinterpret_cast<const uint8_t*>(text + 1), &advance) != '\n');
            return CODE_NEWLINE;

        case '\n':
            advance = 1;
            return CODE_NEWLINE;
    }

    if (wide != '|' || flags & 0x800) {
        return CODE_INVALIDCODE;
    }

    auto quotedCode = text[advance];

    if (!quotedCode) {
        return CODE_INVALIDCODE;
    }

    switch (quotedCode) {
        case 'N':
        case 'n': {
            if (flags & 0x200) {
                return CODE_INVALIDCODE;
            }

            advance = 2;
            return CODE_NEWLINE;
        }

        // TODO handle other control codes
    }

    // TODO remainder of function

    return CODE_INVALIDCODE;
}

int32_t GxuFontAddToBatch(CGxStringBatch* batch, CGxString* string) {
    if (batch && string) {
        batch->AddString(string);
        return 1;
    } else {
        return 0;
    }
}

void GxuFontAddShadow(CGxString* string, const CImVector& color, const C2Vector& offset) {
    if (string) {
        if (!(string->m_flags & 0x80)) {
            string->AddShadow(offset, color);
        }
    }
}

CGxStringBatch* GxuFontCreateBatch(bool a1, bool a2) {
    CGxStringBatch* batch;

    if (s_unusedBatches.Head()) {
        batch = s_unusedBatches.Head();
        s_unusedBatches.UnlinkNode(batch);
    } else {
        auto m = SMemAlloc(sizeof(CGxStringBatch), __FILE__, __LINE__, 0x8);
        batch = new (m) CGxStringBatch();
    }

    if (a1) {
        batch->m_flags |= 0x1;
    } else {
        batch->m_flags &= ~0x1;
    }

    if (a2) {
        batch->m_flags |= 0x2;
    } else {
        batch->m_flags &= ~0x2;
    }

    return batch;
}

int32_t GxuFontCreateFont(const char* name, float fontHeight, CGxFont*& face, uint32_t flags) {
    STORM_ASSERT(name);
    STORM_ASSERT(*name);
    STORM_ASSERT((fontHeight <= 1.0f) && (fontHeight > 0.0f));

    CGxFont* newFace = g_fonts.NewNode(2, 0, 0);

    uint32_t v12 = flags;

    if (flags & 0x8) {
        v12 |= 0x1;
    }

    int32_t v13 = newFace->Initialize(name, v12, fontHeight);

    if (!v13) {
        g_fonts.DeleteNode(newFace);
        newFace = nullptr;
    }

    face = newFace;

    return v13;
}

int32_t GxuFontCreateString(CGxFont* face, const char* text, float fontHeight, const C3Vector& position, float blockWidth, float blockHeight, float spacing, CGxString*& string, EGxFontVJusts vertJustification, EGxFontHJusts horzJustification, uint32_t flags, const CImVector& color, float charSpacing, float scale) {
    STORM_ASSERT(face);
    STORM_ASSERT(text);
    // TODO
    // STORM_ASSERT(fontHeight || (flags & EGxStringFlags_FixedSize));
    STORM_ASSERT(vertJustification < GxVJ_Last);
    STORM_ASSERT(horzJustification < GxHJ_Last);

    auto newString = CGxString::GetNewString(1);

    int32_t result = newString->Initialize(
        fontHeight,
        position,
        blockWidth,
        blockHeight,
        face,
        text,
        vertJustification,
        horzJustification,
        spacing,
        flags & ~0x1,
        color,
        scale
    );

    if (result) {
        string = newString;
    } else {
        GxuFontDestroyString(newString);
    }

    return result;
}

int32_t GxuFontDestroyBatch(CGxStringBatch* batch) {
    if (!batch) {
        return 0;
    }

    batch->m_fontBatch.Clear();

    s_unusedBatches.LinkToTail(batch);

    return 1;
}

void GxuFontDestroyFont(CGxFont*& font) {
    if (font) {
        g_fonts.DeleteNode(font);
    }

    font = nullptr;
}

void GxuFontDestroyString(CGxString*& string) {
    if (string) {
        string->Unlink();
        string->Recycle();
        string = nullptr;
    }
}

uint32_t GxuFontGetFontFlags(CGxFont* font) {
    if (font) {
        return font->m_flags;
    }

    return 0;
}

const char* GxuFontGetFontName(CGxFont* font) {
    return font
        ? font->GetName()
        : nullptr;
}

uint32_t GxuFontGetMaxCharsWithinWidth(CGxFont* font, const char* text, float height, float maxWidth, uint32_t lineBytes, float* extent, float a7, float scale, float a9, uint32_t flags) {
    return InternalGetMaxCharsWithinWidth(
        font,
        text,
        height,
        maxWidth,
        lineBytes,
        extent,
        flags,
        a7,
        scale,
        nullptr,
        nullptr,
        nullptr
    );
}

uint32_t GxuFontGetMaxCharsWithinWidthAndHeight(CGxFont* font, const char* text, float fontHeight, float maxWidth, float maxHeight, uint32_t lineBytes, float a7, float scale, float a9, uint32_t flags) {
    if (!font) {
        return 0;
    }

    if (!text || !*text) {
        return 0;
    }

    if (fontHeight == 0.0f || maxWidth == 0.0f || maxHeight == 0.0f || g_heightPixels == 0.0f) {
        return 0;
    }

    if (lineBytes == 0) {
        return 0;
    }

    if (flags & 0x4) {
        fontHeight = GxuFontGetOneToOneHeight(font);
    }

    auto v24 = static_cast<float>(g_heightPixels) * a9;
    auto v22 = CMath::fuint_pi(v24);
    auto v12 = static_cast<float>(v22) / static_cast<float>(g_heightPixels);
    bool v26 = true;
    auto v27 = v12;
    v24 = 0.0f;
    auto v13 = fontHeight;
    const char* nextText;

    uint32_t maxChars = 0;

    auto currentText = text;
    int32_t v25 = 0;
    while (*currentText) {
        if (flags & 0x2000 && v25 == 1) {
            maxWidth -= g_indentNormWidth;
        }

        float v21;
        CalcWrapPoint(
            font,
            currentText,
            fontHeight,
            maxWidth,
            &v22,
            &v21,
            &nextText,
            a7,
            flags,
            &v26,
            0,
            scale
        );

        v22 = nextText - currentText;
        if (nextText == currentText) {
            break;
        }

        auto v15 = v24;
        auto v16 = v25 + 1 < 0;
        auto v17 = v24 + fontHeight;

        v25++;

        float v18 = v25;

        if (v16) {
            v18 = v18 + 4294967300.0;
        }

        if (v18 * 0.00000095367431640625 + maxHeight < v17) {
            break;
        }

        maxChars += nextText - currentText;
        currentText = nextText;
        v24 = fontHeight + v15 + v27;

        if (!v26 || !nextText) {
            break;
        }
    }

    return maxChars;
}

float GxuFontGetOneToOneHeight(CGxFont* font) {
    STORM_ASSERT(font);

    return PixelToScreenHeight(font->GetPixelSize());
}

void GxuFontGetTextExtent(CGxFont* font, const char* text, uint32_t numBytes, float height, float* extent, float a6, float scale, float a8, uint32_t flags) {
    InternalGetTextExtent(font, text, numBytes, height, extent, flags, a6, scale);
}

float GxuFontGetWrappedTextHeight(CGxFont* font, const char* text, float a3, float a4, const C2Vector& a5, float a6, float a7, uint32_t flags) {
    STORM_ASSERT(font);
    STORM_ASSERT(text);

    if (flags & 0x04) {
        a3 = GxuFontGetOneToOneHeight(font);
    }

    int32_t advance;
    uint32_t numBytes;
    uint32_t code;
    int32_t numLines = 0;
    float extent = 0.0f;
    float v17 = 0.0f;
    float v18 = 0.0f;
    const char* currentText = text;
    const char* nextText = nullptr;
    bool v21 = true;

    while (currentText && *currentText) {
        QUOTEDCODE quotedCode = GxuDetermineQuotedCode(currentText, advance, nullptr, flags, code);

        if (flags & 0x2000 && numLines == 1) {
            a4 = a4 - g_indentNormWidth;
        }

        if (quotedCode == CODE_NEWLINE) {
            nextText = currentText + advance;
        } else {
            CalcWrapPoint(
                font,
                currentText,
                a3,
                a4,
                &numBytes,
                &extent,
                &nextText,
                a5.x,
                flags,
                &v21,
                &v17,
                a6
            );

            if (currentText == nextText) {
                break;
            }
        }

        numLines++;
        currentText = nextText;

        float v14 = (float)GetScreenPixelHeight() * a3;
        if (v14 < v17) {
            v18 = v17 - v14 + v18;
        }

        if (flags & 0x02) {
            break;
        }
    }

    if (!(flags & 0x02) && currentText > text && GxuDetermineQuotedCode(currentText - 1, advance, nullptr, flags, code) == CODE_NEWLINE) {
        numLines++;
    }

    if (!numLines) {
        return 0.0f;
    }

    float v22 = GetScreenPixelHeight();
    float v14 = v22 * a7;
    auto v13 = CMath::fuint_pi(v14);
    auto v23 = static_cast<float>(v13) / v22;

    return (v18 / v22 + v23 * (float)(numLines - 1) + (float)numLines * a3);
}

void GxuFontInitialize() {
    g_theGxDevicePtr->ShaderCreate(g_fontVertexShader, GxSh_Vertex, "Shaders\\Vertex", "UI", 2);
    g_theGxDevicePtr->ShaderCreate(g_fontPixelShader, GxSh_Pixel, "Shaders\\Pixel", "UI", 1);

    BATCHEDRENDERFONTDESC::Initialize();

    FreeTypeInitialize();

    GxuFontWindowSizeChanged();

    // TODO
    // sub_6BD160();
}

void GxuFontRenderBatch(CGxStringBatch* batch) {
    if (batch) {
        batch->RenderBatch();
    }
}

int32_t GxuFontSetStringColor(CGxString* string, const CImVector& color) {
    STORM_ASSERT(string);

    string->SetColor(color);

    return 1;
}

void GxuFontSetStringPosition(CGxString* string, const C3Vector& position) {
    STORM_ASSERT(string);

    string->SetStringPosition(position);
}

void GxuFontUpdate() {
    for (auto string = g_strings.Head(); string; string = g_strings.Link(string)->Next()) {
        string->Tick();
    }

    // TODO
}

void GxuFontWindowSizeChanged() {
    static CRect s_currentRect = { 0.0f, 0.0f, 0.0f, 0.0f };

    CRect rect = { 0.0f, 0.0f, 0.0f, 0.0f };

    GxCapsWindowSize(rect);

    if (rect.maxY - rect.minY == 0.0f || rect.maxX - rect.minX == 0.0f) {
        rect = { 0.0f, 0.0f, 480.0f, 640.0f };
    }

    if (s_currentRect == rect) {
        return;
    }

    s_currentRect = rect;

    g_widthPixels = rect.maxX - rect.minX;
    g_heightPixels = rect.maxY - rect.minY;
    g_indentPixelWidth = 15.0f;
    g_indentNormWidth = 15.0f / (rect.maxX - rect.minX);

    // TODO
    // - walk s_fonts and trigger HandleScreenSizeChange
}
