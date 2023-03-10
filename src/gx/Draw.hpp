#ifndef GX_DRAW_HPP
#define GX_DRAW_HPP

#include "gx/CGxBatch.hpp"
#include "gx/Types.hpp"
#include <cstdint>

class C3Vector;
class CImVector;

void GxDraw(CGxBatch* batch, int32_t indexed);

void GxSceneClear(uint32_t, CImVector);

void GxScenePresent(void);

void GxuFlushDrawList(EGxuDrawListCategory, const C3Vector&);

#endif
