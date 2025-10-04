#ifndef UTIL_LOCALE_HPP
#define UTIL_LOCALE_HPP

#include <cstdint>

enum WOW_LOCALE {
    LOCALE_en_US     = 0,
    LOCALE_ko_KR     = 1,
    LOCALE_fr_FR     = 2,
    LOCALE_de_DE     = 3,
    LOCALE_zh_CN     = 4,
    LOCALE_zh_TW     = 5,
    LOCALE_es_ES     = 6,
    LOCALE_es_MX     = 7,
    LOCALE_ru_RU     = 8,
    NUM_LOCALES      = 9,
    DEFAULT_LANGUAGE = 0,
};

extern WOW_LOCALE CURRENT_LANGUAGE;

extern int32_t CURRENT_REGION;

#endif
