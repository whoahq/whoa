#include "model/CM2SceneRender.hpp"
#include "gx/Draw.hpp"
#include "gx/RenderState.hpp"
#include "gx/Shader.hpp"
#include "gx/Texture.hpp"
#include "gx/Transform.hpp"
#include "model/CM2Cache.hpp"
#include "model/CM2Model.hpp"
#include "model/CM2Shared.hpp"
#include "model/M2Types.hpp"
#include <tempest/Math.hpp>

C44Matrix CM2SceneRender::s_identity;

int32_t CM2SceneRender::s_fogModeList[M2BLEND_COUNT] = {
    1,  // M2BLEND_OPAQUE
    1,  // M2BLEND_ALPHA_KEY
    1,  // M2BLEND_ALPHA
    2,  // M2BLEND_NO_ALPHA_ADD
    2,  // M2BLEND_ADD
    3,  // M2BLEND_MOD
    4   // M2BLEND_MOD_2X
};

EGxBlend CM2SceneRender::s_gxBlend[M2PASS_COUNT][M2BLEND_COUNT] = {
    // M2PASS_0
    {
        GxBlend_Opaque,         // M2BLEND_OPAQUE
        GxBlend_AlphaKey,       // M2BLEND_ALPHA_KEY
        GxBlend_Alpha,          // M2BLEND_ALPHA
        GxBlend_NoAlphaAdd,     // M2BLEND_NO_ALPHA_ADD
        GxBlend_Add,            // M2BLEND_ADD
        GxBlend_Mod,            // M2BLEND_MOD
        GxBlend_Mod2x           // M2BLEND_MOD_2X
    },

    // M2PASS_1
    {
        GxBlend_Alpha,          // M2BLEND_OPAQUE
        GxBlend_Alpha,          // M2BLEND_ALPHA_KEY
        GxBlend_Alpha,          // M2BLEND_ALPHA
        GxBlend_NoAlphaAdd,     // M2BLEND_NO_ALPHA_ADD
        GxBlend_Add,            // M2BLEND_ADD
        GxBlend_Mod,            // M2BLEND_MOD
        GxBlend_Mod2x           // M2BLEND_MOD_2X
    },

    // M2PASS_2
    {
        GxBlend_Alpha,          // M2BLEND_OPAQUE
        GxBlend_Alpha,          // M2BLEND_ALPHA_KEY
        GxBlend_Alpha,          // M2BLEND_ALPHA
        GxBlend_NoAlphaAdd,     // M2BLEND_NO_ALPHA_ADD
        GxBlend_Add,            // M2BLEND_ADD
        GxBlend_Mod,            // M2BLEND_MOD
        GxBlend_Mod2x           // M2BLEND_MOD_2X
    }
};

int32_t CM2SceneRender::s_shadedList[M2BLEND_COUNT] = {
    1,  // M2BLEND_OPAQUE
    1,  // M2BLEND_ALPHA_KEY
    1,  // M2BLEND_ALPHA
    1,  // M2BLEND_NO_ALPHA_ADD
    1,  // M2BLEND_ADD
    0,  // M2BLEND_MOD
    0   // M2BLEND_MOD_2X
};

void CM2SceneRender::Draw(M2PASS pass, M2Element* elements, uint32_t* a4, uint32_t a5) {
    if (!a5) {
        return;
    }

    GxRsPush();

    C44Matrix savedView;
    GxXformView(savedView);

    for (int32_t xf = GxXform_Tex0; xf <= GxXform_World; xf++) {
        GxXformPush(static_cast<EGxXform>(xf));
    }

    GxXformSetView(CM2SceneRender::s_identity);
    GxXformSet(GxXform_World, CM2SceneRender::s_identity);

    GxRsSet(GxRs_DepthFunc, 0);
    GxRsSet(GxRs_PolygonOffset, 0);
    GxRsSet(GxRs_Lighting, 0);
    GxRsSet(GxRs_Fog, 0);
    GxRsSet(GxRs_MatSpecularExp, 0.0f);

    if (CShaderEffect::s_enableShaders) {
        C44Matrix projNative;
        GxXformProjNative(projNative);
        this->matrix0 = projNative.Inverse(projNative.Determinant()).Transpose();

        CShaderEffect::UpdateProjMatrix();

        // TODO
        // CShadowCache::SetShadowMapGenericGlobal();
    }

    this->m_curPass = pass;

    for (int32_t i = 0; i < a5; i++) {
        uint32_t index = a4[i];
        auto element = &elements[index];

        if (element->type == 2 || element->type == 4 || !element->model->m_flag2000) {
            this->m_curElement = element;
            this->m_curType = element->type;
            this->m_curModel = element->model;
            this->m_curShared = element->model->m_shared;
            this->m_curShaded = 1;
            this->m_curFogMode = 1;
            this->m_curBatch = nullptr;
            this->m_curSkinSection = nullptr;
            this->m_curLighting = element->model->m_currentLighting;
            // TODO
            // this->m_curMaterial = this->dwordB8;
            this->m_data = element->model->m_shared->m_data;

            // TODO
            // this->m_cache->LinkToSharedUpdateList(this->m_curShared);

            switch (this->m_curElement->type) {
                case 0: {
                    this->DrawBatch();
                    break;
                }

                case 1: {
                    this->DrawBatchProj();
                    break;
                }

                case 2: {
                    this->DrawBatchDoodad(elements, &a4[i]);
                    // TODO
                    // i += this->m_curElement->dword1C - 1;
                    break;
                }

                case 3: {
                    this->DrawRibbon();
                    break;
                }

                case 4: {
                    i += this->DrawParticle(i, elements, a4, a5);
                    break;
                }

                case 5: {
                    this->DrawCallback();
                    break;
                }

                default:
                    continue;
            }

            this->m_prevElement = this->m_curElement;
            this->m_prevType = this->m_curType;
            this->m_prevModel = this->m_curModel;
            this->m_prevShared = this->m_curShared;
            this->m_curLighting = this->m_prevLighting; // TODO investigate (maybe bug?)
            this->m_prevShaded = this->m_curShaded;
            this->m_prevFogMode = this->m_curFogMode;
            this->m_prevBatch = this->m_curBatch;
            this->m_prevSkinSection = this->m_curSkinSection;
            this->m_prevMaterial = this->m_curMaterial;
        }
    }

    for (int32_t xf = GxXform_Tex0; xf <= GxXform_World; xf++) {
        GxXformPop(static_cast<EGxXform>(xf));
    }

    GxXformSetView(savedView);

    GxRsPop();
    GxRsSet(GxRs_Fog, 0);
}

void CM2SceneRender::DrawBatch() {
    auto element = this->m_curElement;

    this->m_curBatch = element->batch;
    this->m_curSkinSection = element->skinSection;
    this->m_curMaterial = &this->m_data->materials[element->batch->materialIndex];

    element->effect->SetCurrent();
    this->SetupLighting();
    this->SetupMaterial();
    this->SetupTextures();

    if (
        CShaderEffect::s_enableShaders
        && (
            this->m_curType != this->m_prevType
            || this->m_curModel != this->m_prevModel
            || this->m_curSkinSection->boneComboIndex != this->m_prevSkinSection->boneComboIndex
        )
    ) {
        C4Vector* constants = reinterpret_cast<C4Vector*>(GxShaderConstantsLock(GxSh_Vertex));

        for (int32_t i = 0; i < this->m_curSkinSection->boneCount; i++) {
            auto& boneMatrix = this->m_curModel->m_boneMatrices[this->m_data->boneCombos[this->m_curSkinSection->boneComboIndex + i]];

            constants[31 + (i * 3) + 0] = { boneMatrix.a0, boneMatrix.b0, boneMatrix.c0, boneMatrix.d0 };
            constants[31 + (i * 3) + 1] = { boneMatrix.a1, boneMatrix.b1, boneMatrix.c1, boneMatrix.d1 };
            constants[31 + (i * 3) + 2] = { boneMatrix.a2, boneMatrix.b2, boneMatrix.c2, boneMatrix.d2 };
        }

        GxShaderConstantsUnlock(GxSh_Vertex, 31, this->m_curSkinSection->boneCount * 3);
    }

    if (this->m_curElement->flags & 0x4) {
        if (
            this->m_curType != this->m_prevType
            || this->m_curModel != this->m_prevModel
        ) {
            this->m_curModel->SetIndices();
        }
    } else if (
        this->m_curType != this->m_prevType
        || this->m_curShared != this->m_prevShared
        || this->m_prevElement->flags & 0x4
    ) {
        this->m_curShared->SetIndices();
    }

    int32_t v9 = this->m_curModel->m_shared->m_data->bones.count == 1 && this->m_cache->m_flags & 0x40;
    this->SetBatchVertices(v9);

    CShaderEffect::SetShaders(this->m_curElement->vertexPermute, this->m_curElement->pixelPermute);

    if (CShaderEffect::s_enableShaders) {
        auto skinSection = this->m_curSkinSection;

        CGxBatch batch;

        batch.m_primType = GxPrim_Triangles;
        batch.m_start = skinSection->indexStart;
        batch.m_count = skinSection->indexCount;
        batch.m_minIndex = skinSection->vertexStart;
        batch.m_maxIndex = skinSection->vertexStart + skinSection->vertexCount - 1;

        GxDraw(&batch, 1);
    } else if (v9) {
        // TODO
    } else {
        // TODO
    }
}

void CM2SceneRender::DrawBatchDoodad(M2Element* elements, uint32_t* a3) {
    // TODO
}

void CM2SceneRender::DrawBatchProj() {
    // TODO
}

void CM2SceneRender::DrawCallback() {
    // TODO
}

int32_t CM2SceneRender::DrawParticle(uint32_t a2, M2Element* elements, uint32_t* a4, uint32_t a5) {
    // TODO
    return 0;
}

void CM2SceneRender::DrawRibbon() {
    // TODO
}

void CM2SceneRender::SetBatchVertices(int32_t a2) {
    if (CShaderEffect::s_enableShaders) {
        if (this->m_curType != this->m_prevType || this->m_curShared != this->m_prevShared) {
            this->m_curShared->SetVertices(0);
        }
    } else {
        // TODO
        // - non-shader code path
    }
}

void CM2SceneRender::SetupLighting() {
    if (this->m_curMaterial->flags & 0x1) {
        this->m_curShaded = 0;
    } else {
        this->m_curShaded = CM2SceneRender::s_shadedList[this->m_curMaterial->blendMode];
    }

    CShaderEffect::SetLocalLighting(this->m_curLighting, this->m_curShaded, 0);

    // TODO
    // dwordD43010 = this->m_curElement->dword3C;

    if ((this->m_curMaterial->flags & 0x2) || this->m_curLighting->m_fogScale <= 0.0f) {
        this->m_curFogMode = 0;
    } else {
        this->m_curFogMode = CM2SceneRender::s_fogModeList[this->m_curMaterial->blendMode];
    }

    if (this->m_curFogMode == 0) {
        CShaderEffect::SetFogEnabled(0);
    } else {
        CImVector fogColor;

        switch (this->m_curFogMode) {
            case 1: {
                float x = this->m_curLighting->m_fogColor.x;
                float y = this->m_curLighting->m_fogColor.y;
                float z = this->m_curLighting->m_fogColor.z;

                fogColor.b = z <= 0.0f ? 0x00 : z >= 1.0f ? 0xFF : CMath::fuint_n(z * 255.0f);
                fogColor.g = y <= 0.0f ? 0x00 : y >= 1.0f ? 0xFF : CMath::fuint_n(y * 255.0f);
                fogColor.r = x <= 0.0f ? 0x00 : x >= 1.0f ? 0xFF : CMath::fuint_n(x * 255.0f);
                fogColor.a = 0xFF;

                break;
            }

            case 2: {
                fogColor = { 0x00, 0x00, 0x00, 0x00 };
                break;
            }

            case 3: {
                fogColor = { 0xFF, 0xFF, 0xFF, 0x00 };
                break;
            }

            case 4: {
                fogColor = { 0x80, 0x80, 0x80, 0x00 };
                break;
            }
        }

        CShaderEffect::SetFogParams(
            this->m_curLighting->m_fogStart,
            this->m_curLighting->m_fogEnd,
            this->m_curLighting->m_fogDensity,
            fogColor
        );

        CShaderEffect::SetFogEnabled(1);
    }

    if (
        this->m_curType != this->m_prevType
        || this->m_curModel != this->m_prevModel
        || this->m_curLighting != this->m_prevLighting
        || ((this->m_curElement->flags & 0x2) != (this->m_prevElement->flags & 0x2))
    ) {
        if (this->m_curElement->flags & 0x2) {
            // TODO
            // - enable clip plane mask for liquid plane
        } else  {
            GxRsSet(GxRs_ClipPlaneMask, 0);
        }
    }
}

void CM2SceneRender::SetupMaterial() {
    if (
        this->m_curType != this->m_prevType
        || this->m_curMaterial != this->m_prevMaterial
        || (this->m_curElement->flags & 0x1) != (this->m_prevElement->flags & 0x1)
        || this->m_curElement->alpha != this->m_prevElement->alpha
    ) {
        EGxBlend blendingMode = this->m_curElement->flags & 0x1
            ? GxBlend_AlphaKey
            : CM2SceneRender::s_gxBlend[this->m_curPass][this->m_curMaterial->blendMode];

        int32_t colorWrite = (this->m_curElement->flags & 0x1)
            ? 0
            : 15;

        GxRsSet(GxRs_ColorWrite, colorWrite);
        GxRsSet(GxRs_BlendingMode, blendingMode);

        float alphaRef;
        if (this->m_curMaterial->blendMode == 0) {
            alphaRef = 0.0f;
        } else if (this->m_curMaterial->blendMode == 1) {
            alphaRef = this->m_curElement->alpha * 0.87843138f;
        } else {
            alphaRef = 0.0039215689f;
        }

        CShaderEffect::SetAlphaRef(alphaRef);
    }

    if (
        this->m_curType != this->m_prevType
        || this->m_curMaterial != this->m_prevMaterial
    ) {
        int32_t culling = (this->m_curMaterial->flags & 0x4) == 0;
        GxRsSet(GxRs_Culling, culling);

        int32_t depthTest = (this->m_curMaterial->flags & 0x8) == 0;
        GxRsSet(GxRs_DepthTest, depthTest);

        int32_t depthWrite = (this->m_curMaterial->flags & 0x10) == 0;
        GxRsSet(GxRs_DepthWrite, depthWrite);
    }

    if (!CShaderEffect::s_enableShaders) {
        // TODO
    }

    if (this->m_curElement->type > 2) {
        if (
            this->m_curType != this->m_prevType
            || this->m_curElement->alpha != this->m_prevElement->alpha
        ) {
            C4Vector diffuse = { 1.0f, 1.0f, 1.0f, this->m_curElement->alpha };
            CShaderEffect::SetDiffuse(diffuse);

            C4Vector emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
            CShaderEffect::SetEmissive(emissive);
        }
    } else if (
        this->m_curType != this->m_prevType
        || this->m_curShared != this->m_prevShared
        || this->m_curShaded != this->m_prevShaded
        || this->m_curMaterial->blendMode != this->m_prevMaterial->blendMode
        || this->m_prevBatch == nullptr
        || this->m_curBatch->colorIndex != this->m_prevBatch->colorIndex
        || this->m_prevElement->alpha != this->m_curElement->alpha
        || this->m_curModel->m_currentDiffuse != this->m_prevModel->m_currentDiffuse
        || this->m_curModel->m_currentEmissive != this->m_prevModel->m_currentEmissive
    ) {
        if (this->m_curMaterial->blendMode == M2BLEND_MOD) {
            C4Vector diffuse = { 0.0f, 0.0f, 0.0f, this->m_curElement->alpha };
            CShaderEffect::SetDiffuse(diffuse);

            C4Vector emissive = { 1.0f, 1.0f, 1.0f, 0.0f };
            CShaderEffect::SetEmissive(emissive);
        } else if (this->m_curMaterial->blendMode == M2BLEND_MOD_2X) {
            C4Vector diffuse = { 0.0f, 0.0f, 0.0f, this->m_curElement->alpha };
            CShaderEffect::SetDiffuse(diffuse);

            C4Vector emissive = { 0.5f, 0.5f, 0.5f, 0.0f };
            CShaderEffect::SetEmissive(emissive);
        } else {
            auto modelDiffuse = this->m_curModel->m_currentDiffuse;
            auto modelEmissive = this->m_curModel->m_currentEmissive;

            if (this->m_curBatch->colorIndex < this->m_data->colors.Count()) {
                auto& modelColor = this->m_curModel->m_colors[this->m_curBatch->colorIndex];

                modelDiffuse.x *= modelColor.colorTrack.currentValue.x;
                modelDiffuse.y *= modelColor.colorTrack.currentValue.y;
                modelDiffuse.z *= modelColor.colorTrack.currentValue.z;
            }

            if (!this->m_curShaded) {
                modelEmissive.x += modelDiffuse.x;
                modelEmissive.y += modelDiffuse.y;
                modelEmissive.z += modelDiffuse.z;

                modelDiffuse.x = 0.0f;
                modelDiffuse.y = 0.0f;
                modelDiffuse.z = 0.0f;
            }

            C4Vector diffuse = { modelDiffuse.x, modelDiffuse.y, modelDiffuse.z, this->m_curElement->alpha };
            CShaderEffect::SetDiffuse(diffuse);

            C4Vector emissive = { modelEmissive.x, modelEmissive.y, modelEmissive.z, 0.0f };
            CShaderEffect::SetEmissive(emissive);
        }
    }
}

void CM2SceneRender::SetupTextures() {
    if (this->m_curType > 2) {
        for (int32_t i = 0; i < 2; i++) {
            GxRsSet(static_cast<EGxRenderState>(GxRs_Texture0 + i), 0);
            CShaderEffect::SetTexMtx_Identity(i);
        }

        return;
    }

    uint32_t textureCount = this->m_curBatch->textureCount;
    int32_t v19 = 1;

    // TODO
    // - override texture count in certain cases

    if (!(this->m_curBatch->shader & 0x4000) || textureCount != 1) {
        v19 = 0;
    }

    for (int32_t i = 0; i < 2; i++) {
        if (i >= textureCount) {
            GxRsSet(static_cast<EGxRenderState>(GxRs_Texture0 + i), static_cast<CGxTex*>(nullptr));
            continue;
        }

        auto textureIndex = this->m_data->textureCombos[this->m_curBatch->textureComboIndex + i];
        auto textureHandle = textureIndex < this->m_data->textures.Count()
            ? this->m_curModel->m_textures[textureIndex]
            : nullptr;
        auto texture = textureHandle
            ? TextureGetGxTex(textureHandle, 1, nullptr)
            : nullptr;

        if (texture) {
            uint16_t textureFlags = this->m_data->textures[textureIndex].flags;

            EGxTexWrapMode wrapU = static_cast<EGxTexWrapMode>(textureFlags & 0x1);
            EGxTexWrapMode wrapV = static_cast<EGxTexWrapMode>(textureFlags & 0x2);
            GxTexSetWrap(texture, wrapU, wrapV);
        }

        GxRsSet(static_cast<EGxRenderState>(GxRs_Texture0 + i), texture);

        auto textureTransformIndex = this->m_data->textureTransformCombos[this->m_curBatch->textureTransformComboIndex + i];
        auto stageShift = M2COMBINER_STAGE_SHIFT * (2 - (i + 1));

        uint32_t v21 = v19 == 0 ? i : 1;

        if (this->m_curBatch->shader & 0x8000 || !((this->m_curBatch->shader >> stageShift) & M2COMBINER_ENVMAP)) {
            if (textureTransformIndex >= this->m_data->textureTransforms.Count()) {
                CShaderEffect::SetTexMtx_Identity(v21);
            } else {
                CShaderEffect::SetTexMtx(this->m_curModel->m_textureMatrices[textureTransformIndex], v21);
            }
        } else {
            CShaderEffect::SetTexMtx_SphereMap(v21);
        }
    }
}
