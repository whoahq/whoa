#include "ui/simple/CSimpleRender.hpp"
#include "gx/Buffer.hpp"
#include "gx/Device.hpp"
#include "gx/Draw.hpp"
#include "gx/Font.hpp"
#include "gx/Gx.hpp"
#include "gx/RenderState.hpp"
#include "gx/Shader.hpp"
#include "gx/Texture.hpp"
#include "gx/Transform.hpp"
#include "gx/Types.hpp"
#include "ui/CRenderBatch.hpp"

CGxShader* CSimpleRender::s_vertexShader[2];

void CSimpleRender::DrawBatch(CRenderBatch* batch) {
    if (batch->m_texturelist.Count()) {
        GxRsPush();

        CGxShader* vs = g_theGxDevicePtr->StereoEnabled()
            ? CSimpleRender::s_vertexShader[1]
            : CSimpleRender::s_vertexShader[0];

        if (vs && vs->Valid()) {
            GxRsSet(GxRs_VertexShader, vs);

            C44Matrix viewProjMat;
            GxXformViewProjNativeTranspose(viewProjMat);
            GxShaderConstantsSet(GxSh_Vertex, 0, reinterpret_cast<float*>(&viewProjMat), 4);
        } else {
            GxRsSet(GxRs_Lighting, 0);
        }

        GxRsSet(GxRs_Fog, 0);
        GxRsSet(GxRs_DepthTest, 0);
        GxRsSet(GxRs_DepthWrite, 0);

        CGxTex* textureID = nullptr;
        EGxBlend alphaMode = GxBlends_Last;
        CGxShader* ps = nullptr;

        uint32_t idxCount = 0;
        uint32_t posCount = 0;

        for (int32_t i = 0; i < batch->m_texturelist.Count(); i++) {
            auto mesh = &batch->m_texturelist[i];

            auto dirty = 0;

            if (mesh->textureID != textureID) {
                textureID = mesh->textureID;
                dirty = 1;
            }

            if (mesh->GetAlphaMode() != alphaMode) {
                alphaMode = mesh->GetAlphaMode();
                dirty = 1;
            }

            if (mesh->shader != ps) {
                ps = mesh->shader;
                dirty = 1;
            }

            CTexture* texture = mesh->texture;
            CGxTex* gxTex = TextureGetGxTex(texture, 1, nullptr);
            GxRsSet(GxRs_Texture0, gxTex);

            GxRsSet(GxRs_BlendingMode, alphaMode);

            GxRsSet(GxRs_PixelShader, ps);

            // TODO
            // - alphaRef

            // TODO
            // - geometry merging (CSimpleRender::s_mergeEnable)

            posCount = mesh->posCount;
            idxCount = mesh->idxCount;

            CGxBuf* vertexStream = g_theGxDevicePtr->BufStream(GxPoolTarget_Vertex, 24, posCount);
            char* vertexData = g_theGxDevicePtr->BufLock(vertexStream);
            CGxVertexPCT* vertexBuf = reinterpret_cast<CGxVertexPCT*>(vertexData);

            CGxBuf* indexStream = g_theGxDevicePtr->BufStream(GxPoolTarget_Index, 2, idxCount);
            char* indexData = g_theGxDevicePtr->BufLock(indexStream);
            uint16_t* indexBuf = reinterpret_cast<uint16_t*>(indexData);

            if (mesh->indices) {
                for (int32_t i = 0; i < mesh->posCount; i++) {
                    C3Vector* p = &mesh->position[i];
                    vertexBuf->p.x = p->x;
                    vertexBuf->p.y = p->y;
                    vertexBuf->p.z = p->z;

                    if (mesh->color) {
                        auto& color = mesh->color[i * mesh->colorCount >> 2];

                        if (GxCaps().m_colorFormat == GxCF_rgba) {
                            vertexBuf->c.r = color.b;
                            vertexBuf->c.g = color.g;
                            vertexBuf->c.b = color.r;
                            vertexBuf->c.a = color.a;
                        } else {
                            vertexBuf->c = color;
                        }
                    } else {
                        vertexBuf->c.r = 0xFF;
                        vertexBuf->c.g = 0xFF;
                        vertexBuf->c.b = 0xFF;
                        vertexBuf->c.a = 0xFF;
                    }

                    if (mesh->onAtlas) {
                        // TODO
                    } else {
                        C2Vector* tc = &mesh->texCoord[i];
                        vertexBuf->tc[0].x = tc->x;
                        vertexBuf->tc[0].y = tc->y;
                    }

                    vertexBuf++;
                }

                for (int32_t i = 0; i < mesh->idxCount; i++) {
                    *indexBuf = mesh->indices[i];
                    indexBuf++;
                }
            }

            GxBufUnlock(vertexStream, 24 * posCount);
            GxBufUnlock(indexStream, 2 * idxCount);

            GxPrimVertexPtr(vertexStream, GxVBF_PCT);
            GxPrimIndexPtr(indexStream);

            CGxBatch batch;
            batch.m_primType = GxPrim_Triangles;
            batch.m_start = 0;
            batch.m_count = 6;
            batch.m_minIndex = 0;
            batch.m_maxIndex = 3;

            GxDraw(&batch, 1);
        }

        GxRsPop();
    }

    if (batch->m_stringbatch) {
        GxuFontRenderBatch(batch->m_stringbatch);
    }

    for (auto callback = batch->m_callbacks.Head(); callback; callback = batch->m_callbacks.Link(callback)->Next()) {
        callback->callback(callback->param);
    }
}

void CSimpleRender::Init() {
    g_theGxDevicePtr->ShaderCreate(
        CSimpleRender::s_vertexShader,
        GxSh_Vertex,
        "Shaders\\Vertex",
        "UI",
        2
    );
}
