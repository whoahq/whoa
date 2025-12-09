#ifndef GX_FONT_GXU_FONT_INTERNAL_HPP
#define GX_FONT_GXU_FONT_INTERNAL_HPP

#include "gx/font/FreeType.hpp"
#include <cstdint>

class CGxFont;
struct GLYPHBITMAPDATA;

void CalculateYOffset(uint32_t, uint32_t, FT_Face, uint32_t, int32_t*, int32_t*);

int32_t IGxuFontGlyphRenderGlyph(FT_Face face, uint32_t pixelHeight, uint32_t code, uint32_t baseline, GLYPHBITMAPDATA* dataPtr, int32_t monochrome, uint32_t a7);

uint32_t InternalGetMaxCharsWithinWidth(CGxFont* face, const char* text, float height, float maxWidth, uint32_t lineBytes, float* extent, uint32_t flags, float a8, float scale, uint32_t* bytesInString, float* widthArray, float* widthArrayGuard);

void InternalGetTextExtent(CGxFont* font, const char* text, uint32_t numBytes, float height, float* extent, uint32_t flags, float a7, float a8);

#endif
