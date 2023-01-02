#ifndef GX_FONT_FREE_TYPE_HPP
#define GX_FONT_FREE_TYPE_HPP

#include "freetype/freetype.h"
#include <cstdint>

void FreeTypeInitialize();

int32_t FREETYPE_RenderGlyph(uint32_t, bool, FT_Face);

FT_Library GetFreeTypeLibrary();

#endif
