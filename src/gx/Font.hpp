#ifndef GX_FONT_HPP
#define GX_FONT_HPP

#include "gx/font/FreeType.hpp"
#include "gx/font/GxuFont.hpp"
#include "gx/font/Types.hpp"
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
extern uint32_t g_heightPixels;
extern uint32_t g_widthPixels;
extern float g_indentPixelWidth;
extern float g_indentNormWidth;

float GetCharacterWidth(const char*, uint32_t, uint32_t, CGxFont*, float);

float GetIndentNormWidth();

float GetIndentPixelWidth(void);

uint32_t GetScreenPixelHeight(void);

uint32_t GetScreenPixelWidth(void);

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
