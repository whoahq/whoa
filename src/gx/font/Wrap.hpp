#ifndef GX_FONT_WRAP_HPP
#define GX_FONT_WRAP_HPP

#include <cstdint>

class CGxFont;

void CalcWrapPoint(CGxFont*, const char*, float, float, uint32_t*, float*, const char**, float, uint32_t, bool*, float*, float);

void CalcWrapPointBillboarded(const char*, uint32_t, CGxFont*, float, uint32_t*, float*, const char**, float*, float);

void CalcWrapPointNonBillboarded(const char*, CGxFont*, float, float, uint32_t*, float*, const char**, float, uint32_t, bool*, float*, float);

int32_t CanWrapBetween(uint32_t codeA, uint32_t codeB);

#endif
