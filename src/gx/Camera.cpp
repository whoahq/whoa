#include "gx/Camera.hpp"
#include "gx/CCamera.hpp"
#include "gx/Device.hpp"
#include "gx/Transform.hpp"
#include <common/Handle.hpp>
#include <storm/Error.hpp>

HCAMERA CameraCreate() {
    void* m = SMemAlloc(sizeof(CCamera), __FILE__, __LINE__, 0x0);
    auto camera = new (m) CCamera();
    return HandleCreate(camera);
}

void CameraSetupScreenProjection(const CRect& projectionRect, const C2Vector& screenPoint, float depth, int32_t a4) {
    float offsetX = (projectionRect.minX + projectionRect.maxX) * 0.5f;
    float offsetY = (projectionRect.minY + projectionRect.maxY) * 0.5f;

    CRect frustumRect = {
        projectionRect.minY - offsetY,
        projectionRect.minX - offsetX,
        projectionRect.maxY - offsetY,
        projectionRect.maxX - offsetX
    };

    float minZ = -500.0f;
    float maxZ = 500.0f;

    C44Matrix proj;

    if (a4 || !g_theGxDevicePtr->StereoEnabled()) {
        GxuXformCreateOrtho(
            frustumRect.minX,
            frustumRect.maxX,
            frustumRect.minY,
            frustumRect.maxY,
            minZ,
            maxZ,
            proj
        );

        C3Vector v50 = { 1.0f, 1.0f, -1.0f };
        proj.Scale(v50);
    } else {
        GxuXformCreateOrthoDepth(
            frustumRect.minX,
            frustumRect.maxX,
            frustumRect.minY,
            frustumRect.maxY,
            minZ,
            maxZ,
            proj
        );
    }

    // TODO some caps stuff

    GxXformSetProjection(proj);

    C44Matrix view;
    C3Vector move = { screenPoint.x - offsetX, screenPoint.y - offsetY, 0.0f };
    view.Translate(move);

    GxXformSetView(view);
}

void CameraSetupWorldProjection(HCAMERA camera, const CRect& projectionRect, uint32_t flags) {
    auto cameraPtr = reinterpret_cast<CCamera*>(camera);
    STORM_ASSERT(cameraPtr);
    cameraPtr->SetupWorldProjection(projectionRect, flags);
}
