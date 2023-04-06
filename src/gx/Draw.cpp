#include "gx/Draw.hpp"
#include "gx/Device.hpp"

void GxDraw(CGxBatch* batch, int32_t indexed) {
    g_theGxDevicePtr->Draw(batch, indexed);
}

void GxSceneClear(uint32_t mask, CImVector color) {
    g_theGxDevicePtr->SceneClear(mask, color);
}

void GxScenePresent(uint32_t a2) {
    g_theGxDevicePtr->ScenePresent();
}

void GxSub682A00() {
    C3Vector v2 = { 0.0f, 0.0f, 0.0f };
    GxuFlushDrawList(GxuCat_2, v2);

    GxScenePresent(0);
}

void GxuFlushDrawList(EGxuDrawListCategory a1, const C3Vector& a2) {
    // TODO
}
