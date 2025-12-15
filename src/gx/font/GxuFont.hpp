#ifndef GX_FONT_GXU_FONT_HPP
#define GX_FONT_GXU_FONT_HPP

#include "gx/Types.hpp"
#include "gx/font/Types.hpp"
#include <storm/List.hpp>
#include <tempest/Vector.hpp>
#include <cstdint>

class CGxFont;
class CGxShader;
class CGxString;
class CGxStringBatch;

extern CGxShader* g_fontPixelShader[1];
extern CGxShader* g_fontVertexShader[2];
extern STORM_LIST(CGxFont) g_fonts;
extern STORM_LIST(CGxString) g_freeStrings;
extern STORM_LIST(CGxString) g_strings;

#define FLAG_IGNORE_COLORS      0x100
#define FLAG_IGNORE_NEWLINES    0x200
#define FLAG_IGNORE_HYPERLINKS  0x400
#define FLAG_IGNORE_PIPES       0x800
#define FLAG_IGNORE_TEXTURES    0x1000

QUOTEDCODE GxuDetermineQuotedCode(const char* text, int32_t& advance, CImVector* color, uint32_t flags, uint32_t& wide);

void GxuFontAddShadow(CGxString* string, const CImVector& color, const C2Vector& offset);

int32_t GxuFontAddToBatch(CGxStringBatch*, CGxString*);

CGxStringBatch* GxuFontCreateBatch(bool, bool);

int32_t GxuFontCreateFont(const char*, float, CGxFont*&, uint32_t);

int32_t GxuFontCreateString(CGxFont*, const char*, float, const C3Vector&, float, float, float, CGxString*&, EGxFontVJusts, EGxFontHJusts, uint32_t, const CImVector&, float, float);

int32_t GxuFontDestroyBatch(CGxStringBatch*);

void GxuFontDestroyFont(CGxFont*& font);

void GxuFontDestroyString(CGxString*&);

uint32_t GxuFontGetFontFlags(CGxFont*);

const char* GxuFontGetFontName(CGxFont*);

uint32_t GxuFontGetMaxCharsWithinWidth(CGxFont*, const char*, float, float, uint32_t, float*, float, float, float, uint32_t);

uint32_t GxuFontGetMaxCharsWithinWidthAndHeight(CGxFont*, const char*, float, float, float, uint32_t, float, float, float, uint32_t);

float GxuFontGetOneToOneHeight(CGxFont*);

void GxuFontGetTextExtent(CGxFont* font, const char* text, uint32_t numBytes, float height, float* extent, float a6, float scale, float a8, uint32_t flags);

float GxuFontGetWrappedTextHeight(CGxFont*, const char*, float, float, const C2Vector&, float, float, uint32_t);

void GxuFontInitialize();

void GxuFontRenderBatch(CGxStringBatch*);

int32_t GxuFontSetStringColor(CGxString*, const CImVector&);

void GxuFontSetStringPosition(CGxString* string, const C3Vector& position);

void GxuFontUpdate();

void GxuFontWindowSizeChanged();

#endif
