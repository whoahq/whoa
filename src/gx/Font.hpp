#ifndef GX_FONT_HPP
#define GX_FONT_HPP

#include "gx/font/FreeType.hpp"
#include "gx/font/Types.hpp"
#include "gx/Types.hpp"
#include <cstdint>
#include <common/Handle.hpp>
#include <storm/Hash.hpp>

#define FONT_OUTLINE 0x1
#define FONT_MONOCHROME 0x2
#define FONT_THICKOUTLINE 0x4

class C2Vector;
class C3Vector;
class CGxFont;
class CGxShader;
class CGxString;
class CGxStringBatch;
class CImVector;
struct GLYPHBITMAPDATA;

class FONTHASHOBJ : public CHandleObject, public TSHashObject<FONTHASHOBJ, HASHKEY_STR> {
    public:
        CGxFont* font = nullptr;
        virtual ~FONTHASHOBJ();
};

class TEXTBLOCK : public CHandleObject {
    public:
        CGxString* string = nullptr;
        virtual ~TEXTBLOCK();
};

extern CGxShader* g_fontPixelShader[1];

extern CGxShader* g_fontVertexShader[2];

extern STORM_LIST(CGxString) g_freeStrings;

extern STORM_LIST(CGxString) g_strings;

void CalculateYOffset(uint32_t, uint32_t, FT_Face, uint32_t, int32_t*, int32_t*);

float GetCharacterWidth(const char*, uint32_t, uint32_t, CGxFont*, float);

float GetIndentNormWidth();

float GetIndentPixelWidth(void);

uint32_t GetScreenPixelHeight(void);

uint32_t GetScreenPixelWidth(void);

QUOTEDCODE GxuDetermineQuotedCode(const char*, int32_t&, CImVector*, uint32_t, uint32_t&);

int32_t GxuFontAddToBatch(CGxStringBatch*, CGxString*);

void GxuFontAddShadow(CGxString* string, const CImVector& color, const C2Vector& offset);

CGxStringBatch* GxuFontCreateBatch(bool, bool);

int32_t GxuFontCreateFont(const char*, float, CGxFont*&, uint32_t);

int32_t GxuFontCreateString(CGxFont*, const char*, float, const C3Vector&, float, float, float, CGxString*&, EGxFontVJusts, EGxFontHJusts, uint32_t, const CImVector&, float, float);

int32_t GxuFontDestroyBatch(CGxStringBatch*);

int32_t GxuFontClearBatch(CGxStringBatch* batch);

void GxuFontDestroyFont(CGxFont*& font);

void GxuFontDestroyString(CGxString*&);

uint32_t GxuFontGetFontFlags(CGxFont*);

const char* GxuFontGetFontName(CGxFont*);

uint32_t GxuFontGetMaxCharsWithinWidth(CGxFont*, const char*, float, float, uint32_t, float*, float, float, float, uint32_t);

uint32_t GxuFontGetMaxCharsWithinWidthAndHeight(CGxFont*, const char*, float, float, float, uint32_t, float, float, float, uint32_t);

float GxuFontGetOneToOneHeight(CGxFont*);

void GxuFontGetTextExtent(CGxFont*, const char*, uint32_t, float, float*, float, float, float, uint32_t);

float GxuFontGetWrappedTextHeight(CGxFont*, const char*, float, float, const C2Vector&, float, float, uint32_t);

void GxuFontInitialize(void);

void GxuFontRenderBatch(CGxStringBatch*);

int32_t GxuFontSetStringColor(CGxString*, const CImVector&);

void GxuFontSetStringPosition(CGxString* string, const C3Vector& position);

void GxuFontUpdate();

void GxuFontWindowSizeChanged(void);

int32_t IGxuFontGlyphRenderGlyph(FT_Face, uint32_t, uint32_t, uint32_t, GLYPHBITMAPDATA*, int32_t, uint32_t);

void TextBlockAddShadow(HTEXTBLOCK, CImVector, const C2Vector&);

HTEXTBLOCK TextBlockCreate(HTEXTFONT, const char*, const CImVector&, const C3Vector&, float, float, float, uint32_t, float, float, float);

HTEXTFONT TextBlockGenerateFont(const char*, uint32_t, float);

uint32_t TextBlockGetFontFlags(HTEXTFONT);

const char* TextBlockGetFontName(HTEXTFONT);

CGxFont* TextBlockGetFontPtr(HTEXTFONT);

uint32_t TextBlockGetMaxCharsWithinWidth(HTEXTFONT, const char*, float, float, uint32_t, float*, float, float, float, uint32_t);

uint32_t TextBlockGetMaxCharsWithinWidthAndHeight(HTEXTFONT, const char*, float, float, float, uint32_t, float, float, float, uint32_t);

CGxString* TextBlockGetStringPtr(HTEXTBLOCK);

void TextBlockGetTextExtent(HTEXTFONT, const char*, uint32_t, float, float*, float, float, float, uint32_t);

float TextBlockGetWrappedTextHeight(HTEXTFONT, const char*, float, float, const C2Vector&, float, float, uint32_t);

void TextBlockSetStringPos(HTEXTBLOCK stringHandle, const C3Vector& pos);

void TextBlockUpdateColor(HTEXTBLOCK, const CImVector&);

float PixelToScreenHeight(int32_t);

float PixelToScreenWidth(int32_t);

float PixelToScreenWidth(float);

float ScreenToPixelHeight(int32_t, float);

float ScreenToPixelWidth(int32_t, float);

float Sub6C2280(FT_Face, float);

#endif
