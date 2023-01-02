#ifndef C_SIMPLE_MODEL_HPP
#define C_SIMPLE_MODEL_HPP

#include "gx/Camera.hpp"
#include "ui/CSimpleFrame.hpp"
#include "model/Model2.hpp"
#include <tempest/Vector.hpp>
#include <tempest/Box.hpp>

class CStatus;
class XMLNode;

class CSimpleModel : public CSimpleFrame {
    public:
        // Static variables
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable(void);
        static int32_t GetObjectType(void);
        static void LightingCallback(CM2Model* model, CM2Lighting* lighting, void* userArg);
        static void ModelLoaded(CM2Model* model, void* arg);
        static void RegisterScriptMethods(lua_State*);
        static void RenderModel(void* arg);

        // Member variables
        CM2Scene* m_scene = nullptr;
        CM2Model* m_model = nullptr;
        HCAMERA m_camera = nullptr;
        uint32_t m_pendingCameraIndex = 0;
        uint32_t m_pendingCameraId = 0;
        CM2Light m_light;
        C3Vector m_position;
        float m_facing = 0.0f;
        float m_scale = 1.0f;
        uint32_t m_flags = 0;
        CImVector m_fogColor = { 0 };
        float m_fogNear = 0.0f;
        float m_fogFar = 1.0f;
        CAaBox m_bounds;
        ScriptIx m_onUpdateModel;
        ScriptIx m_onAnimFinished;

        // Virtual member functions
        virtual ScriptIx* GetScriptByName(const char* name, ScriptData& data);
        virtual bool IsA(int32_t type);
        virtual int32_t GetScriptMetaTable(void);
        virtual void LoadXML(XMLNode*, CStatus*);
        virtual void OnLayerUpdate(float elapsedSec);
        virtual void OnFrameRender(CRenderBatch*, uint32_t);
        virtual void UpdateModel(void);
        virtual void OnModelLoaded(CM2Model* model);

        // Member functions
        CSimpleModel(CSimpleFrame*);
        CM2Scene* GetScene(void);
        void SetCamera(HCAMERA camera);
        void SetCameraByID(uint32_t id);
        void SetCameraByIndex(uint32_t index);
        void SetFogColor(CImVector&);
        void SetFogFar(float);
        void SetFogNear(float);
        void SetModel(const char*);
        void SetModel(CM2Model*);
        void SetScale(float);
        void SetSequence(uint32_t sequence);
        int32_t SetSequenceTime(uint32_t sequence, int32_t time);
};

#endif
