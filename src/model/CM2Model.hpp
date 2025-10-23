#ifndef MODEL_C_M2_MODEL_HPP
#define MODEL_C_M2_MODEL_HPP

#include "gx/Camera.hpp"
#include "gx/Texture.hpp"
#include "model/CM2Lighting.hpp"
#include <cstdint>
#include <tempest/Matrix.hpp>
#include <tempest/Vector.hpp>

class CAaBox;
class CM2Scene;
class CM2Shared;
struct M2Batch;
struct M2Data;
struct M2ModelAttachment;
struct M2ModelBone;
struct M2ModelBoneSeq;
struct M2ModelCamera;
struct M2ModelColor;
struct M2ModelLight;
struct M2ModelTextureWeight;
struct M2SequenceFallback;
struct M2TrackBase;

struct CM2ModelCall {
    uint32_t type = -1;
    CM2ModelCall* modelCallNext;
    uint32_t time;
    uint32_t args[8];
};

class CM2Model {
    public:
        // Static variables
        static uint32_t s_loadingSequence;
        static uint8_t* s_sequenceBase;
        static uint32_t s_sequenceBaseSize;
        static uint32_t s_skinProfileBoneCountMax[];

        // Static functions
        static CM2Model* AllocModel(uint32_t* heapId);
        static bool Sub825E00(M2Data* data, uint32_t a2);
        static uint16_t Sub8260C0(M2Data* data, uint32_t sequenceId, int32_t a3);

        // Member variables
        uint32_t m_refCount = 1;
        uint32_t m_flags = 0;
        CM2Model** m_scenePrev = nullptr;
        CM2Model* m_sceneNext = nullptr;
        uint32_t m_loaded : 1;
        uint32_t m_flag2 : 1;
        uint32_t m_flag4 : 1;
        uint32_t m_flag8 : 1;
        uint32_t m_flag10 : 1;
        uint32_t m_flag20 : 1;
        uint32_t m_flag40 : 1;
        uint32_t m_flag80 : 1;
        uint32_t m_flag100 : 1;
        uint32_t m_flag200 : 1;
        uint32_t m_flag400 : 1;
        uint32_t m_flag800 : 1;
        uint32_t m_flag1000 : 1;
        uint32_t m_flag2000 : 1;
        uint32_t m_flag4000 : 1;
        uint32_t m_flag8000 : 1;
        uint32_t m_flag10000 : 1;
        uint32_t m_flag20000 : 1;
        uint32_t m_flag40000 : 1;
        uint32_t m_flag80000 : 1;
        uint32_t m_flag100000 : 1;
        uint32_t m_flag200000 : 1;
        uint32_t m_flag400000 : 1;
        CM2Model** m_callbackPrev = nullptr;
        CM2Model* m_callbackNext = nullptr;
        void (*m_loadedCallback)(CM2Model*, void*) = nullptr;
        void* m_loadedArg = nullptr;
        CM2Scene* m_scene = nullptr;
        CM2Shared* m_shared = nullptr;
        CM2Model* model30 = nullptr;
        CM2ModelCall* m_modelCallList = nullptr;
        CM2ModelCall** m_modelCallTail = nullptr;
        CM2Model** m_animatePrev = nullptr;
        CM2Model* m_animateNext = nullptr;
        CM2Model* m_attachParent = nullptr;
        M2ModelAttachment* m_attachments = nullptr;
        uint32_t m_attachId = -1;
        uint32_t m_attachIndex = 0xFFFF;
        CM2Model* m_attachList = nullptr;
        CM2Model** m_attachPrev = nullptr;
        CM2Model* m_attachNext = nullptr;
        uint32_t m_time = 0;
        CM2Model** m_drawPrev = nullptr;
        CM2Model* m_drawNext = nullptr;
        uint32_t* m_loops = nullptr;
        uint32_t uint74 = 0;
        float float88 = 0.0f;
        uint32_t uint90 = 0;
        M2ModelBone* m_bones = nullptr;
        C44Matrix* m_boneMatrices = nullptr;
        M2ModelColor* m_colors = nullptr;
        uint32_t* m_skinSections = nullptr;
        HTEXTURE* m_textures = nullptr;
        M2ModelTextureWeight* m_textureWeights = nullptr;
        C44Matrix* m_textureMatrices = nullptr;
        C44Matrix matrixB4;
        C44Matrix matrixF4;
        float float198 = 1.0f;
        float alpha19C = 1.0f;
        C3Vector m_currentDiffuse = { 1.0f, 1.0f, 1.0f };
        C3Vector m_currentEmissive = { 0.0f, 0.0f, 0.0f };
        M2ModelLight* m_lights;
        CM2Lighting m_lighting;
        CM2Lighting* m_currentLighting = nullptr;
        void (*m_lightingCallback)(CM2Model*, CM2Lighting*, void*) = nullptr;
        void* m_lightingArg = nullptr;
        M2ModelCamera* m_cameras = nullptr;
        void* ptr2D0 = nullptr;

        // Member functions
        CM2Model()
            : m_loaded(0)
            , m_flag2(0)
            , m_flag4(0)
            , m_flag8(0)
            , m_flag10(0)
            , m_flag20(0)
            , m_flag40(1)
            , m_flag80(0)
            , m_flag100(1)
            , m_flag200(1)
            , m_flag400(0)
            , m_flag800(0)
            , m_flag1000(0)
            , m_flag2000(0)
            , m_flag4000(0)
            , m_flag8000(0)
            , m_flag10000(0)
            , m_flag20000(0)
            , m_flag40000(0)
            , m_flag80000(0)
            , m_flag100000(0)
            , m_flag200000(0)
            , m_flag400000(0)
            {};
        ~CM2Model();
        void AddRef();
        void Animate();
        void AnimateAttachmentsMT();
        void AnimateCamerasST();
        void AnimateMT(const C44Matrix* view, const C3Vector& a3, const C3Vector& a4, float a5, float a6);
        void AnimateMTSimple(const C44Matrix* view, const C3Vector& a3, const C3Vector& a4, float a5, float a6);
        void AnimateST();
        void AttachToParent(CM2Model* parent, uint32_t id, const C3Vector* position, int32_t a5);
        void AttachToScene(CM2Scene* scene);
        void CancelDeferredSequences(uint32_t boneIndex, bool a3);
        void DetachFromParent();
        void DetachFromScene();
        void FindKey(M2ModelBoneSeq* sequence, const M2TrackBase& track, uint32_t& currentKey, uint32_t& nextKey, float& ratio);
        C44Matrix GetAttachmentWorldTransform(uint32_t id);
        CAaBox& GetBoundingBox(CAaBox& bounds);
        HCAMERA GetCameraByIndex(uint32_t index);
        C3Vector GetPosition();
        int32_t Initialize(CM2Scene* scene, CM2Shared* shared, CM2Model* a4, uint32_t flags);
        int32_t InitializeLoaded();
        int32_t IsBatchDoodadCompatible(M2Batch* batch);
        int32_t IsDrawable(int32_t a2, int32_t a3);
        int32_t IsLoaded(int32_t a2, int32_t attachments);
        void LinkToCallbackListTail();
        int32_t ProcessCallbacks();
        void ProcessCallbacksRecursive();
        void Release();
        void ReplaceTexture(uint32_t textureId, HTEXTURE texture);
        void SetAnimating(int32_t animating);
        void SetBoneSequence(uint32_t boneId, uint32_t sequenceId, uint32_t a4, uint32_t time, float a6, int32_t a7, int32_t a8);
        void SetBoneSequenceDeferred(uint16_t a2, M2Data* data, uint16_t boneIndex, uint32_t time, float a6, M2SequenceFallback fallback, int32_t a8, int32_t a9, int32_t a10);
        void SetGeometryVisible(uint32_t start, uint32_t end, int32_t visible);
        void SetIndices();
        void SetLightingCallback(void (*lightingCallback)(CM2Model*, CM2Lighting*, void*), void* lightingArg);
        void SetLoadedCallback(void (*loadedCallback)(CM2Model*, void*), void* loadedArg);
        void SetPrimaryBoneSequence(uint16_t sequenceIndex, uint16_t boneIndex, M2SequenceFallback fallback, uint32_t time, float a6, int32_t a7);
        void SetSecondaryBoneSequence(uint16_t a2, uint16_t boneIndex, M2SequenceFallback fallback, uint32_t time, float a6);
        void SetupBoneSequence(uint16_t sequenceIndex, M2SequenceFallback fallback, uint32_t a4, float a5, M2ModelBoneSeq* boneSequence);
        void SetupLighting();
        void SetVisible(int32_t visible);
        void SetWorldTransform(const C3Vector& position, float orientation, float scale);
        void Sub826350(M2SequenceFallback& fallback, uint32_t sequenceId);
        int32_t Sub8269C0(uint32_t boneId, uint16_t boneIndex);
        void Sub826E60(uint32_t* a2, uint32_t* a3);
        void UnlinkFromCallbackList();
        void UnoptimizeVisibleGeometry();
        void UnsetBoneSequence(uint32_t boneId, int32_t a3, int32_t a4);
        void UpdateLoaded();
        void WaitForLoad(const char* a2);
};

#endif
