#ifndef GX_FONT_HPP
#define GX_FONT_HPP

#include "gx/font/FreeType.hpp"
#include "gx/font/GxuFont.hpp"
#include "gx/font/TextBlock.hpp"
#include <cstdint>

class C2Vector;
class C3Vector;
class CGxFont;

extern uint32_t g_heightPixels;
extern uint32_t g_widthPixels;
extern float g_indentPixelWidth;
extern float g_indentNormWidth;

float GetCharacterWidth(const char*, uint32_t, uint32_t, CGxFont*, float);

float GetIndentNormWidth();

float GetIndentPixelWidth();

uint32_t GetScreenPixelHeight();

uint32_t GetScreenPixelWidth();

float PixelToScreenHeight(int32_t);

float PixelToScreenWidth(int32_t);

float PixelToScreenWidth(float);

float ScreenToPixelHeight(int32_t, float);

float ScreenToPixelWidth(int32_t, float);

float Sub6C2280(FT_Face, float);

#endif
