#ifndef GLUE_CHARACTER_HPP
#define GLUE_CHARACTER_HPP

#include "util/Locale.hpp"
#include <cstdint>

bool NameNeedsDeclension(WOW_LOCALE locale, const char* name);

void ValidateNameInitialize(int32_t localeMask, int32_t charsetMask);

#endif
