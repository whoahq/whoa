#ifndef UI_UTIL_HPP
#define UI_UTIL_HPP

#include "gx/Types.hpp"
#include "ui/Types.hpp"
#include <cstdint>

const char* LanguageProcess(const char*);

int32_t StringToBlendMode(const char*, EGxBlend&);

int32_t StringToFramePoint(const char*, FRAMEPOINT&);

int32_t StringToFrameStrata(const char*, FRAME_STRATA&);

#endif
