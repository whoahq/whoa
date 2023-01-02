#ifndef GX_BLIT_HPP
#define GX_BLIT_HPP

#include "gx/Types.hpp"
#include <cstdint>

class C2iVector;

typedef void (*BLIT_FUNCTION)(const C2iVector&, const void*, uint32_t, void*, uint32_t);

void Blit(const C2iVector&, BlitAlpha, const void*, uint32_t, BlitFormat, void*, uint32_t, BlitFormat);

BlitFormat GxGetBlitFormat(EGxTexFormat);

#endif
