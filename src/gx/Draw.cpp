#include "gx/Buffer.hpp"
#include "gx/Draw.hpp"
#include "gx/Device.hpp"
#include <bc/Debug.hpp>

void GxDraw(CGxBatch* batch, int32_t indexed) {
    g_theGxDevicePtr->Draw(batch, indexed);
}

void GxDrawLockedElements(EGxPrim primType, uint32_t indexCount, const uint16_t* indices) {
    if (Buffer::s_lockVertexCount == 0) {
        return;
    }

    GxPrimIndexPtr(indexCount, indices);

    CGxBatch batch;
    batch.m_primType = primType;
    batch.m_minIndex = 0;
    batch.m_maxIndex = Buffer::s_lockVertexCount - 1;
    batch.m_start = 0;
    batch.m_count = indexCount;

    BLIZZARD_ASSERT(batch.m_count > 0);

    g_theGxDevicePtr->Draw(&batch, 1);
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
