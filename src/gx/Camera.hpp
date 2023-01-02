#ifndef GX_CAMERA_HPP
#define GX_CAMERA_HPP

#include <cstdint>
#include <common/DataMgr.hpp>

typedef HDATAMGR HCAMERA;

class CRect;
class C2Vector;

HCAMERA CameraCreate(void);

void CameraSetupScreenProjection(const CRect&, const C2Vector&, float, int32_t);

void CameraSetupWorldProjection(HCAMERA camera, const CRect& projectionRect, uint32_t flags);

#endif
