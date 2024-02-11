#include "gx/Font.hpp"
#include "gx/font/CGxFont.hpp"
#include "gx/font/CGxString.hpp"
#include "gx/font/CGxStringBatch.hpp"
#include "gx/font/Wrap.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Device.hpp"
#include "gx/FontInternal.hpp"
#include "gx/Gx.hpp"
#include "gx/Shader.hpp"
#include <algorithm>
#include <cstring>
#include <new>
#include <storm/Error.hpp>
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include <storm/Unicode.hpp>
#include <tempest/Math.hpp>

CGxShader* g_fontPixelShader[1];
CGxShader* g_fontVertexShader[2];
TSList<CGxFont, TSGetLink<CGxFont>> g_fonts;
TSHashTable<FONTHASHOBJ, HASHKEY_STR> s_fontHash;
uint32_t g_heightPixels;
uint32_t g_widthPixels;
float g_indentPixelWidth;
float g_indentNormWidth;
TSList<CGxStringBatch, TSGetLink<CGxStringBatch>> s_unusedBatches;
STORM_LIST(CGxString) g_freeStrings;
STORM_LIST(CGxString) g_strings;

FONTHASHOBJ::~FONTHASHOBJ() {
    if (this->font) {
       GxuFontDestroyFont(this->font);
    }
}

TEXTBLOCK::~TEXTBLOCK() {
    GxuFontDestroyString(this->string);
}

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

int32_t GxuFontClearBatch(CGxStringBatch* batch) {
    if (!batch) {
        return 0;
    }

    batch->m_fontBatch.Clear();

    return batch != 0;
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

void GxuFontGetTextExtent(CGxFont* font, const char* text, uint32_t numBytes, float a4, float* extent, float a6, float a7, float a8, uint32_t flags) {
    InternalGetTextExtent(font, text, numBytes, a4, extent, flags, a6, a7);
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
    int32_t v8 = 0;
    float extent = 0.0f;
    float v17 = 0.0f;
    float v18 = 0.0f;
    const char* currentText = text;
    const char* nextText = nullptr;
    bool v21 = true;

    while (currentText && *currentText) {
        QUOTEDCODE quotedCode = GxuDetermineQuotedCode(currentText, advance, nullptr, flags, code);

        if (flags & 0x2000 && v8 == 1) {
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

        v8++;
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
        v8++;
    }

    if (!v8) {
        return 0.0f;
    }

    float v22 = GetScreenPixelHeight();
    float v14 = v22 * a7;
    float v13 = v14 + 0.99994999f;
    float v23 = v13 / v22;

    return (v18 / v22 + v23 * (float)(v8 - 1) + (float)v8 * a3);
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
        DDCToNDCWidth(a9),
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
