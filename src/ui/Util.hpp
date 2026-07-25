#ifndef UI_UTIL_HPP
#define UI_UTIL_HPP

#include "gx/Types.hpp"
#include "ui/Types.hpp"
#include <cstdint>

struct lua_State;

const char* LanguageProcess(const char* string);

int32_t StringToBlendMode(const char* string, EGxBlend& blend);

int32_t StringToBOOL(const char* string);

bool StringToBOOL(const char* string, int32_t def);

bool StringToBOOL(lua_State* L, int32_t idx, int32_t def);

uint64_t StringToClickAction(const char* string);

int32_t StringToDrawLayer(const char* string, int32_t& layer);

int32_t StringToFramePoint(const char* string, FRAMEPOINT& point);

int32_t StringToFrameStrata(const char* string, FRAME_STRATA& strata);

int32_t StringToJustify(const char* string, uint32_t& justify);

int32_t StringToOrientation(const char* string, ORIENTATION& orientation);

#endif
