#include "gx/font/CGxStringBatch.hpp"
#include "gx/font/CGxFont.hpp"
#include "gx/Buffer.hpp"
#include "gx/CGxBatch.hpp"
#include "gx/Device.hpp"
#include "gx/Draw.hpp"
#include "gx/Font.hpp"
#include "gx/Gx.hpp"
#include "gx/RenderState.hpp"
#include "gx/Shader.hpp"
#include "gx/Texture.hpp"
#include "gx/Transform.hpp"
#include <cmath>
#include <storm/Error.hpp>

bool BATCHEDRENDERFONTDESC::s_billboarded;
CGxBuf* BATCHEDRENDERFONTDESC::s_indexBuf;
CGxPool* BATCHEDRENDERFONTDESC::s_indexPool;

int32_t SetProjection() {
    float minX, maxX, minY, maxY, minZ, maxZ;
    GxXformViewport(minX, maxX, minY, maxY, minZ, maxZ);

    float v14 = 0.0f;
    float v15 = 0.0f;

    if (!GxCaps().m_pixelCenterOnEdge) {
        v14 = -0.5f;
        v15 = 0.5f;
    }

    C44Matrix proj;

    uint32_t pixelWidth = GetScreenPixelWidth();
    float pixelMinX = floor(minX * pixelWidth);
    float pixelMaxX = floor(maxX * pixelWidth);

    if (pixelMinX >= pixelMaxX) {
        return 0;
    }

    uint32_t pixelHeight = GetScreenPixelHeight();
    float pixelMinY = floor(minY * pixelHeight);
    float pixelMaxY = floor(maxY * pixelHeight);

    if (pixelMinY >= pixelMaxY) {
        return 0;
    }

    if (pixelHeight > pixelMaxY) {
        // TODO
    }

    pixelMinX += v14;
    pixelMaxX += v14;
    pixelMinY += v15;
    pixelMaxY += v15;

    GxuXformCreateOrthoDepth(pixelMinX, pixelMaxX, pixelMinY, pixelMaxY, -5000.0f, 5000.0f, proj);
    GxXformSetProjection(proj);

    return 1;
}

void BATCHEDRENDERFONTDESC::Initialize() {
    CGxPool* indexPool = GxPoolCreate(
        GxPoolTarget_Index,
        GxPoolUsage_Static,
        6144,
        GxPoolHintBit_Unk0,
        "BATCHEDRENDERFONTDESC_idx"
    );

    CGxBuf* indexBuf = GxBufCreate(
        indexPool,
        2,
        3072,
        0
    );

    BATCHEDRENDERFONTDESC::s_indexPool = indexPool;
    BATCHEDRENDERFONTDESC::s_indexBuf = indexBuf;
}

void BATCHEDRENDERFONTDESC::InitializeIndexBuff() {
    char* indexData = g_theGxDevicePtr->BufLock(BATCHEDRENDERFONTDESC::s_indexBuf);
    uint16_t* indexBuf = reinterpret_cast<uint16_t*>(indexData);

    uint16_t index = 0;

    for (int32_t i = 0; i < 512; i++) {
        indexBuf[i * 6 + 0] = index + 0;
        indexBuf[i * 6 + 1] = index + 2;
        indexBuf[i * 6 + 2] = index + 1;
        indexBuf[i * 6 + 3] = index + 2;
        indexBuf[i * 6 + 4] = index + 3;
        indexBuf[i * 6 + 5] = index + 1;

        index += 4;
    }

    GxBufUnlock(BATCHEDRENDERFONTDESC::s_indexBuf, 0);
};

CGxVertexPCT* BATCHEDRENDERFONTDESC::UnlockVertexPtrAndRender(CGxBuf*& buf, int32_t count) {
    GxBufUnlock(buf, sizeof(CGxVertexPCT) * count);

    if (!BATCHEDRENDERFONTDESC::s_indexBuf->unk1C || !BATCHEDRENDERFONTDESC::s_indexBuf->unk1D) {
        BATCHEDRENDERFONTDESC::InitializeIndexBuff();
    }

    if (BATCHEDRENDERFONTDESC::s_indexBuf->unk1C && BATCHEDRENDERFONTDESC::s_indexBuf->unk1D) {
        GxPrimVertexPtr(buf, GxVBF_PCT);
        GxPrimIndexPtr(BATCHEDRENDERFONTDESC::s_indexBuf);

        CGxBatch batch;
        batch.m_primType = GxPrim_Triangles;
        batch.m_start = 0;
        batch.m_count = 6 * (count / 4);
        batch.m_minIndex = 0;
        batch.m_maxIndex = count - 1;

        GxDraw(&batch, 1);
    }

    return reinterpret_cast<CGxVertexPCT*>(g_theGxDevicePtr->BufLock(buf));
}

void BATCHEDRENDERFONTDESC::RenderBatch() {
    if (!BATCHEDRENDERFONTDESC::s_indexPool) {
        return;
    }

    for (auto string = this->m_strings.Head(); string; string = this->m_strings.Next(string)) {
        string->CheckGeometry();
    }

    for (int32_t i = 0; i < 8; i++) {
        this->m_face->m_textureCache[i].UpdateDirty();
    }

    int32_t maxBatchCapacity = 2048;
    int32_t batchCapacity = maxBatchCapacity;

    CGxBuf* vertexStream = g_theGxDevicePtr->BufStream(GxPoolTarget_Vertex, sizeof(CGxVertexPCT), maxBatchCapacity);
    char* vertexData = g_theGxDevicePtr->BufLock(vertexStream);
    CGxVertexPCT* vertexBuf = reinterpret_cast<CGxVertexPCT*>(vertexData);

    for (int32_t lineIndex = 0; lineIndex < 8; lineIndex++) {
        auto& textureCache = this->m_face->m_textureCache[lineIndex];
        auto texture = textureCache.m_texture;

        if (!texture) {
            continue;
        }

        auto gxTex = TextureGetGxTex(reinterpret_cast<CTexture*>(texture), 1, nullptr);

        if (!gxTex) {
            continue;
        }

        GxRsSet(GxRs_Texture0, gxTex);

        for (auto string = this->m_strings.Head(); string; string = this->m_strings.Next(string)) {
            auto line = string->m_textLines[lineIndex];

            if (!line) {
                continue;
            }

            int32_t vertsNeeded = string->CalculateVertsNeeded(lineIndex);
            int32_t sliceOffset = 0;

            while (vertsNeeded) {
                // Clamp slice vertex count to remaining batch capacity
                int32_t sliceCount = std::min(vertsNeeded, batchCapacity);

                // Write string vertices within slice to buffer
                string->WriteGeometry(vertexBuf, lineIndex, sliceOffset, sliceCount);

                vertsNeeded -= sliceCount;
                batchCapacity -= sliceCount;
                sliceOffset += sliceCount;
                vertexBuf += sliceCount;

                // Render full buffer and reset capacity
                if (batchCapacity == 0) {
                    vertexBuf = this->UnlockVertexPtrAndRender(vertexStream, maxBatchCapacity);
                    batchCapacity = maxBatchCapacity;
                }
            }
        }

        // Render used portion of buffer and reset capacity
        if (batchCapacity != maxBatchCapacity) {
            vertexBuf = this->UnlockVertexPtrAndRender(vertexStream, maxBatchCapacity - batchCapacity);
            batchCapacity = maxBatchCapacity;
        }
    }

    GxBufUnlock(vertexStream, 0);
}

CGxStringBatch::~CGxStringBatch() {
    this->m_fontBatch.Clear();
}

void CGxStringBatch::AddString(CGxString* string) {
    STORM_ASSERT(string);
    STORM_ASSERT(string->m_currentFace);

    auto face = string->m_currentFace;
    uint32_t hashval = reinterpret_cast<uintptr_t>(face);
    HASHKEY_PTR key = { face };

    auto batch = this->m_fontBatch.Ptr(hashval, key);

    if (!batch) {
        batch = this->m_fontBatch.New(hashval, key, 0, 0);
        batch->m_face = face;
    }

    batch->m_strings.LinkToTail(string);
}

void CGxStringBatch::RenderBatch() {
    // TODO
    // if (!g_perf) {
    //     return;
    // }

    C44Matrix oldProjection;
    C44Matrix oldView;

    GxXformProjection(oldProjection);
    GxXformView(oldView);

    GxRsPush();

    GxRsSet(GxRs_Fog, 0);
    GxRsSet(GxRs_Culling, 0);
    GxRsSet(GxRs_BlendingMode, GxBlend_Alpha);
    GxRsSetAlphaRef();

    int32_t setProjection;
    int32_t stereoEnabled;

    if (this->m_flags & 0x1) {
        BATCHEDRENDERFONTDESC::s_billboarded = true;

        GxRsSet(GxRs_DepthTest, 1);
        GxRsSet(GxRs_DepthWrite, 1);
        GxRsSet(GxRs_AlphaRef, 1);

        setProjection = 1;
        stereoEnabled = 0;
    } else {
        C44Matrix view;
        GxXformSetView(view);

        GxRsSet(GxRs_DepthTest, 0);
        GxRsSet(GxRs_DepthWrite, 0);

        setProjection = SetProjection();
        stereoEnabled = g_theGxDevicePtr->StereoEnabled();
    }

    CGxShader* vs = g_fontVertexShader[stereoEnabled ? 1 : 0];
    CGxShader* ps = g_fontPixelShader[0];

    if (setProjection && vs->Valid() && ps->Valid()) {
        GxRsSet(GxRs_VertexShader, vs);
        GxRsSet(GxRs_PixelShader, ps);

        C44Matrix viewProjMat;
        GxXformViewProjNativeTranspose(viewProjMat);
        GxShaderConstantsSet(GxSh_Vertex, 0, reinterpret_cast<float*>(&viewProjMat), 4);

        for (auto fontBatch = this->m_fontBatch.Head(); fontBatch; fontBatch = this->m_fontBatch.Next(fontBatch)) {
            if (fontBatch->m_strings.Head()) {
                fontBatch->RenderBatch();

                if (this->m_flags & 0x2) {
                    // TODO
                }
            } else {
                this->m_fontBatch.Unlink(fontBatch);
            }
        }
    }

    BATCHEDRENDERFONTDESC::s_billboarded = false;

    GxRsPop();

    GxXformSetView(oldView);
    GxXformSetProjection(oldProjection);
}
