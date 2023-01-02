#include "ui/CSimpleModel.hpp"
#include "gx/Camera.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Draw.hpp"
#include "gx/Shader.hpp"
#include "gx/Transform.hpp"
#include "math/Utils.hpp"
#include "model/CM2Shared.hpp"
#include "model/M2Data.hpp"
#include "ui/CSimpleModelScript.hpp"
#include "ui/LoadXML.hpp"
#include <common/XML.hpp>
#include <tempest/Math.hpp>

int32_t CSimpleModel::s_metatable;
int32_t CSimpleModel::s_objectType;

void CSimpleModel::CreateScriptMetaTable() {
    lua_State* L = FrameScript_GetContext();
    int32_t ref = FrameScript_Object::CreateScriptMetaTable(L, &CSimpleModel::RegisterScriptMethods);
    CSimpleModel::s_metatable = ref;
}

int32_t CSimpleModel::GetObjectType() {
    if (!CSimpleModel::s_objectType) {
        CSimpleModel::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CSimpleModel::s_objectType;
}

void CSimpleModel::LightingCallback(CM2Model* model, CM2Lighting* lighting, void* userArg) {
    CSimpleModel* simpleModel = static_cast<CSimpleModel*>(userArg);

    if (simpleModel->m_flags & 0x1) {
        lighting->SetFog(simpleModel->m_fogColor, simpleModel->m_fogNear, simpleModel->m_fogFar);
    }

    if (simpleModel->m_light.m_visible) {
        lighting->AddLight(&simpleModel->m_light);
    }
}

void CSimpleModel::ModelLoaded(CM2Model* model, void* arg) {
    auto simpleModel = static_cast<CSimpleModel*>(arg);
    simpleModel->OnModelLoaded(model);
}

void CSimpleModel::RegisterScriptMethods(lua_State* L) {
    CSimpleFrame::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, SimpleModelMethods, NUM_SIMPLE_MODEL_SCRIPT_METHODS);
}

void CSimpleModel::RenderModel(void* arg) {
    auto simpleModel = static_cast<CSimpleModel*>(arg);

    if (!simpleModel->m_model) {
        return;
    }

    CRect viewRect;

    if (!simpleModel->GetRect(&viewRect)) {
        return;
    }

    C44Matrix proj;
    GxXformProjection(proj);

    C44Matrix view;
    GxXformView(view);

    float minX, maxX, minY, maxY, minZ, maxZ;
    GxXformViewport(minX, maxX, minY, maxY, minZ, maxZ);

    C3Vector cameraPos;
    C3Vector cameraTarg;

    if (simpleModel->m_camera) {
        DataMgrGetCoord(simpleModel->m_camera, 7, &cameraPos);
        DataMgrGetCoord(simpleModel->m_camera, 8, &cameraTarg);
        CameraSetupWorldProjection(simpleModel->m_camera, viewRect, 0x0);
    } else {
        C2Vector v19 = { viewRect.minX, viewRect.minY };
        CameraSetupScreenProjection(viewRect, v19, 0.0f, 1);
    }

    DDCToNDC(viewRect.minX, viewRect.minY, &viewRect.minX, &viewRect.minY);
    DDCToNDC(viewRect.maxX, viewRect.maxY, &viewRect.maxX, &viewRect.maxY);

    viewRect.minX = std::max(0.0f, std::min(viewRect.minX, 1.0f));
    viewRect.maxX = std::max(0.0f, std::min(viewRect.maxX, 1.0f));
    viewRect.minY = std::max(0.0f, std::min(viewRect.minY, 1.0f));
    viewRect.maxY = std::max(0.0f, std::min(viewRect.maxY, 1.0f));

    // Handle zero width or zero height
    if (AreEqual(viewRect.minX, viewRect.maxX, WHOA_EPSILON_1) || AreEqual(viewRect.minY, viewRect.maxY, WHOA_EPSILON_1)) {
        GxXformSetProjection(proj);
        GxXformSetView(view);
        GxXformSetViewport(minX, maxX, minY, maxY, minZ, maxZ);

        return;
    }

    float v11 = viewRect.minX * maxX;
    float v12 = viewRect.maxX * maxX;
    float v13 = viewRect.minY * maxY;
    float v14 = viewRect.maxY * maxY;

    GxXformSetViewport(v11, v12, v13, v14, 0.0f, 1.0f);
    CImVector clearColor = { 0x00, 0x00, 0x00, 0xFF };
    GxSceneClear(0x2, clearColor);
    CShaderEffect::UpdateProjMatrix();

    simpleModel->UpdateModel();

    if (simpleModel->m_pendingCameraIndex == -1u) {
        simpleModel->GetScene()->Animate(cameraPos);
        simpleModel->GetScene()->Draw(M2PASS_0);
        simpleModel->GetScene()->Draw(M2PASS_1);
    }

    GxXformSetProjection(proj);
    GxXformSetView(view);
    GxXformSetViewport(minX, maxX, minY, maxY, minZ, maxZ);
    CShaderEffect::UpdateProjMatrix();
}

CSimpleModel::CSimpleModel(CSimpleFrame* parent) : CSimpleFrame(parent) {
    this->m_light.SetVisible(0);
    this->m_light.SetLightType(M2LIGHT_1);
    this->m_light.m_ambColor = { 1.0f, 1.0f, 1.0f };
    this->m_light.m_dirColor = { 1.0f, 1.0f, 1.0f };

    this->m_fogColor = { 0xFF, 0xFF, 0xFF, 0xFF };
}

CM2Scene* CSimpleModel::GetScene() {
    if (!this->m_scene) {
        this->m_scene = M2CreateScene();
    }

    return this->m_scene;
}

FrameScript_Object::ScriptIx* CSimpleModel::GetScriptByName(const char* name, ScriptData& data) {
    auto parentScript = CSimpleFrame::GetScriptByName(name, data);

    if (parentScript) {
        return parentScript;
    }

    if (!SStrCmpI(name, "OnUpdateModel", STORM_MAX_STR)) {
        return &this->m_onUpdateModel;
    }

    if (!SStrCmpI(name, "OnAnimFinished", STORM_MAX_STR)) {
        return &this->m_onAnimFinished;
    }

    return nullptr;
}

int32_t CSimpleModel::GetScriptMetaTable() {
    return CSimpleModel::s_metatable;
}

bool CSimpleModel::IsA(int32_t type) {
    return type == CSimpleModel::s_objectType
        || type == CSimpleFrame::s_objectType
        || type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}

void CSimpleModel::LoadXML(XMLNode* node, CStatus* status) {
    CSimpleFrame::LoadXML(node, status);

    const char* fileAttr = node->GetAttributeByName("file");
    if (fileAttr && *fileAttr) {
        this->SetModel(fileAttr);

        if (!this->m_model) {
            status->Add(
                STATUS_WARNING,
                "Frame %s: Bad model file: %s",
                this->GetDisplayName(),
                fileAttr
            );
        }
    }

    const char* scaleAttr = node->GetAttributeByName("scale");
    if (scaleAttr && *scaleAttr) {
        float scale = SStrToFloat(scaleAttr);

        if (scale <= 0.0f) {
            status->Add(
                STATUS_WARNING,
                "Frame %s: Invalid model scale: %s",
                this->GetDisplayName(),
                scaleAttr
            );
        } else {
            this->SetScale(scale);
        }
    }

    const char* fogNearAttr = node->GetAttributeByName("fogNear");
    if (fogNearAttr && *fogNearAttr) {
        float fogNear = SStrToFloat(fogNearAttr);

        if (fogNear < 0.0f) {
            fogNear = 0.0f;
        }

        this->SetFogNear(fogNear);
    }

    const char* fogFarAttr = node->GetAttributeByName("fogFar");
    if (fogFarAttr && *fogFarAttr) {
        float fogFar = SStrToFloat(fogFarAttr);

        if (fogFar < 0.0f) {
            fogFar = 0.0f;
        }

        this->SetFogFar(fogFar);
    }

    for (XMLNode* child = node->m_child; child; child = child->m_next) {
        if (!SStrCmpI(child->GetName(), "FogColor", STORM_MAX_STR)) {
            CImVector color;
            LoadXML_Color(child, color);
            this->SetFogColor(color);
        }
    }
}

void CSimpleModel::OnFrameRender(CRenderBatch* batch, uint32_t layer) {
    // TODO
}

void CSimpleModel::OnLayerUpdate(float elapsedSec) {
    CSimpleFrame::OnLayerUpdate(elapsedSec);

    if (this->m_model && this->m_visible && this->m_pendingCameraIndex == -1u) {
        this->m_model->SetAnimating(1);
        this->m_model->SetVisible(1);

        if (this->m_model->m_attachParent) {
            this->m_model->m_flag20000 = 1;
        } else {
            this->m_model->m_flag10000 = 1;
        }
    }

    uint32_t advance = CMath::fuint(elapsedSec * 1000.f);
    this->GetScene()->AdvanceTime(advance);
}

void CSimpleModel::OnModelLoaded(CM2Model* model) {
    model->SetAnimating(1);

    C3Vector cameraPos;
    model->m_scene->Animate(cameraPos);

    if (this->m_pendingCameraIndex != -1u) {
        this->SetCameraByIndex(this->m_pendingCameraIndex);
    } else if (this->m_pendingCameraId != -1u) {
        this->SetCameraByID(this->m_pendingCameraId);
    }

    CAaBox bounds;
    this->m_bounds = model->GetBoundingBox(bounds);

    this->Resize(0);
}

void CSimpleModel::SetCamera(HCAMERA camera) {
    if (camera) {
        camera = HandleDuplicate(camera);
    }

    if (this->m_camera) {
        HandleClose(this->m_camera);
    }

    this->m_camera = camera;
    this->m_pendingCameraIndex = -1u;
    this->m_pendingCameraId = -1u;
}

void CSimpleModel::SetCameraByID(uint32_t id) {
    // TODO
}

void CSimpleModel::SetCameraByIndex(uint32_t index) {
    if (!this->m_model || !this->m_model->IsLoaded(0, 0)) {
        this->m_pendingCameraIndex = index;

        return;
    }

    if (!this->m_model->m_loaded) {
        this->m_model->WaitForLoad(nullptr);
    }

    if (index >= this->m_model->m_shared->m_data->cameras.Count()) {
        if (this->m_camera) {
            HandleClose(this->m_camera);
        }

        this->m_camera = nullptr;
        this->m_pendingCameraIndex = -1u;
        this->m_pendingCameraId = -1u;

        return;
    }

    HCAMERA camera = this->m_model->GetCameraByIndex(index);
    this->SetCamera(camera);
}

void CSimpleModel::SetFogColor(CImVector& fogColor) {
    this->m_fogColor = fogColor;
    this->m_flags |= 0x1;
}

void CSimpleModel::SetFogFar(float fogFar) {
    this->m_fogFar = fogFar;
}

void CSimpleModel::SetFogNear(float fogNear) {
    this->m_fogNear = fogNear;
}

void CSimpleModel::SetModel(const char* sourcefile) {
    CM2Model* model = nullptr;

    if (sourcefile && *sourcefile) {
        if (!this->m_scene) {
            this->m_scene = M2CreateScene();
        }

        model = this->m_scene->CreateModel(sourcefile, 1);
    }

    this->SetModel(model);

    if (model) {
        model->Release();
    }
}

void CSimpleModel::SetModel(CM2Model* model) {
    if (model) {
        model->m_refCount++;
    }

    if (this->m_model) {
        this->m_model->Release();
    }

    this->m_model = model;

    if (model) {
        // TODO
        this->m_model->SetLightingCallback(&CSimpleModel::LightingCallback, this);
        this->m_model->SetLoadedCallback(&CSimpleModel::ModelLoaded, this);
    }

    this->NotifyDrawLayerChanged(DRAWLAYER_ARTWORK);
}

void CSimpleModel::SetScale(float scale) {
    this->m_scale = scale;
}

void CSimpleModel::SetSequence(uint32_t sequenceId) {
    if (this->m_model) {
        this->m_model->SetBoneSequence(-1, sequenceId, -1, 0, 1.0f, 0, 1);
    }
}

int32_t CSimpleModel::SetSequenceTime(uint32_t sequence, int32_t time) {
    if (this->m_model) {
        this->m_model->SetBoneSequence(-1, sequence, -1, time, 1.0f, 0, 1);
    }

    return 1;
}

void CSimpleModel::UpdateModel() {
    if (this->m_onUpdateModel.luaRef) {
        this->RunScript(this->m_onUpdateModel, 0, nullptr);
    }

    C3Vector position = {
        this->m_position.x * this->m_layoutScale,
        this->m_position.y * this->m_layoutScale,
        this->m_position.z * this->m_layoutScale
    };

    float scale = NDCToDDCHeight(1.0f) * 1.6666666 * (this->m_scale * this->m_layoutScale);

    this->m_model->SetWorldTransform(position, this->m_facing, scale);
}
