#ifndef GX_FONT_TEXT_BLOCK_HPP
#define GX_FONT_TEXT_BLOCK_HPP

#include "gx/font/Types.hpp"
#include <common/Handle.hpp>
#include <storm/Hash.hpp>
#include <tempest/Vector.hpp>
#include <cstdint>

#define FONT_OUTLINE 0x1
#define FONT_MONOCHROME 0x2
#define FONT_THICKOUTLINE 0x4

class CGxFont;
class CGxString;

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

void TextBlockAddShadow(HTEXTBLOCK, CImVector, const C2Vector&);

HTEXTBLOCK TextBlockCreate(HTEXTFONT fontHandle, const char* text, const CImVector& color, const C3Vector& position, float fontHeight, float blockWidth, float blockHeight, uint32_t flags, float charSpacing, float lineSpacing, float scale);

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

#endif
