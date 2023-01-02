#ifndef GX_RENDER_STATE_HPP
#define GX_RENDER_STATE_HPP

#include "gx/Types.hpp"
#include <cstdint>

class CGxShader;
class CGxTex;

void GxRsPop(void);

void GxRsPush(void);

void GxRsSet(EGxRenderState, int32_t);

void GxRsSet(EGxRenderState, CGxShader*);

void GxRsSet(EGxRenderState, CGxTex*);

void GxRsSetAlphaRef(void);

#endif
