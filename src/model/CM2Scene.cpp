#include "model/CM2Scene.hpp"
#include "gx/Shader.hpp"
#include "gx/Transform.hpp"
#include "model/CM2Cache.hpp"
#include "model/CM2Light.hpp"
#include "model/CM2Model.hpp"
#include "model/CM2SceneRender.hpp"
#include "model/CM2Shared.hpp"
#include "model/M2Internal.hpp"
#include "model/M2Sort.hpp"
#include <algorithm>
#include <cassert>
#include <tempest/Math.hpp>

uint32_t CM2Scene::s_optFlags = 0xFFFFFFFF;

void CM2Scene::AnimateThread(void* arg) {
    // TODO
}

void CM2Scene::ComputeElementShaders(M2Element* element) {
    auto model = element->model;
    auto batch = element->batch;
    auto material = &model->m_shared->m_data->materials[batch->materialIndex];
    auto lighting = model->m_currentLighting;

    int32_t shaded;
    int32_t lightCount;

    if (material->flags & 0x1 || CM2SceneRender::s_shadedList[material->blendMode] == 0) {
        shaded = 0;
        lightCount = material->flags & 0x1 ? 0 : lighting->m_lightCount;
    } else {
        shaded = 1;
        lightCount = lighting->m_lightCount;
    }

    int32_t boneInfluences = element->skinSection->boneInfluences;

    int32_t v18;
    if (material->blendMode == M2BLEND_OPAQUE) {
        v18 = 0;
    } else if (material->blendMode == M2BLEND_ALPHA_KEY) {
        v18 = CMath::fuint(element->alpha * 224.0f);
    } else {
        v18 = 1;
    }

    int32_t v8 = 0;
    if (!(material->flags & 0x1) && !(material->flags & 0x100) && lighting->m_flags & 0x10) {
        // TODO
        // v8 = Sub873FF0();

        if (v8) {
            if (lighting->m_flags & 0x8) {
                v8 = 1;
            }

            if (element->type == 1) {
                v8 = 0;
            }
        }
    }

    int32_t v9 = v18 && (/* TODO !GxCaps().dword130 ||*/ v8);
    int32_t v10 = std::min(boneInfluences, 2);
    int32_t v11 = std::min(v8, 2);

    element->vertexPermute = shaded + 2 * (v11 + v10 + 2 * v11 + lightCount + 4 * (v11 + v10 + 2 * v11));
    element->pixelPermute = v8 + 4 * (CShaderEffect::s_usePcfFiltering + 2 * v9);

    // TODO
    // element->dword3C = v8;
}

int32_t CM2Scene::SortOpaque(uint32_t a, uint32_t b, const void* userArg) {
    auto elements = static_cast<const CM2Scene*>(userArg)->m_elements.Ptr();
    auto elementA = const_cast<M2Element*>(&elements[a]);
    auto elementB = const_cast<M2Element*>(&elements[b]);

    if (elementA->type < elementB->type) {
        return -1;
    }

    if (elementA->type > elementB->type) {
        return 1;
    }

    switch (elementA->type) {
        case 0:
        case 1:
            return CM2Scene::SortOpaqueGeoBatches(elementA, elementB);

        case 3:
            return CM2Scene::SortOpaqueRibbons(elementA, elementB);

        case 4:
            return CM2Scene::SortOpaqueParticles(elementA, elementB);

        default:
            return 0;
    }
}

int32_t CM2Scene::SortOpaqueGeoBatches(M2Element* elementA, M2Element* elementB) {
    auto modelA = elementA->model;
    auto dataA = modelA->m_shared->m_data;
    auto batchA = elementA->batch;
    auto modelB = elementB->model;
    auto dataB = modelB->m_shared->m_data;
    auto batchB = elementB->batch;

    if (elementA->type == 0) {
        if (batchA->materialLayer < batchB->materialLayer) {
            return -1;
        }

        if (batchA->materialLayer > batchB->materialLayer) {
            return 1;
        }

        if (elementA->effect && elementB->effect) {
            auto effectA = elementA->effect;
            auto effectB = elementB->effect;
            auto vertexShaderA = effectA->m_vertexShaders[elementA->vertexPermute];
            auto pixelShaderA = effectA->m_pixelShaders[elementA->pixelPermute];
            auto vertexShaderB = effectB->m_vertexShaders[elementB->vertexPermute];
            auto pixelShaderB = effectB->m_pixelShaders[elementB->pixelPermute];

            if (vertexShaderA < vertexShaderB) {
                return -1;
            }

            if (vertexShaderA > vertexShaderB) {
                return 1;
            }

            if (pixelShaderA < pixelShaderB) {
                return -1;
            }

            if (pixelShaderA > pixelShaderB) {
                return 1;
            }
        }

        if (modelA->m_shared < modelB->m_shared) {
            return -1;
        }

        if (modelA->m_shared > modelB->m_shared) {
            return 1;
        }

        if ((elementA->flags & 0x4) < (elementB->flags & 0x4)) {
            return -1;
        }

        if ((elementA->flags & 0x4) > (elementB->flags & 0x4)) {
            return 1;
        }

        if (modelA < modelB) {
            return -1;
        }

        if (modelA > modelB) {
            return 1;
        }

        if (elementA->skinSection->boneComboIndex < elementB->skinSection->boneComboIndex) {
            return -1;
        }

        if (elementA->skinSection->boneComboIndex > elementB->skinSection->boneComboIndex) {
            return 1;
        }
    }

    auto materialA = &dataA->materials[batchA->materialIndex];
    auto materialB = &dataB->materials[batchB->materialIndex];

    if (materialA->blendMode < materialB->blendMode) {
        return -1;
    }

    if (materialA->blendMode > materialB->blendMode) {
        return 1;
    }

    if ((materialA->flags & 0x1F) < (materialB->flags & 0x1F)) {
        return -1;
    }

    if ((materialA->flags & 0x1F) > (materialB->flags & 0x1F)) {
        return 1;
    }

    if (batchA->textureCount > 0 && batchB->textureCount > 0) {
        for (int32_t i = 0; i < std::min(batchA->textureCount, batchB->textureCount); i++) {
            auto textureIndexA = dataA->textureCombos[batchA->textureComboIndex];
            auto textureA = textureIndexA >= dataA->textures.Count() ? 0 : reinterpret_cast<intptr_t>(modelA->m_textures[textureIndexA]);
            auto textureIndexB = dataB->textureCombos[batchB->textureComboIndex];
            auto textureB = textureIndexB >= dataB->textures.Count() ? 0 : reinterpret_cast<intptr_t>(modelB->m_textures[textureIndexB]);

            if ((textureA - textureB) / sizeof(void*) < 0) {
                return -1;
            }

            if ((textureA - textureB) / sizeof(void*) > 0) {
                return 1;
            }
        }
    }

    if (batchA->textureCount < batchB->textureCount) {
        return -1;
    }

    if (batchA->textureCount > batchB->textureCount) {
        return 1;
    }

    if (batchA < batchB)  {
        return -1;
    }

    return batchA > batchB;
}

int32_t CM2Scene::SortOpaqueParticles(M2Element* elementA, M2Element* elementB) {
    // TODO
    return 0;
}

int32_t CM2Scene::SortOpaqueRibbons(M2Element* elementA, M2Element* elementB) {
    // TODO
    return 0;
}

int32_t CM2Scene::SortTransparent(uint32_t a, uint32_t b, const void* userArg) {
    auto elements = static_cast<const CM2Scene*>(userArg)->m_elements.Ptr();
    auto elementA = const_cast<M2Element*>(&elements[a]);
    auto elementB = const_cast<M2Element*>(&elements[b]);

    if (elementA->float10 > elementB->float10) {
        return -1;
    }

    if (elementA->float10 < elementB->float10) {
        return 1;
    }

    if ((elementA->flags & 0x1) > (elementB->flags & 0x1)) {
        return -1;
    }

    if ((elementA->flags & 0x1) < (elementB->flags & 0x1)) {
        return 1;
    }

    if (elementA->priorityPlane < elementB->priorityPlane) {
        return -1;
    }

    if (elementA->priorityPlane > elementB->priorityPlane) {
        return 1;
    }

    if (elementA->float14 > elementB->float14) {
        return -1;
    }

    if (elementA->float14 < elementB->float14) {
        return 1;
    }

    if ((CM2Scene::s_optFlags & 0x4000)
        && (elementA->type != elementB->type || elementA->model != elementB->model)
        && elementA->effect
        && elementB->effect
    ) {
        auto effectA = elementA->effect;
        auto effectB = elementB->effect;
        auto vertexShaderA = effectA->m_vertexShaders[elementA->vertexPermute];
        auto pixelShaderA = effectA->m_pixelShaders[elementA->pixelPermute];
        auto vertexShaderB = effectB->m_vertexShaders[elementB->vertexPermute];
        auto pixelShaderB = effectB->m_pixelShaders[elementB->pixelPermute];

        if (vertexShaderA < vertexShaderB) {
            return -1;
        }

        if (vertexShaderA > vertexShaderB) {
            return 1;
        }

        if (pixelShaderA < pixelShaderB) {
            return -1;
        }

        if (pixelShaderA > pixelShaderB) {
            return 1;
        }
    }

    if (elementA->model < elementB->model) {
        return -1;
    }

    if (elementA->model > elementB->model) {
        return 1;
    }

    if (elementA->type < elementB->type) {
        return -1;
    }

    if (elementA->type > elementB->type) {
        return 1;
    }

    if (elementA->type <= 2) {
        if (elementA->batch->materialLayer < elementB->batch->materialLayer) {
            return -1;
        }

        if (elementA->batch->materialLayer > elementB->batch->materialLayer) {
            return 1;
        }
    }

    if (!(CM2Scene::s_optFlags & 0x4000) || !elementA->effect || !elementB->effect) {
        return CM2Scene::SortOpaque(a, b, userArg);
    }

    auto effectA = elementA->effect;
    auto effectB = elementB->effect;
    auto vertexShaderA = effectA->m_vertexShaders[elementA->vertexPermute];
    auto pixelShaderA = effectA->m_pixelShaders[elementA->pixelPermute];
    auto vertexShaderB = effectB->m_vertexShaders[elementB->vertexPermute];
    auto pixelShaderB = effectB->m_pixelShaders[elementB->pixelPermute];

    if (vertexShaderA < vertexShaderB) {
        return -1;
    }

    if (vertexShaderA > vertexShaderB) {
        return 1;
    }

    if (pixelShaderA < pixelShaderB) {
        return -1;
    }

    if (pixelShaderA > pixelShaderB) {
        return 1;
    }

    return CM2Scene::SortOpaque(a, b, userArg);
}

void CM2Scene::AdvanceTime(uint32_t a2) {
    this->m_time += a2;

    this->m_cache->UpdateShared();
    this->m_cache->GarbageCollect(0);

    this->m_flags |= 0x4;
    this->uint10 = a2;

    if (a2) {
        for (auto model = this->m_animateList; model; model = model->m_animateNext) {
            model->ProcessCallbacksRecursive();
        }
    }

    this->m_flags &= ~0x4;
}

void CM2Scene::Animate(const C3Vector& cameraPos) {
    this->uint14++;

    uint32_t optFlags = this->m_cache->m_flags & 0xE000;
    if (CM2Scene::s_optFlags != optFlags) {
        CM2Scene::s_optFlags = optFlags;
    }

    GxXformView(this->m_view);
    C3Vector invCameraPos = { -cameraPos.x, -cameraPos.y, -cameraPos.z };
    this->m_view.Translate(invCameraPos);
    this->m_viewInv = this->m_view.Inverse(this->m_view.Determinant());

    if (this->m_cache->m_flags & 0x4) {
        // In multithreaded mode, iteration over the animate list is interleaved:
        // - the current thread animates entries 0, 2, 4, ...
        // - the newly created thread animates entries 1, 3, 5, ...

        this->m_cache->BeginThread(CM2Scene::AnimateThread, this);

        CM2Model* nextModel;
        for (auto model = this->m_animateList; model; model = nextModel->m_animateNext) {
            if (!model->m_attachParent) {
                if (model->m_flag1000) {
                    C3Vector v222 = { 0.0f, 0.0f, 0.0f };
                    C3Vector v218 = { 1.0f, 1.0f, 1.0f };

                    model->AnimateMTSimple(&this->m_view, v218, v222, 1.0f, 1.0f);
                } else {
                    C3Vector v220 = { 0.0f, 0.0f, 0.0f };
                    C3Vector v221 = { 1.0f, 1.0f, 1.0f };

                    model->AnimateMT(&this->m_view, v221, v220, 1.0f, 1.0f);
                }
            }

            nextModel = model->m_animateNext;
            if (!nextModel) {
                break;
            }
        }

        this->m_cache->WaitThread();
    } else {
        for (auto model = this->m_animateList; model; model = model->m_animateNext) {
            if (!model->m_attachParent) {
                if (model->m_flag1000 != 0) {
                    C3Vector v222 = { 0.0f, 0.0f, 0.0f };
                    C3Vector v218 = { 1.0f, 1.0f, 1.0f };

                    model->AnimateMTSimple(&this->m_view, v218, v222, 1.0f, 1.0f);
                } else {
                    C3Vector v220 = { 0.0f, 0.0f, 0.0f };
                    C3Vector v221 = { 1.0f, 1.0f, 1.0f };

                    model->AnimateMT(&this->m_view, v221, v220, 1.0f, 1.0f);
                }
            }
        }
    }

    for (auto model = this->m_animateList; model; model = model->m_animateNext) {
        if (!model->m_attachParent) {
            model->AnimateST();
        }
    }

    while (this->m_animateList) {
        // TODO
        // - this is clearing out the animate list; why? something must reattach things to it...
        auto model = this->m_animateList;
        this->m_animateList = model->m_animateNext;
        model->m_animatePrev = nullptr;
        model->m_animateNext = nullptr;

        model->SetupLighting();
    }

    this->array44.SetCount(0);
    for (int32_t i = 0; i < M2PASS_COUNT; i++) {
        this->array54[i].SetCount(0);
    }

    this->m_elements.SetCount(0);
    int32_t elementIndex = 0;

    while (this->m_drawList) {
        auto model = this->m_drawList;
        this->m_drawList = model->m_drawNext;

        model->m_flag8 = 0;
        model->m_flag10000 = 0;
        model->m_drawPrev = nullptr;
        model->m_drawNext = nullptr;

        if (!model->IsDrawable(0, 0) || model->m_flag4000) {
            continue;
        }

        auto v19 = model->m_currentLighting;
        auto data = model->m_shared->m_data;
        auto v21 = v19->m_flags & 0x20;
        auto v22 = v19->m_flags & 0x40;

        if (v21 && v22) {
            // TODO
            // - liquid plane stuff
        }

        auto skinProfile = model->m_shared->skinProfile;
        auto v17 = (this->m_cache->m_flags & 0x1) == 0;

        int32_t v229;
        if (v17 || (model->m_flags & 0x1) != 0 || (v17 = (model->m_flag40) == 0, v229 = 1, v17)) {
            v229 = 0;
        }

        uint32_t batchCount;
        if (model->ptr2D0) {
            // TODO
            // batchCount = (model->ptr2D0 + 4);

            assert(false);
        } else {
            batchCount = skinProfile->batches.Count();
        }

        for (int32_t batchIndex = 0; batchIndex < batchCount; batchIndex++) {
            M2Batch* batch;
            M2SkinSection* skinSection;
            CShaderEffect* effect;
            int32_t v221;
            int32_t v222;

            if (model->ptr2D0) {
                // TODO
                // batch = &model->m_optGeo->batches[batchIndex];
                // skinSection = model->m_optGeo->skinSections[batch->skinSectionIndex];

                assert(false);
            } else {
                batch = &skinProfile->batches[batchIndex];
                skinSection = &model->m_shared->m_skinSections[batch->skinSectionIndex];

                if (!skinSection) {
                    continue;
                }
            }

            if (batch->shader == 0x8000) {
                continue;
            }

            float alpha = model->alpha19C;

            if (batch->colorIndex < data->colors.Count()) {
                auto& color = model->m_colors[batch->colorIndex];
                alpha *= color.alphaTrack.currentValue;
            }

            if (batch->textureCount) {
                auto& textureWeight = model->m_textureWeights[data->textureWeightCombos[batch->textureWeightComboIndex]];
                alpha *= textureWeight.weightTrack.currentValue;
            }

            if (alpha < 0.000099999997f) {
                continue;
            }

            M2Material* material = &data->materials[batch->materialIndex];

            auto v17 = (batch->flags & 0x4) == 0;
            if (v17 || (v17 = this->uint104 == 0, v222 = 1, v17)) {
                v222 = 0;
            }

            M2Material* layerMaterial = batch->materialLayer
                ? &data->materials[batch->materialIndex - batch->materialLayer]
                : &data->materials[batch->materialIndex];

            if (layerMaterial->blendMode > 1 || (v221 = 0, alpha < 0.99998999f)) {
                v221 = 1;
            }

            if (model->ptr2D0) {
                // TODO
                // effect = model->m_optGeo->effects[batchIndex];

                assert(false);
            } else {
                effect = model->m_shared->m_batchShaders[batchIndex];
            }

            if (!effect) {
                continue;
            }

            auto element = this->m_elements.New();

            if (v222) {
                element->type = 1;
            } else if (!model->IsBatchDoodadCompatible(batch) || v221) {
                element->type = 0;
            } else {
                element->type = 2;
            }

            element->model = model;

            element->flags = 0x0;
            if (v221 == 1 && v21 && v22 && !v222) {
                element->flags |= 0x2;
            }
            if (model->ptr2D0) {
                element->flags |= 0x4;
            }

            element->alpha = alpha;
            element->index = batchIndex;
            element->priorityPlane = batch->priorityPlane;
            element->batch = batch;
            element->skinSection = skinSection;
            element->effect = effect;

            CM2Scene::ComputeElementShaders(element);

            float v58;

            if (v221 < 1) {
                element->float14 = model->float88;
                v58 = model->float88;
            } else if (data->flags & 0x10) {
                element->float14 = (skinSection->sortCenterPosition * model->m_boneMatrices[skinSection->centerBoneIndex]).SquaredMag();
                v58 = model->float88;
            } else {
                // TODO other sort position logic

                v58 = model->float88;
            }

            element->float10 = v58;

            if (element->type == 2) {
                // TODO
            } else if (v221 == 1) {
                if (v222) {
                    if (v22) {
                        *this->array54[2].New() = elementIndex;
                    } else {
                        *this->array54[1].New() = elementIndex;
                    }
                } else {
                    if (v21) {
                        *this->array54[1].New() = elementIndex;
                    }

                    if (v22) {
                        *this->array54[2].New() = elementIndex;
                    }
                }
            } else {
                *this->array54[v221].New() = elementIndex;
            }

            elementIndex++;

            if (v229 && !v222 && v221 >= 1 && !(material->flags & 0x10)) {
                // TODO
            }
        }

        // TODO
        // - ribbons

        // TODO
        // - draw callbacks
    }

    M2HeapSort(CM2Scene::SortOpaque, this->array54[0].Ptr(), this->array54[0].Count(), this);
    M2HeapSort(CM2Scene::SortTransparent, this->array54[1].Ptr(), this->array54[1].Count(), this);
    M2HeapSort(CM2Scene::SortTransparent, this->array54[2].Ptr(), this->array54[2].Count(), this);

    // TODO sort additive particles
}

CM2Model* CM2Scene::CreateModel(const char* file, uint32_t a3) {
    if (!file) {
        return nullptr;
    }

    CM2Shared* shared = this->m_cache->CreateShared(file, a3);
    if (!shared) {
        shared = this->m_cache->CreateShared("Spells\\ErrorCube.mdx", 0);
    }

    CM2Model* model = nullptr;

    if (shared) {
        model = CM2Model::AllocModel(g_modelPool);

        if (model) {
            if (!model->Initialize(this, shared, nullptr, a3)) {
                // TODO
            }
        }

        shared->Release();
    }

    return model;
}

int32_t CM2Scene::Draw(M2PASS pass) {
    // TODO
    // - conditional check on this->dword144

    if (CM2Scene::s_optFlags != (this->m_cache->m_flags & 0xE000)) {
        CM2Scene::s_optFlags = this->m_cache->m_flags & 0xE000;
    }

    CM2SceneRender render(this);

    render.Draw(pass, this->m_elements.m_data, this->array54[pass].m_data, this->array54[pass].Count());

    if (pass == M2PASS_0) {
        render.Draw(pass, this->m_elements.m_data, this->array44.m_data, this->array44.Count());
    }

    return 1;
}

void CM2Scene::SelectLights(CM2Lighting* lighting) {
    for (auto light = this->m_lightList; light; light = light->m_lightNext) {
        lighting->AddLight(light);
    }

    // TODO
}
