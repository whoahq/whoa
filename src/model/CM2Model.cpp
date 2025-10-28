#include "model/CM2Model.hpp"
#include "async/AsyncFileRead.hpp"
#include "math/Types.hpp"
#include "model/CM2Scene.hpp"
#include "model/CM2Shared.hpp"
#include "model/M2Animate.hpp"
#include "model/M2Data.hpp"
#include "model/M2Model.hpp"
#include <cmath>
#include <new>
#include <common/DataMgr.hpp>
#include <common/ObjectAlloc.hpp>
#include <tempest/Math.hpp>

// Alignment helpers
#define ALIGN(addr, type) ((addr + alignof(type) - 1) & ~(alignof(type) - 1))
#define ALIGN_PAD(addr, type) (ALIGN(addr, type) - addr)
#define ALIGN_SIZE(addr, type, count) ALIGN(addr + sizeof(type) * count, type) - addr
#define ALIGN_BUFFER(current, start, type) (current + ALIGN_PAD((ptrdiff_t)((char*)current - (char*)start), type))

uint32_t CM2Model::s_loadingSequence = 0xFFFFFFFF;
uint8_t* CM2Model::s_sequenceBase;
uint32_t CM2Model::s_sequenceBaseSize;
uint32_t CM2Model::s_skinProfileBoneCountMax[] = { 256, 64, 53, 21 };

CM2Model* CM2Model::AllocModel(uint32_t* heapId) {
    uint32_t memHandle;
    void* object = nullptr;

    if (ObjectAlloc(*heapId, &memHandle, &object, 0)) {
        CM2Model* model = new (object) CM2Model();

        // TODO
        // model->uint2E8 = memHandle;

        return model;
    }

    return nullptr;
}

bool CM2Model::Sub825E00(M2Data* data, uint32_t a2) {
    if (data->sequenceIdxHashById.Count() == 0) {
        for (int32_t i = 0; i < data->sequences.Count(); i++) {
            auto& sequence = data->sequences[i];

            if (sequence.id == a2) {
                return i < data->sequences.Count();
            }
        }

        return data->sequences.Count() > 0xFFFF;
    }

    uint32_t v8 = a2 % data->sequenceIdxHashById.Count();
    uint16_t v5 = data->sequenceIdxHashById[v8];
    if (v5 == 0xFFFF) {
        return data->sequences.Count() > 0xFFFF;
    }
    if (data->sequences[v5].id == a2) {
        return v5 < data->sequences.Count();
    }

    int32_t v10 = 1;
    while (1) {
        v8 = (v8 + v10 * v10) % data->sequenceIdxHashById.Count();
        v5 = data->sequenceIdxHashById[v8];
        if (v5 == 0xFFFF) {
            return data->sequences.Count() > 0xFFFF;
        }

        ++v10;

        if (data->sequences[v5].id == a2) {
            return v5 < data->sequences.Count();
        }
    }
}

uint16_t CM2Model::Sub8260C0(M2Data* data, uint32_t sequenceId, int32_t a3) {
    // TODO
    return -1;
}

CM2Model::~CM2Model() {
    // TODO

    // Unlink from lists

    if (this->m_animatePrev) {
        *this->m_animatePrev = this->m_animateNext;
    }
    if (this->m_animateNext) {
        this->m_animateNext->m_animatePrev = this->m_animatePrev;
    }

    if (this->m_drawPrev) {
        *this->m_drawPrev = this->m_drawNext;
    }
    if (this->m_drawNext) {
        this->m_drawNext->m_drawPrev = this->m_drawPrev;
    }

    // TODO
}

void CM2Model::AddRef() {
    this->m_refCount++;
}

void CM2Model::Animate() {
    // TODO
}

void CM2Model::AnimateAttachmentsMT() {
    // Animate attachment visibility

    for (int32_t i = 0; i < this->m_shared->m_data->attachments.Count(); i++) {
        auto& attachment = this->m_shared->m_data->attachments[i];
        auto& modelAttachment = this->m_attachments[i];
        auto& modelBone = this->m_bones[attachment.boneIndex];

        if (
            attachment.visibilityTrack.sequenceTimes.Count() > 1
            || (attachment.visibilityTrack.sequenceTimes.Count() == 1 && attachment.visibilityTrack.sequenceTimes[0].times.Count() > this->uint90)
        ) {
            uint8_t defaultValue = 1;
            M2AnimateTrack<uint8_t, uint8_t>(this, &modelBone, attachment.visibilityTrack, modelAttachment.visibilityTrack, defaultValue);
        }
    }

    // Animate attached models

    for (auto model = this->m_attachList; model; model = model->m_attachNext) {
        C44Matrix view;

        if (model->m_attachIndex == 0xFFFF) {
            if (!model->m_flag40000) {
                continue;
            }

            view = this->m_boneMatrices[0];
        } else {
            auto& attachment = this->m_shared->m_data->attachments[model->m_attachIndex];
            auto& modelAttachment = this->m_attachments[model->m_attachIndex];

            // Attachment not currently visible
            if (!modelAttachment.visibilityTrack.currentValue) {
                continue;
            }

            view = this->m_boneMatrices[attachment.boneIndex];
            view.Translate(attachment.position);
        }

        if (model->m_flag1000) {
            model->AnimateMTSimple(&view, this->m_currentDiffuse, this->m_currentEmissive, this->float198, this->alpha19C);
        } else {
            model->AnimateMT(&view, this->m_currentDiffuse, this->m_currentEmissive, this->float198, this->alpha19C);
        }
    }
}

void CM2Model::AnimateCamerasST() {
    for (int32_t i = 0; i < this->m_shared->m_data->cameras.Count(); i++) {
        auto& camera = this->m_shared->m_data->cameras[i];
        auto& modelCamera = this->m_cameras[i];

        C3Vector v56 = modelCamera.positionTrack.currentValue + camera.positionPivot;
        C3Vector cameraPos = (v56 * this->matrixF4) * this->m_scene->m_viewInv;
        DataMgrSetCoord(modelCamera.m_camera, 7, cameraPos, 0x0);

        C3Vector v57 = modelCamera.targetTrack.currentValue + camera.targetPivot;
        C3Vector targetPos = (v57 * this->matrixF4) * this->m_scene->m_viewInv;
        DataMgrSetCoord(modelCamera.m_camera, 8, targetPos, 0x0);

        DataMgrSetFloat(modelCamera.m_camera, 5, modelCamera.rollTrack.currentValue);
    }
}

void CM2Model::AnimateMT(const C44Matrix* view, const C3Vector& a3, const C3Vector& a4, float a5, float a6) {
    if (!this->m_loaded /* TODO other conditionals */) {
        return;
    }

    // Handle attachment visibility

    if (this->m_attachParent) {
        this->m_flag8 = this->m_attachParent->m_flag8 && this->m_flag80;
        this->m_flag10000 = this->m_attachParent->m_flag10000 && this->m_flag20000;

        // TODO dword174
    }

    // TODO

    for (int32_t i = 0; i < this->m_shared->m_data->loops.Count(); i++) {
        auto loopLength = this->m_shared->m_data->loops[i].length;
        this->m_loops[i] = loopLength ? (this->m_scene->m_time - this->uint74) % loopLength : 0;
    }

    this->matrixF4 = this->matrixB4 * *view;

    this->float88 = !this->m_attachParent || this->m_attachParent->m_flags & 0x1
        ? this->matrixF4.d2 * this->matrixF4.d2 + this->matrixF4.d1 * this->matrixF4.d1 + this->matrixF4.d0 * this->matrixF4.d0
        : this->m_attachParent->float88;

    C44Matrix v237;
    C44Matrix v224;
    C3Vector v236;

    // TODO

    uint32_t elapsedTime = 0;
    if (this->m_time && this->m_scene->m_time) {
        elapsedTime = this->m_scene->m_time - this->m_time;
        this->m_time = this->m_scene->m_time;
    }

    for (int32_t i = 0; i < this->m_shared->m_data->bones.Count(); i++) {
        auto& bone = this->m_shared->m_data->bones[i];
        auto& modelBone = this->m_bones[i];

        if (modelBone.sequence.uint8 == 0xFFFF) {
            if (bone.parentIndex >= this->m_shared->m_data->bones.Count()) {
                if (i != 0) {
                    modelBone.sequence.uint0 = this->m_bones[0].sequence.uint0;
                    modelBone.sequence.uint4 = this->m_bones[0].sequence.uint4;
                    modelBone.sequence.uint6 = this->m_bones[0].sequence.uint6;
                }
            } else {
                modelBone.sequence.uint0 = this->m_bones[bone.parentIndex].sequence.uint0;
                modelBone.sequence.uint4 = this->m_bones[bone.parentIndex].sequence.uint4;
                modelBone.sequence.uint6 = this->m_bones[bone.parentIndex].sequence.uint6;
            }
        } else {
            if (this->m_time) {
                modelBone.sequence.uintC += elapsedTime;
                modelBone.sequence.uint10 += elapsedTime;
            }

            auto v45 = this->m_scene->m_time;
            auto& v46 = this->m_shared->m_data->sequences[modelBone.sequence.uint8];
            uint32_t v47 = 0;

            if (v46.flags & 0x1) {
                if (modelBone.sequence.uint10 - v45 <= 0) {
                    auto v234 = modelBone.sequence.uint10 - modelBone.sequence.uintC;
                    auto v235 = CMath::fuint(v234 * modelBone.sequence.float14);
                    v47 = modelBone.sequence.uint1C + v235;
                    v47 = std::min(v47, v46.duration);
                } else {
                    if (modelBone.sequence.uintC - v45 > 0) {
                        v45 = modelBone.sequence.uintC;
                    }

                    if (v46.duration) {
                        auto v234 = v45 - modelBone.sequence.uintC;
                        auto v235 = CMath::fuint(v234 * modelBone.sequence.float14);
                        v47 = (modelBone.sequence.uint1C + v235) % v46.duration;
                    }
                }
            } else {
                if (v46.duration) {
                    auto v234 = v45 - modelBone.sequence.uintC;
                    auto v235 = CMath::fuint(v234 * modelBone.sequence.float14);
                    v47 = (modelBone.sequence.uint1C + v235) % v46.duration;
                }
            }

            modelBone.sequence.uint0 = v47;
            modelBone.sequence.uint4 = modelBone.sequence.uint8;
            modelBone.sequence.uint6 = i;
        }

        // TODO

        uint32_t boneFlags = bone.flags | modelBone.flags;

        C44Matrix* boneParentMatrix;

        if (bone.parentIndex == 0xFFFF) {
            boneParentMatrix = &this->matrixF4;
        } else {
            boneParentMatrix = &this->m_boneMatrices[bone.parentIndex];

            if (boneFlags & (0x1 | 0x2 | 0x4)) {
                // TODO
            }
        }

        if (boneFlags & (0x80 | 0x200)) {
            C44Matrix boneLocalMatrix;

            if (bone.rotationTrack.sequenceTimes.Count()) {
                auto& rotationTrack = bone.rotationTrack;

                if (
                    rotationTrack.sequenceTimes.Count() > 1
                    || (rotationTrack.sequenceTimes.Count() == 1 && rotationTrack.sequenceTimes[0].times.Count() > this->uint90)
                ) {
                    C4Quaternion defaultValue = { 0.0f, 0.0f, 0.0f, 1.0f };
                    M2AnimateTrack<M2CompQuat, C4Quaternion>(this, &modelBone, rotationTrack, modelBone.rotationTrack, defaultValue);
                }

                boneLocalMatrix = C44Matrix(modelBone.rotationTrack.currentValue);
            } else {
                // TODO
            }

            if (bone.scaleTrack.sequenceTimes.Count()) {
                auto& scaleTrack = bone.scaleTrack;

                if (
                    scaleTrack.sequenceTimes.Count() > 1
                    || (scaleTrack.sequenceTimes.Count() == 1 && scaleTrack.sequenceTimes[0].times.Count() > this->uint90)
                ) {
                    C3Vector defaultValue = { 1.0f, 1.0f, 1.0f };
                    M2AnimateTrack<C3Vector, C3Vector>(this, &modelBone, scaleTrack, modelBone.scaleTrack, defaultValue);
                }

                boneLocalMatrix.Scale(modelBone.scaleTrack.currentValue);
            }

            // TODO
            // conditional involving bone flags and a matrix member of M2ModelBone

            C3Vector translation;

            if (bone.translationTrack.sequenceTimes.Count()) {
                auto& translationTrack = bone.translationTrack;

                if (
                    translationTrack.sequenceTimes.Count() > 1
                    || (translationTrack.sequenceTimes.Count() == 1 && translationTrack.sequenceTimes[0].times.Count() > this->uint90)
                ) {
                    C3Vector defaultValue = { 0.0f, 0.0f, 0.0f };
                    M2AnimateTrack<C3Vector, C3Vector>(this, &modelBone, translationTrack, modelBone.translationTrack, defaultValue);
                }

                translation = modelBone.translationTrack.currentValue + bone.pivot;
            } else {
                translation = bone.pivot;
            }

            boneLocalMatrix.d0 += translation.x;
            boneLocalMatrix.d1 += translation.y;
            boneLocalMatrix.d2 += translation.z;

            C3Vector negPivot = {
                -bone.pivot.x,
                -bone.pivot.y,
                -bone.pivot.z
            };

            boneLocalMatrix.Translate(negPivot);

            this->m_boneMatrices[i] = boneLocalMatrix * *boneParentMatrix;
        } else {
            this->m_boneMatrices[i] = *boneParentMatrix;
        }

        if (boneFlags & (0x8 | 0x10 | 0x20 | 0x40)) {
            // TODO
        }

        // TODO
    }

    for (int32_t i = 0; i < this->m_shared->m_data->colors.Count(); i++) {
        auto& color = this->m_shared->m_data->colors[i];
        auto& modelColor = this->m_colors[i];

        auto& colorTrack = color.colorTrack;
        if (
            colorTrack.sequenceTimes.Count() > 1
            || (colorTrack.sequenceTimes.Count() == 1 && colorTrack.sequenceTimes[0].times.Count() > this->uint90)
        ) {
            C3Vector defaultValue = { 0.0f, 0.0f, 0.0f };
            M2AnimateTrack<C3Vector, C3Vector>(
                this,
                this->m_bones,
                color.colorTrack,
                modelColor.colorTrack,
                defaultValue
            );
        }

        auto& alphaTrack = color.alphaTrack;
        if (
            alphaTrack.sequenceTimes.Count() > 1
            || (alphaTrack.sequenceTimes.Count() == 1 && alphaTrack.sequenceTimes[0].times.Count() > this->uint90)
        ) {
            float defaultValue = 1.0f;
            M2AnimateTrack<fixed16, float>(
                this,
                this->m_bones,
                color.alphaTrack,
                modelColor.alphaTrack,
                defaultValue
            );
        }
    }

    for (int32_t i = 0; i < this->m_shared->m_data->textureWeights.Count(); i++) {
        auto& textureWeight = this->m_shared->m_data->textureWeights[i];
        auto& modelTextureWeight = this->m_textureWeights[i];

        auto& weightTrack = textureWeight.weightTrack;
        if (
            weightTrack.sequenceTimes.Count() > 1
            || (weightTrack.sequenceTimes.Count() == 1 && weightTrack.sequenceTimes[0].times.Count() > this->uint90)
        ) {
            float defaultValue = 1.0f;
            M2AnimateTrack<fixed16, float>(
                this,
                this->m_bones,
                textureWeight.weightTrack,
                modelTextureWeight.weightTrack,
                defaultValue
            );
        }
    }

    // TODO

    for (int32_t i = 0; i < this->m_shared->m_data->lights.Count(); i++) {
        auto& light = this->m_shared->m_data->lights[i];
        auto& modelLight = this->m_lights[i];

        if (modelLight.uint64) {
            uint8_t defaultValue = 1;
            M2AnimateTrack<uint8_t, uint8_t>(
                this,
                &this->m_bones[light.boneIndex],
                light.visibilityTrack,
                modelLight.visibilityTrack,
                defaultValue
            );
        }

        if ((modelLight.uint64 == 0 || modelLight.visibilityTrack.currentValue == 0) && this->uint90) {
            continue;
        }

        auto& ambientIntensityTrack = light.ambientIntensityTrack;
        if (
            ambientIntensityTrack.sequenceTimes.Count() > 1
            || (ambientIntensityTrack.sequenceTimes.Count() == 1 && ambientIntensityTrack.sequenceTimes[0].times.Count() > this->uint90)
        ) {
            float defaultValue = 0.0f;
            M2AnimateTrack<float, float>(
                this,
                &this->m_bones[light.boneIndex],
                light.ambientIntensityTrack,
                modelLight.ambientIntensityTrack,
                defaultValue
            );
        }

        auto& ambientColorTrack = light.ambientColorTrack;
        if (
            ambientColorTrack.sequenceTimes.Count() > 1
            || (ambientColorTrack.sequenceTimes.Count() == 1 && ambientColorTrack.sequenceTimes[0].times.Count() > this->uint90)
        ) {
            C3Vector defaultValue = { 0.0f, 0.0f, 0.0f };
            M2AnimateTrack<C3Vector, C3Vector>(
                this,
                &this->m_bones[light.boneIndex],
                light.ambientColorTrack,
                modelLight.ambientColorTrack,
                defaultValue
            );

            float mul = modelLight.ambientIntensityTrack.currentValue * this->float198;

            modelLight.light.m_ambColor.x = modelLight.ambientColorTrack.currentValue.x * mul;
            modelLight.light.m_ambColor.y = modelLight.ambientColorTrack.currentValue.y * mul;
            modelLight.light.m_ambColor.z = modelLight.ambientColorTrack.currentValue.z * mul;
        }

        auto& diffuseIntensityTrack = light.diffuseIntensityTrack;
        if (
            diffuseIntensityTrack.sequenceTimes.Count() > 1
            || (diffuseIntensityTrack.sequenceTimes.Count() == 1 && diffuseIntensityTrack.sequenceTimes[0].times.Count() > this->uint90)
        ) {
            float defaultValue = 0.0f;
            M2AnimateTrack<float, float>(
                this,
                &this->m_bones[light.boneIndex],
                light.diffuseIntensityTrack,
                modelLight.diffuseIntensityTrack,
                defaultValue
            );
        }

        auto& diffuseColorTrack = light.diffuseColorTrack;
        if (
            diffuseColorTrack.sequenceTimes.Count() > 1
            || (diffuseColorTrack.sequenceTimes.Count() == 1 && diffuseColorTrack.sequenceTimes[0].times.Count() > this->uint90)
        ) {
            C3Vector defaultValue = { 0.0f, 0.0f, 0.0f };
            M2AnimateTrack<C3Vector, C3Vector>(
                this,
                &this->m_bones[light.boneIndex],
                light.diffuseColorTrack,
                modelLight.diffuseColorTrack,
                defaultValue
            );

            float mul = modelLight.diffuseIntensityTrack.currentValue * this->float198;

            modelLight.light.m_dirColor.x = modelLight.ambientColorTrack.currentValue.x * mul;
            modelLight.light.m_dirColor.y = modelLight.ambientColorTrack.currentValue.y * mul;
            modelLight.light.m_dirColor.z = modelLight.ambientColorTrack.currentValue.z * mul;
        }
    }

    for (int32_t i = 0; i < this->m_shared->m_data->cameras.Count(); i++) {
        auto& camera = this->m_shared->m_data->cameras[i];
        auto& modelCamera = this->m_cameras[i];

        auto& positionTrack = camera.positionTrack;
        if (
            positionTrack.sequenceTimes.Count() > 1
            || (positionTrack.sequenceTimes.Count() == 1 && positionTrack.sequenceTimes[0].times.Count() > this->uint90)
        ) {
            C3Vector defaultValue = { 0.0f, 0.0f, 0.0f };
            M2AnimateSplineTrack<M2SplineKey<C3Vector>, C3Vector>(
                this,
                this->m_bones,
                camera.positionTrack,
                modelCamera.positionTrack,
                defaultValue
            );
        }

        auto& targetTrack = camera.targetTrack;
        if (
            targetTrack.sequenceTimes.Count() > 1
            || (targetTrack.sequenceTimes.Count() == 1 && targetTrack.sequenceTimes[0].times.Count() > this->uint90)
        ) {
            C3Vector defaultValue = { 0.0f, 0.0f, 0.0f };
            M2AnimateSplineTrack<M2SplineKey<C3Vector>, C3Vector>(
                this,
                this->m_bones,
                camera.targetTrack,
                modelCamera.targetTrack,
                defaultValue
            );
        }

        auto& rollTrack = camera.rollTrack;
        if (
            rollTrack.sequenceTimes.Count() > 1
            || (rollTrack.sequenceTimes.Count() == 1 && rollTrack.sequenceTimes[0].times.Count() > this->uint90)
        ) {
            float defaultValue = 0.0f;
            M2AnimateSplineTrack<M2SplineKey<float>, float>(
                this,
                this->m_bones,
                camera.rollTrack,
                modelCamera.rollTrack,
                defaultValue
            );
        }
    }

    // TODO

    if (this->m_attachments || this->m_attachList) {
        this->AnimateAttachmentsMT();
    }

    // TODO
}

void CM2Model::AnimateMTSimple(const C44Matrix* view, const C3Vector& a3, const C3Vector& a4, float a5, float a6) {
    // TODO
}

void CM2Model::AnimateST() {
    if (!this->m_loaded) {
        return;
    }

    auto attachParent = this->m_attachParent;

    if (!attachParent) {
        this->m_currentLighting = &this->m_lighting;
    } else {
        this->m_flag8000 = attachParent->m_flag8000;

        if (this->m_flag8000 && attachParent->m_flags & 0x1) {
            this->m_currentLighting = &this->m_lighting;
        } else {
            this->m_currentLighting = attachParent->m_currentLighting;
        }
    }

    if (!this->m_currentLighting) {
        this->m_currentLighting = &this->m_lighting;
    }

    for (int32_t i = 0; i < this->m_shared->m_data->lights.Count(); i++) {
        auto& light = this->m_shared->m_data->lights[i];
        auto& modelLight = this->m_lights[i];

        int32_t visible = 0;
        if (modelLight.uint64 && modelLight.visibilityTrack.currentValue) {
            visible = 1;

            if (light.lightType == M2LIGHT_1) {
                // TODO
            } else {
                float v10 = -this->m_boneMatrices[light.boneIndex].c0;
                float v11 = -this->m_boneMatrices[light.boneIndex].c1;
                float v12 = -this->m_boneMatrices[light.boneIndex].c2;

                float x = this->m_scene->m_viewInv.a0 * v10
                        + this->m_scene->m_viewInv.b0 * v11
                        + this->m_scene->m_viewInv.c0 * v12;
                float y = this->m_scene->m_viewInv.a1 * v10
                        + this->m_scene->m_viewInv.b1 * v11
                        + this->m_scene->m_viewInv.c1 * v12;
                float z = this->m_scene->m_viewInv.a2 * v10
                        + this->m_scene->m_viewInv.b2 * v11
                        + this->m_scene->m_viewInv.c2 * v12;

                C3Vector dir = { x, y, z };

                modelLight.light.SetDirection(dir);
            }
        }

        modelLight.light.SetVisible(visible);

        // TODO modelLight.light.dword4 = this->m_scene->uint14;
    }

    if (this->m_shared->m_data->cameras.Count()) {
        this->AnimateCamerasST();
    }

    // TODO

    if (this->m_flag8) {
        this->m_drawPrev = &this->m_scene->m_drawList;
        this->m_drawNext = this->m_scene->m_drawList;
        this->m_scene->m_drawList = this;

        if (this->m_drawNext) {
            this->m_drawNext->m_drawPrev = &this->m_drawNext;
        }
    }

    // TODO

    // Animate attached models

    for (auto model = this->m_attachList; model; model = model->m_attachNext) {
        bool animate;

        if (model->m_attachIndex == 0xFFFF) {
            animate = model->m_flag40000;
        } else {
            animate = this->m_attachments[model->m_attachIndex].visibilityTrack.currentValue;
        }

        if (animate) {
            model->AnimateST();
        }
    }

    if (this->float198 == 1.0f) {
        this->uint90 = 1;
    }
}

void CM2Model::AttachToParent(CM2Model* parent, uint32_t id, const C3Vector* position, int32_t a5) {
    if (this->m_attachParent) {
        this->DetachFromParent();
    }

    this->SetAnimating(0);

    auto& attachmentIndicesById = this->m_shared->m_data->attachmentIndicesById;

    uint16_t attachIndex = 0xFFFF;
    if (parent->m_loaded && id < attachmentIndicesById.count) {
        attachIndex = attachmentIndicesById[id];
    }

    if (attachIndex == 0xFFFF && !a5) {
        return;
    }

    this->m_attachIndex = attachIndex;
    this->m_attachId = id;
    this->m_attachParent = parent;

    this->m_flag80 = 1;
    this->m_flag20000 = 1;
    this->m_flag40000 = a5 ? 1 : 0;

    this->m_attachPrev = &parent->m_attachList;
    this->m_attachNext = parent->m_attachList;
    if (parent->m_attachList) {
        parent->m_attachList->m_attachPrev = &this->m_attachNext;
    }
    parent->m_attachList = this;

    if (!this->m_loaded || !this->m_flag100) {
        auto model = this->m_attachParent;
        while (model) {
            model->m_flag100 = 0;
            model = model->m_attachParent;
        }
    }

    if (!this->m_flag2) {
        auto model = this->m_attachParent;
        while (model) {
            model->m_flag200 = 0;
            model = model->m_attachParent;
        }
    }

    if (position && this->m_attachParent->m_loaded && this->m_attachIndex != 0xFFFF) {
        auto transform = parent->GetAttachmentWorldTransform(id);
        auto scale = sqrt(transform.a0 * transform.a0 + transform.a1 * transform.a1 + transform.a2 * transform.a2);
        transform = transform.AffineInverse(scale);

        if (!this->m_flag8000) {
            this->matrixB4.Identity();
        }

        auto transformedPosition = *position * transform;

        this->matrixB4.d0 = transformedPosition.x;
        this->matrixB4.d1 = transformedPosition.y;
        this->matrixB4.d2 = transformedPosition.z;

        this->m_flag8000 = 1;
    }

    this->AddRef();
}

void CM2Model::AttachToScene(CM2Scene* scene) {
    this->DetachFromScene();

    this->m_scene = scene;

    this->m_scenePrev = &this->m_scene->m_modelList;
    this->m_sceneNext = this->m_scene->m_modelList;
    this->m_scene->m_modelList = this;
    if (this->m_sceneNext) {
        this->m_sceneNext->m_scenePrev = &this->m_sceneNext;
    }

    if (this->m_loaded) {
        for (int32_t i = 0; i < this->m_shared->m_data->lights.Count(); i++) {
            this->m_lights[i].light.Initialize(this->m_scene);
        }

        // TODO
        // - sequence / sequence fallback logic
    } else {
        for (auto modelCall = this->m_modelCallList; modelCall; modelCall = modelCall->modelCallNext) {
            modelCall->time += this->m_scene->m_time;
        }
    }
}

void CM2Model::CancelDeferredSequences(uint32_t boneIndex, bool a3) {
    // TODO
}

void CM2Model::DetachFromParent() {
    // TODO
}

void CM2Model::DetachFromScene() {
    // TODO
}

void CM2Model::FindKey(M2ModelBoneSeq* sequence, const M2TrackBase& track, uint32_t& currentKey, uint32_t& nextKey, float& ratio) {
    if (!track.sequenceTimes.Count()) {
        nextKey = 0;
        currentKey = 0;
        ratio = 0.0f;

        return;
    }

    uint32_t v6 = sequence->uint0;
    uint32_t v7 = sequence->uint4;

    if (track.loopIndex == 0xFFFF) {
        if (v7 >= track.sequenceTimes.Count()) {
            v7 = 0;
        }
    } else {
        v6 = this->m_loops[track.loopIndex];
        v7 = 0;
    }

    uint32_t v12 = track.sequenceTimes[v7].times.Count();

    if (v12 <= 1) {
        nextKey = 0;
        currentKey = 0;
        ratio = 0.0f;

        return;
    }

    if (currentKey >= v12) {
        currentKey = 0;
    }

    uint32_t v15 = currentKey;
    auto& v24 = track.sequenceTimes[v7];
    auto v14 = v24.times.Data();
    auto v16 = v6 - v14[currentKey];
    uint32_t* v17;
    uint32_t* v18;
    uint32_t* v19;
    uint32_t v20;
    uint32_t v21;

    if (v16 >= 500) {
        if (v16 < 0xFFFFFE0C) {
            v15 = 0;

            if (v6 >= 500) {
                v20 = v12;

                while (1) {
                    v21 = (v20 + v15) >> 1;

                    if (v6 >= v14[v21]) {
                        v15 = v21 + 1;

                        if (v21 + 1 >= v12 || v6 < v14[v21 + 1]) {
                            v15 = v21;
                            goto LABEL_36;
                        }
                    } else {
                        v20 = v21 - 1;
                    }

                    if (v15 >= v20) {
                        goto LABEL_36;
                    }
                }
            }

            v19 = v14 + 1;

            do {
                if (*v19 > v6) {
                    break;
                }

                ++v15;
                ++v19;
            } while (v15 < v12 - 1);
        } else if (v15) {
            v18 = &v14[v15];

            do {
                if (*v18 <= v6) {
                    break;
                }

                --v15;
                --v18;
            } while (v15);
        }
    } else if (v15 < v12 - 1) {
        v17 = &v14[v15 + 1];

        do {
            if (*v17 > v6) {
                break;
            }

            ++v15;
            ++v17;
        } while (v15 < v12 - 1);
    }

LABEL_36:

    if (v15 + 1 >= v24.times.Count()) {
        nextKey = v15;
        currentKey = v15;
        ratio = 0.0f;
    } else {
        currentKey = v15;
        nextKey = v15 + 1;

        uint32_t* v22 = &v24.times[v15];
        float v23 = static_cast<float>(v6 - v22[0]);
        float v25 = static_cast<float>(v22[1] - v22[0]);
        ratio = v23 / v25;
    }
}

C44Matrix CM2Model::GetAttachmentWorldTransform(uint32_t id) {
    if (!this->m_loaded) {
        this->WaitForLoad("GetAttachmentWorldTransform");
    }

    auto& attachmentIndicesById = this->m_shared->m_data->attachmentIndicesById;
    auto& attachments = this->m_shared->m_data->attachments;

    // Look up attachment index

    uint16_t attachIndex = 0xFFFF;
    if (id < attachmentIndicesById.count) {
        attachIndex = attachmentIndicesById[id];
    }

    // Look up bone index

    uint16_t boneIndex = 0xFFFF;
    if (attachIndex < this->m_shared->m_data->attachments.count) {
        boneIndex = attachments[attachIndex].boneIndex;
    }

    // Animate

    this->Animate();

    // Calculate attachment world transform

    C44Matrix transform;

    if (attachIndex == 0xFFFF) {
        transform = this->m_boneMatrices[0];
    } else {
        transform = this->m_boneMatrices[boneIndex];
        transform.Translate(attachments[attachIndex].position);
    }

    return transform * this->m_scene->m_viewInv;
}

CAaBox& CM2Model::GetBoundingBox(CAaBox& bounds) {
    // TODO
    // WaitForLoad

    bounds = this->m_shared->m_data->bounds.extent;

    return bounds;
}

HCAMERA CM2Model::GetCameraByIndex(uint32_t index) {
    if (!this->m_loaded) {
        this->WaitForLoad("GetCameraByIndex");
    }

    return this->m_cameras[index].m_camera;
}

C3Vector CM2Model::GetPosition() {
    return reinterpret_cast<C3Vector&>(this->matrixF4.d0) * this->m_scene->m_viewInv;
}

int32_t CM2Model::Initialize(CM2Scene* scene, CM2Shared* shared, CM2Model* a4, uint32_t flags) {
    this->AttachToScene(scene);

    // TODO
    // this->dword30[23] = this->m_scene->dwordC;

    this->m_shared = shared;
    this->m_shared->AddRef();

    if (a4) {
        a4->AddRef();
    }

    this->m_flags = flags;

    this->m_modelCallTail = &this->m_modelCallList;

    this->uint74 = this->m_scene->m_time;

    // TODO

    return this->m_shared->CallbackWhenLoaded(this);
}

int32_t CM2Model::InitializeLoaded() {
    if (!this->m_shared->m_m2DataLoaded || !this->m_shared->m_skinProfileLoaded) {
        return 1;
    }

    // Allocate a single buffer to hold unique per-model data

    uint32_t bufferSize = 0;
    bufferSize += ALIGN_SIZE(bufferSize, M2ModelBone, this->m_shared->m_data->bones.Count());
    bufferSize += ALIGN_SIZE(bufferSize, uint32_t, this->m_shared->m_data->loops.Count());
    bufferSize += ALIGN_SIZE(bufferSize, uint32_t, this->m_shared->skinProfile->skinSections.Count());
    bufferSize += ALIGN_SIZE(bufferSize, M2ModelColor, this->m_shared->m_data->colors.Count());
    bufferSize += ALIGN_SIZE(bufferSize, HTEXTURE, this->m_shared->m_data->textures.Count());
    bufferSize += ALIGN_SIZE(bufferSize, M2ModelTextureWeight, this->m_shared->m_data->textureWeights.Count());
    bufferSize += ALIGN_SIZE(bufferSize, M2ModelTextureTransform, this->m_shared->m_data->textureTransforms.Count());
    bufferSize += ALIGN_SIZE(bufferSize, M2ModelAttachment, this->m_shared->m_data->attachments.Count());
    bufferSize += ALIGN_SIZE(bufferSize, M2ModelLight, this->m_shared->m_data->lights.Count());
    bufferSize += ALIGN_SIZE(bufferSize, M2ModelCamera, this->m_shared->m_data->cameras.Count());

    // TODO allocate space for particles and ribbons

    auto buffer = static_cast<char*>(SMemAlloc(bufferSize, __FILE__, __LINE__, 0));
    auto start = buffer;

    // Allocate and initialize per-model data

    if (this->m_shared->m_data->bones.Count()) {
        buffer = ALIGN_BUFFER(buffer, start, M2ModelBone);
        this->m_bones = reinterpret_cast<M2ModelBone*>(buffer);
        buffer += sizeof(M2ModelBone) * this->m_shared->m_data->bones.Count();

        for (int32_t i = 0; i < this->m_shared->m_data->bones.Count(); i++) {
            new (&this->m_bones[i]) M2ModelBone();
        }

        for (int32_t i = 0; i < this->m_shared->m_data->bones.Count(); i++) {
            this->m_bones[i].flags = this->m_shared->m_data->bones[i].flags;
        }

        // TODO use A16 allocator
        this->m_boneMatrices = static_cast<C44Matrix*>(SMemAlloc(sizeof(C44Matrix) * this->m_shared->m_data->bones.Count(), __FILE__, __LINE__, 0));

        for (int32_t i = 0; i < this->m_shared->m_data->bones.Count(); i++) {
            new (&this->m_boneMatrices[i]) C44Matrix();
        }
    }

    if (this->m_shared->m_data->loops.Count()) {
        buffer = ALIGN_BUFFER(buffer, start, uint32_t);
        this->m_loops = reinterpret_cast<uint32_t*>(buffer);
        buffer += sizeof(uint32_t) * this->m_shared->m_data->loops.Count();

        for (int32_t i = 0; i < this->m_shared->m_data->loops.Count(); i++) {
            if (this->m_loops[i]) {
                this->m_loops[i] = 0;
            }
        }
    }


    if (this->m_shared->skinProfile->skinSections.Count()) {
        buffer = ALIGN_BUFFER(buffer, start, uint32_t);
        this->m_skinSections = reinterpret_cast<uint32_t*>(buffer);
        buffer += sizeof(uint32_t) * this->m_shared->skinProfile->skinSections.Count();

        if (this->model30) {
            memcpy(this->m_skinSections, model30->m_skinSections, sizeof(uint32_t) * this->m_shared->skinProfile->skinSections.Count());
        } else {
            // Mark all skin sections as visible by default
            for (int32_t i = 0; i < this->m_shared->skinProfile->skinSections.Count(); i++) {
                auto modelSkinSection = &this->m_skinSections[i];

                if (modelSkinSection) {
                    *modelSkinSection = 1;
                }
            }
        }
    }

    if (this->m_shared->m_data->colors.Count()) {
        buffer = ALIGN_BUFFER(buffer, start, M2ModelColor);
        this->m_colors = reinterpret_cast<M2ModelColor*>(buffer);
        buffer += sizeof(M2ModelColor) * this->m_shared->m_data->colors.Count();

        for (int32_t i = 0; i < this->m_shared->m_data->colors.Count(); i++) {
            new (&this->m_colors[i]) M2ModelColor();
        }
    }

    if (this->m_shared->m_data->textures.Count()) {
        buffer = ALIGN_BUFFER(buffer, start, HTEXTURE);
        this->m_textures = reinterpret_cast<HTEXTURE*>(buffer);
        buffer += sizeof(HTEXTURE) * this->m_shared->m_data->textures.Count();

        for (int32_t i = 0; i < this->m_shared->m_data->textures.Count(); i++) {
            HTEXTURE textureHandle = this->model30
                ? this->model30->m_textures[i]
                : this->m_shared->textures[i];

            this->m_textures[i] = textureHandle
                ? HandleDuplicate(textureHandle)
                : nullptr;
        }
    }

    if (this->m_shared->m_data->textureWeights.Count()) {
        buffer = ALIGN_BUFFER(buffer, start, M2ModelTextureWeight);
        this->m_textureWeights = reinterpret_cast<M2ModelTextureWeight*>(buffer);
        buffer += sizeof(M2ModelTextureWeight) * this->m_shared->m_data->textureWeights.Count();

        for (int32_t i = 0; i < this->m_shared->m_data->textureWeights.Count(); i++) {
            new (&this->m_textureWeights[i]) M2ModelTextureWeight();
        }
    }

    // TODO texture transforms

    if (this->m_shared->m_data->attachments.Count()) {
        buffer = ALIGN_BUFFER(buffer, start, M2ModelAttachment);
        this->m_attachments = reinterpret_cast<M2ModelAttachment*>(buffer);
        buffer += sizeof(M2ModelAttachment) * this->m_shared->m_data->attachments.Count();

        for (int32_t i = 0; i < this->m_shared->m_data->attachments.Count(); i++) {
            new (&this->m_attachments[i]) M2ModelAttachment();

            auto& modelAttachment = this->m_attachments[i];
            modelAttachment.visibilityTrack.currentValue = 1;
        }
    }

    if (this->m_shared->m_data->lights.Count()) {
        buffer = ALIGN_BUFFER(buffer, start, M2ModelLight);
        this->m_lights = reinterpret_cast<M2ModelLight*>(buffer);
        buffer += sizeof(M2ModelLight) * this->m_shared->m_data->lights.Count();

        for (int32_t i = 0; i < this->m_shared->m_data->lights.Count(); i++) {
            new (&this->m_lights[i]) M2ModelLight();

            auto& light = this->m_shared->m_data->lights[i];
            auto& modelLight = this->m_lights[i];

            modelLight.light.Initialize(this->m_scene);
            modelLight.light.SetLightType(static_cast<M2LIGHTTYPE>(light.lightType));
            modelLight.ambientIntensityTrack.currentValue = 1.0f;
            modelLight.diffuseIntensityTrack.currentValue = 1.0f;
            modelLight.visibilityTrack.currentValue = 1;
        }
    }

    if (this->m_shared->m_data->cameras.Count()) {
        buffer = ALIGN_BUFFER(buffer, start, M2ModelCamera);
        this->m_cameras = reinterpret_cast<M2ModelCamera*>(buffer);
        buffer += sizeof(M2ModelCamera) * this->m_shared->m_data->cameras.Count();

        for (int32_t i = 0; i < this->m_shared->m_data->cameras.Count(); i++) {
            new (&this->m_cameras[i]) M2ModelCamera();
        }

        for (int32_t i = 0; i < this->m_shared->m_data->cameras.Count(); i++) {
            auto& camera = this->m_shared->m_data->cameras[i];
            auto cameraHandle = CameraCreate();

            if (camera.fieldOfView <= 0.0f || camera.fieldOfView >= 3.1415927f || camera.farClip <= camera.nearClip) {
                break;
            }

            DataMgrSetFloat(cameraHandle, 4, camera.fieldOfView);
            DataMgrSetFloat(cameraHandle, 3, camera.nearClip);
            DataMgrSetFloat(cameraHandle, 2, camera.farClip);

            this->m_cameras[i].m_camera = cameraHandle;
        }
    }

    // TODO

    this->m_loaded = 1;

    uint32_t savedTime = this->m_scene->m_time;

    while (this->m_modelCallList) {
        auto modelCall = this->m_modelCallList;

        this->m_scene->m_time = modelCall->time;

        switch (modelCall->type) {
            case 0: {
                auto textureId = modelCall->args[0];
                auto texture = *reinterpret_cast<HTEXTURE*>(&modelCall->args[1]);

                this->ReplaceTexture(textureId, texture);

                break;
            }

            case 1: {
                this->SetGeometryVisible(
                    modelCall->args[0],
                    modelCall->args[1],
                    modelCall->args[2]
                );

                break;
            }

            case 2: {
                // TODO
                break;
            }

            case 3: {
                // TODO
                break;
            }

            case 4: {
                // TODO
                break;
            }

            case 5: {
                this->SetBoneSequence(
                    modelCall->args[0],
                    modelCall->args[1],
                    modelCall->args[2],
                    modelCall->args[3],
                    *reinterpret_cast<float*>(&modelCall->args[4]),
                    modelCall->args[5],
                    modelCall->args[6]
                );

                break;
            }

            case 6: {
                // TODO
                break;
            }

            case 7: {
                // TODO
                break;
            }

            case 8: {
                // TODO
                break;
            }

            case 9: {
                // TODO
                break;
            }

            case 10: {
                // TODO
                break;
            }

            case 11: {
                // TODO
                break;
            }

            case 12: {
                // TODO
                break;
            }

            case 13: {
                // TODO
                break;
            }

            case 14: {
                // TODO
                break;
            }
        }

        this->m_modelCallList = modelCall->modelCallNext;

        if (modelCall->type == 0) {
            auto texture = *reinterpret_cast<HTEXTURE*>(&modelCall->args[1]);

            if (texture) {
                HandleClose(texture);
            }
        }

        STORM_FREE(modelCall);
    }

    this->m_scene->m_time = savedTime;

    this->UpdateLoaded();
    this->m_flag800 = 0;

    return 1;
}

int32_t CM2Model::IsBatchDoodadCompatible(M2Batch* batch) {
    // TODO

    return 0;
}

int32_t CM2Model::IsDrawable(int32_t a2, int32_t a3) {
    if (!this->m_loaded && a2) {
        this->WaitForLoad(nullptr);
    }

    if (!this->m_flag2) {
        if (!this->m_loaded) {
            return 0;
        }

        for (uint32_t i = 0; i < this->m_shared->m_data->textures.Count(); i++) {
            auto texture = this->m_textures[i];

            if (!texture) {
                continue;
            }

            if (!TextureGetGxTex(texture, a2, nullptr)) {
                return 0;
            }
        }

        this->m_flag2 = 1;
    }

    if (!this->m_flag200 && a3) {
        // TODO
    }

    return 1;
}

int32_t CM2Model::IsLoaded(int32_t a2, int32_t attachments) {
    if (this->m_flags & 0x20) {
        if (this->m_loaded) {
            return 1;
        }

        if (a2) {
            this->WaitForLoad(nullptr);
        }

        return this->m_loaded && this->m_shared->m_m2DataLoaded && this->m_shared->m_skinProfileLoaded;
    }

    if (!this->m_loaded && a2) {
        this->WaitForLoad(nullptr);
    }

    if (!this->m_loaded) {
        return 0;
    }

    if (!attachments || this->m_flag100) {
        return 1;
    }

    // TODO

    return 0;
}

void CM2Model::LinkToCallbackListTail() {
    this->m_callbackPrev = this->m_shared->m_callbackListTail;
    this->m_callbackNext = nullptr;
    *this->m_shared->m_callbackListTail = this;
    this->m_shared->m_callbackListTail = &this->m_callbackNext;
}

int32_t CM2Model::ProcessCallbacks() {
    // TODO
    return 1;
}

void CM2Model::ProcessCallbacksRecursive() {
    if (!this->m_loaded) {
        return;
    }

    this->AddRef();

    if (this->ProcessCallbacks()) {
        // TODO process attachments
    }

    this->Release();
}

void CM2Model::Release() {
    // TODO
}

void CM2Model::ReplaceTexture(uint32_t textureId, HTEXTURE texture) {
    // Waiting for load

    if (!this->m_loaded) {
        auto modelCall = STORM_NEW(CM2ModelCall);

        modelCall->type = 0;
        modelCall->modelCallNext = nullptr;
        modelCall->time = this->m_scene->m_time;
        modelCall->args[0] = textureId;
        *reinterpret_cast<HTEXTURE*>(&modelCall->args[1]) = texture ? HandleDuplicate(texture) : nullptr;

        *this->m_modelCallTail = modelCall;
        this->m_modelCallTail = &modelCall->modelCallNext;

        return;
    }

    // Replace textures

    for (int32_t i = 0; i < this->m_shared->m_data->textures.Count(); i++) {
        // Only replace if texture IDs match
        if (this->m_shared->m_data->textures[i].textureId != textureId) {
            continue;
        };

        auto currentTexture = this->m_textures[i];

        if (currentTexture) {
            HandleClose(currentTexture);
        }

        if (texture) {
            this->m_textures[i] = HandleDuplicate(texture);

            auto gxTexture = TextureGetGxTex(this->m_textures[i], 0, nullptr);

            if (!gxTexture) {
                this->m_flag2 = 0;
            }
        } else {
            this->m_textures[i] = nullptr;
        }
    }

    // TODO replace ribbon textures

    // TODO replace particle textures
}

void CM2Model::SetAnimating(int32_t animating) {
    if (!animating) {
        if (this->m_animatePrev) {
            *this->m_animatePrev = this->m_animateNext;

            if (this->m_animateNext) {
                this->m_animateNext->m_animatePrev = this->m_animatePrev;
            }

            this->m_animatePrev = nullptr;
            this->m_animateNext = nullptr;
        }

        return;
    }

    if (this->m_flags & 0x20 && !this->m_loaded) {
        this->WaitForLoad(nullptr);
    }

    if (!this->m_animatePrev) {
        this->m_animatePrev = &this->m_scene->m_animateList;
        this->m_animateNext = this->m_scene->m_animateList;
        this->m_scene->m_animateList = this;

        if (this->m_animateNext) {
            this->m_animateNext->m_animatePrev = &this->m_animateNext;
        }
    }
}

void CM2Model::SetBoneSequence(uint32_t boneId, uint32_t sequenceId, uint32_t a4, uint32_t time, float a6, int32_t a7, int32_t a8) {
    if (sequenceId == -1) {
        this->UnsetBoneSequence(boneId, a7, a8);
        return;
    }

    if (!this->m_loaded) {
        auto m = SMemAlloc(sizeof(CM2ModelCall), __FILE__, __LINE__, 0x0);
        auto modelCall = new (m) CM2ModelCall();

        modelCall->type = 5;
        modelCall->modelCallNext = nullptr;
        modelCall->time = this->m_scene->m_time;
        modelCall->args[0] = boneId;
        modelCall->args[1] = sequenceId;
        modelCall->args[2] = a4;
        modelCall->args[3] = time;
        *reinterpret_cast<float*>(&modelCall->args[4]) = a6;
        modelCall->args[5] = a7;
        modelCall->args[6] = a8;

        *this->m_modelCallTail = modelCall;
        this->m_modelCallTail = &modelCall->modelCallNext;

        return;
    }

    if (this->m_flag800) {
        a7 = 0;
    }

    uint16_t boneIndex;
    if (boneId == -1) {
        boneIndex = 0;
    } else if (boneId < this->m_shared->m_data->boneIndicesById.Count()) {
        boneIndex = this->m_shared->m_data->boneIndicesById[boneId];
    } else {
        boneIndex = -1;
    }

    if (boneIndex >= this->m_shared->m_data->bones.Count()) {
        return;
    }

    M2SequenceFallback fallback;
    this->Sub826350(fallback, sequenceId);
    int32_t v33 = a4 == -1;

    uint16_t v15 = CM2Model::Sub8260C0(this->m_shared->m_data, fallback.uint0, a4 != -1 ? a4 : 0);
    uint32_t v16 = v15;
    uint32_t v32 = v15;
    uint32_t v17;

    if (v15 != 0xFFFF) {
        if (!v33) {
            goto LABEL_30;
        }

        goto LABEL_29;
    }

    v17 = this->m_shared->m_data->sequenceIdxHashById.Count();
    v33 = 1;
    v32 = v17;
    uint16_t v18;

    if (v17) {
        uint32_t v20 = fallback.uint0 % v17;
        v18 = this->m_shared->m_data->sequenceIdxHashById[v20];

        if (v18 != 0xFFFF) {
            uint32_t v21 = 1;

            if (this->m_shared->m_data->sequences[v18].id != fallback.uint0) {
                while (1) {
                    v20 = (v20 + v21 * v21) % v32;
                    v18 = this->m_shared->m_data->sequenceIdxHashById[v20];

                    if (v18 == 0xFFFF) {
                        break;
                    }

                    ++v21;

                    if (this->m_shared->m_data->sequences[v18].id == fallback.uint0) {
                        goto LABEL_26;
                    }
                }

                v32 = 0xFFFF;

                goto LABEL_29;
            }

            goto LABEL_26;
        }

        v32 = 0xFFFF;
    } else {
        v18 = 0;

        if (this->m_shared->m_data->sequences.Count())  {
            while (this->m_shared->m_data->sequences[v18].id != fallback.uint0) {
                ++v18;

                if (v18 >= this->m_shared->m_data->sequences.Count()) {
                    goto LABEL_20;
                }
            }

LABEL_26:
            v32 = v18;
            goto LABEL_29;
        }

LABEL_20:
        v32 = 0xFFFF;
    }

LABEL_29:
    this->Sub826E60(&a4, &v32);
    v16 = v32;

LABEL_30:
    if (this->m_shared->m_data->sequences[v16].flags & 0x20) {
        if (this->Sub8269C0(boneId, boneIndex)) {
            this->CancelDeferredSequences(boneIndex, a8 != 0);

            auto& modelBone = this->m_bones[boneIndex];

            if (a8) {
                modelBone.uint90 = sequenceId;
                modelBone.uint94 = a4;

                this->SetPrimaryBoneSequence(v16, boneIndex, fallback, time, a6, a7);
                modelBone.sequence.uintB = v33;
            } else {
                this->SetSecondaryBoneSequence(v16, boneIndex, fallback, time, a6);
                modelBone.secondarySequence.uintB = v33;
            }
        }
    } else {
        this->SetBoneSequenceDeferred(v16, this->m_shared->m_data, boneIndex, time, a6, fallback, a7, a8, v33);
    }
}

void CM2Model::SetBoneSequenceDeferred(uint16_t a2, M2Data* data, uint16_t boneIndex, uint32_t time, float a6, M2SequenceFallback fallback, int32_t a8, int32_t a9, int32_t a10) {
    // TODO
}

void CM2Model::SetGeometryVisible(uint32_t start, uint32_t end, int32_t visible) {
    // Waiting for load

    if (!this->m_loaded) {
        auto modelCall = STORM_NEW(CM2ModelCall);

        modelCall->type = 1;
        modelCall->modelCallNext = nullptr;
        modelCall->time = this->m_scene->m_time;
        modelCall->args[0] = start;
        modelCall->args[1] = end;
        modelCall->args[2] = visible;

        *this->m_modelCallTail = modelCall;
        this->m_modelCallTail = &modelCall->modelCallNext;

        return;
    }

    // No skin sections

    if (!this->m_shared->skinProfile->skinSections.Count()) {
        return;
    }

    // Update visibility

    bool visibilityChanged = false;

    for (uint32_t i = 0; i < this->m_shared->skinProfile->skinSections.Count(); i++) {
        auto& skinSection = this->m_shared->skinProfile->skinSections[i];
        auto modelSkinSection = &this->m_skinSections[i];

        // Out of range
        if (skinSection.skinSectionId < start || skinSection.skinSectionId > end) {
            continue;
        }

        // No change
        if (*modelSkinSection == 0 && visible == 0 || *modelSkinSection == 1 && visible == 1) {
            continue;
        }

        *modelSkinSection = visible;
        visibilityChanged = true;
    }

    if (visibilityChanged) {
        this->UnoptimizeVisibleGeometry();
    }
}

void CM2Model::SetIndices() {
    // TODO
}

void CM2Model::SetLightingCallback(void (*lightingCallback)(CM2Model*, CM2Lighting*, void*), void* lightingArg) {
    this->m_lightingCallback = lightingCallback;
    this->m_lightingArg = lightingArg;
}

void CM2Model::SetLoadedCallback(void (*loadedCallback)(CM2Model*, void*), void* loadedArg) {
    this->m_loadedCallback = loadedCallback;
    this->m_loadedArg = loadedArg;

    this->UpdateLoaded();
}

void CM2Model::SetPrimaryBoneSequence(uint16_t sequenceIndex, uint16_t boneIndex, M2SequenceFallback fallback, uint32_t time, float a6, int32_t a7) {
    auto& modelBone = this->m_bones[boneIndex];
    auto& sequence = this->m_shared->m_data->sequences[sequenceIndex];

    if (a7) {
        if (!modelBone.sequence.uintA || sequenceIndex != modelBone.sequence.uint8) {
            double v10;
            double v11;
            double v12;

            if (modelBone.secondarySequence.uint8 == 0xFFFF
                || ((v10 = (double)(modelBone.uint9C - this->m_scene->m_time) * modelBone.floatA0, v10 >= 0.0) ? (v10 <= 1.0 ? (v11 = v10 * ((3.0 - (v10 + v10)) * v10)) : (v11 = 1.0)) : (v11 = 0.0), v11 * modelBone.floatA4 <= 0.5)
            ) {
                memcpy(&modelBone.secondarySequence, &modelBone.sequence, sizeof(modelBone.secondarySequence));

                modelBone.uint9C = this->m_scene->m_time + sequence.blendtime;
                if (sequence.blendtime) {
                    v12 = 1.0 / (double)sequence.blendtime;
                } else {
                    v12 = 1.0;
                }
                modelBone.floatA0 = v12;
                modelBone.floatA4 = 1.0f;
            }
        }
    } else {
        modelBone.secondarySequence.uint8 = -1;
    }

    this->SetupBoneSequence(sequenceIndex, fallback, time, a6, &modelBone.sequence);

    int32_t v13 = modelBone.sequence.uint10;
    if (modelBone.sequence.uintC == v13 || ((sequence.flags & 0x1) != 0 && (v13 -= this->m_scene->m_time, v13 <= 0))) {
        modelBone.sequence.uintA = 1;
    }

    // TODO
    // if (!modelBone.dword98) {
    //     modelBone.dword98 = (DWORD)&this->dword14;
    //     v14 = this->dword14;
    //     v15 = &smodelBone.word96;
    //     *v15 = v14;
    //     if (v14 != 0xFFFF) {
    //         this->m_bones[v14].dword98 = v15;
    //     }
    //     LOWORD(v13) = a3;
    //     LOWORD(this->dword14) = a3;
    // }
}

void CM2Model::SetSecondaryBoneSequence(uint16_t a2, uint16_t boneIndex, M2SequenceFallback fallback, uint32_t time, float a6) {
    // TODO
}

void CM2Model::SetupBoneSequence(uint16_t sequenceIndex, M2SequenceFallback fallback, uint32_t a4, float a5, M2ModelBoneSeq* boneSequence) {
    auto& sequence = this->m_shared->m_data->sequences[sequenceIndex];

    int32_t v9 = rand();
    uint32_t v10 = (sequence.replay.l + (sequence.replay.h - sequence.replay.l) * v9 / 0x8000 == 0)
        + sequence.replay.l + (sequence.replay.h - sequence.replay.l) * v9 / 0x8000;
    int32_t v11 = v10 * sequence.duration;

    double v12;
    double v13;
    long double v15;

    if (fallback.uint2 == 1 || fallback.uint2 == 3) {
        v12 = -a5;
    } else {
        v12 = a5;
    }

    v13 = 0.0;

    uint32_t v18 = 0;
    if (v12 < 0.0) {
        v18 = v11;
    }

    if (fallback.uint2 == 2 || fallback.uint2 == 3) {
        v12 = 0.0;
    }

    if (abs(v12) > 0.0000099999997) {
        v13 = 1.0 / v12;
    }

    v15 = abs(v13);
    uint32_t v16 = this->m_scene->m_time - floor((double)a4 * v15);

    if ((~(this->m_scene->m_flags >> 2) & 0x1) != 0) {
        v16++;
    }

    boneSequence->uint8 = sequenceIndex;
    boneSequence->uintC = v16;
    boneSequence->uint20 = v10;
    boneSequence->uintA = 0;
    boneSequence->uint10 = v16 + floor(v15 * (double)(unsigned int)v11);
    boneSequence->uint1C = v18;
    boneSequence->float14 = v12;
    boneSequence->float18 = v13;
}

void CM2Model::SetupLighting() {
    if (!this->m_attachParent || this->m_attachParent->m_flags & 0x1) {
        this->Animate();

        CAaSphere sphere;
        sphere.c = this->GetPosition();
        sphere.r = 0.0f;

        this->m_lighting.Initialize(this->m_scene, sphere);
        this->m_scene->SelectLights(&this->m_lighting);

        if (this->m_lightingCallback) {
            this->m_lightingCallback(this, &this->m_lighting, this->m_lightingArg);
        }

        this->m_lighting.SetupSunlight();
        this->m_lighting.CameraSpace();
    }

    for (auto model = this->m_attachList; model; model = model->m_attachNext) {
        model->SetupLighting();
    }
}

void CM2Model::SetVisible(int32_t visible) {
    if (this->m_attachParent) {
        this->m_flag80 = visible ? 1 : 0;
    } else {
        this->m_flag8 = visible ? 1 : 0;
    }
}

void CM2Model::SetWorldTransform(const C3Vector& position, float orientation, float scale) {
    C44Matrix(this->matrixB4);

    this->matrixB4.RotateAroundZ(orientation);
    this->matrixB4.Scale(scale);
    this->matrixB4.d0 = position.x;
    this->matrixB4.d1 = position.y;
    this->matrixB4.d2 = position.z;

    this->m_flag8000 = 1;
}

void CM2Model::Sub826350(M2SequenceFallback& fallback, uint32_t sequenceId) {
    auto data = this->m_shared->m_data;

    int32_t v12;
    if (CM2Model::Sub825E00(data, 0)) {
        v12 = 0;
    } else if (CM2Model::Sub825E00(data, 147)) {
        v12 = 147;
    } else {
        v12 = data->sequences[0].id;
    }

    uint32_t v10[506];
    memset(v10, 0, sizeof(v10));

    if (CM2Model::Sub825E00(data, sequenceId)) {
        fallback.uint0 = sequenceId;
        fallback.uint2 = 0;
        return;
    }

    // TODO
}

int32_t CM2Model::Sub8269C0(uint32_t boneId, uint16_t boneIndex) {
    // TODO
    return 1;
}

void CM2Model::Sub826E60(uint32_t* a2, uint32_t* a3) {
    // TODO
}

void CM2Model::UnlinkFromCallbackList() {
    if (this->m_callbackPrev) {
        *this->m_callbackPrev = this->m_callbackNext;

        if (this->m_callbackNext) {
            this->m_callbackNext->m_callbackPrev = this->m_callbackPrev;
        } else {
            this->m_shared->m_callbackListTail = this->m_callbackPrev;
        }

        this->m_callbackPrev = nullptr;
        this->m_callbackNext = nullptr;
    }
}

void CM2Model::UnoptimizeVisibleGeometry() {
    // TODO
}

void CM2Model::UnsetBoneSequence(uint32_t boneId, int32_t a3, int32_t a4) {
    // TODO
}

void CM2Model::UpdateLoaded() {
    auto model = this;

    while (model) {
        if (!model->IsLoaded(0, !(this->m_flags & 0x20))) {
            break;
        }

        if (model->m_loadedCallback) {
            model->m_loadedCallback(this, model->m_loadedArg);
            model->m_loadedCallback = nullptr;
        }

        model = model->m_attachParent;
    }
}

void CM2Model::WaitForLoad(const char* a2) {
    if (this->m_shared->asyncObject) {
        AsyncFileReadWait(this->m_shared->asyncObject);
    }

    if (this->m_shared->asyncObject) {
        AsyncFileReadWait(this->m_shared->asyncObject);
    }

    if (this->m_flags & 0x20) {
        this->InitializeLoaded();
    }
}
