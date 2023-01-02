#ifndef GX_FONT_FONT_FACE_HPP
#define GX_FONT_FONT_FACE_HPP

#include "gx/font/FreeType.hpp"
#include "gx/font/Types.hpp"

void FontFaceCloseHandle(HFACE);

FT_Face FontFaceGetFace(HFACE);

const char* FontFaceGetFontName(HFACE);

HFACE FontFaceGetHandle(const char*, FT_Library);

#endif
