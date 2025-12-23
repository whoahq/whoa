#ifndef UI_UTIL_HPP
#define UI_UTIL_HPP

#include "gx/Types.hpp"
#include "ui/Types.hpp"
#include <cstdint>

const char* LanguageProcess(const char* string);

int32_t StringToBlendMode(const char* string, EGxBlend& blend);

int32_t StringToFramePoint(const char* string, FRAMEPOINT& point);

int32_t StringToFrameStrata(const char* string, FRAME_STRATA& strata);

int32_t StringToOrientation(const char* string, ORIENTATION& orientation);

#endif
