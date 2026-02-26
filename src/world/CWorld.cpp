#include "world/CWorld.hpp"
#include "gx/Gx.hpp"
#include "gx/Shader.hpp"
#include "model/Model2.hpp"
#include "world/CWorldParam.hpp"
#include "world/Map.hpp"
#include "world/Weather.hpp"
#include <storm/Memory.hpp>

uint32_t CWorld::s_curTimeMs;
float CWorld::s_curTimeSec;
uint32_t CWorld::s_enables;
uint32_t CWorld::s_enables2;
float CWorld::s_farClip;
uint32_t CWorld::s_gameTimeFixed;
float CWorld::s_gameTimeSec;
CM2Scene* CWorld::s_m2Scene;
float CWorld::s_nearClip = 0.1f;
float CWorld::s_prevFarClip;
uint32_t CWorld::s_tickTimeFixed;
uint32_t CWorld::s_tickTimeMs;
float CWorld::s_tickTimeSec;
Weather* CWorld::s_weather;

namespace {

float AdjustFarClip(float farClip, int32_t mapID) {
    float minFarClip = 183.33333f;
    float maxFarClip = 1583.3334f;

    if (mapID < 530 || mapID == 575 || mapID == 543) {
        if (!CWorldParam::cvar_farClipOverride || CWorldParam::cvar_farClipOverride->GetInt() < 1) {
            maxFarClip = 791.66669f;
        }
    } else if (false /* TODO OsGetPhysicalMemory() <= 1073741824 */) {
        maxFarClip = 791.66669f;
    }

    return std::min(std::max(farClip, minFarClip), maxFarClip);
}

}

HWORLDOBJECT CWorld::AddObject(CM2Model* model, void* handler, void* handlerParam, uint64_t param64, uint32_t param32, uint32_t objFlags) {
    auto entity = CMap::AllocEntity(objFlags & 0x8 ? true : false);

    entity->m_model = model;
    entity->m_param64 = param64;
    entity->m_param32 = param32;

    // TODO

    entity->m_dirLightScale = 1.0f;
    entity->m_dirLightScaleTarget = 1.0f;

    // TODO

    entity->m_type |= CMapBaseObj::Type_200;

    // TODO

    entity->m_flags = 0x0;

    if (objFlags & 0x20) {
        entity->m_flags = 0x20000;
    }

    // TODO

    return reinterpret_cast<HWORLDOBJECT>(entity);
}

uint32_t CWorld::GetCurTimeMs() {
    return CWorld::s_curTimeMs;
}

float CWorld::GetCurTimeSec() {
    return CWorld::s_curTimeSec;
}

float CWorld::GetFarClip() {
    return CWorld::s_farClip;
}

uint32_t CWorld::GetFixedPrecisionTime(float timeSec) {
    return static_cast<uint32_t>(timeSec * 1024.0f);
}

uint32_t CWorld::GetGameTimeFixed() {
    return CWorld::s_gameTimeFixed;
}

float CWorld::GetGameTimeSec() {
    return CWorld::s_gameTimeSec;
}

CM2Scene* CWorld::GetM2Scene() {
    return CWorld::s_m2Scene;
}

float CWorld::GetNearClip() {
    return CWorld::s_nearClip;
}

uint32_t CWorld::GetTickTimeFixed() {
    return CWorld::s_tickTimeFixed;
}

uint32_t CWorld::GetTickTimeMs() {
    return CWorld::s_tickTimeMs;
}

float CWorld::GetTickTimeSec() {
    return CWorld::s_tickTimeSec;
}

void CWorld::Initialize() {
    CWorld::s_enables |=
          Enables::Enable_1
        | Enables::Enable_2
        | Enables::Enable_10
        | Enables::Enable_Culling
        | Enables::Enable_Shadow
        | Enables::Enable_100
        | Enables::Enable_200
        | Enables::Enable_800
        | Enables::Enable_4000
        | Enables::Enable_DetailDoodads
        | Enables::Enable_1000000
        | Enables::Enable_Particulates
        | Enables::Enable_LowDetail;

    CWorld::s_gameTimeFixed = 0;
    CWorld::s_gameTimeSec = 0.0f;

    if (GxCaps().m_shaderTargets[GxSh_Pixel] > GxShPS_none) {
        CWorld::s_enables |= Enables::Enable_PixelShader;
    }

    if (GxCaps().m_shaderTargets[GxSh_Vertex] > GxShVS_none) {
        CWorld::s_enables2 |= Enables2::Enable_VertexShader;
    }

    // TODO

    CWorld::s_m2Scene = M2CreateScene();

    // TODO

    uint32_t m2Flags = M2GetCacheFlags();
    CShaderEffect::InitShaderSystem(
        (m2Flags & 0x8) != 0,
        (CWorld::s_enables2 & Enables2::Enable_HwPcf) != 0
    );

    // TODO

    CMap::Initialize();

    // TODO

    CWorld::s_weather = STORM_NEW(Weather);

    // TODO
}

void CWorld::LoadMap(const char* mapName, const C3Vector& position, int32_t mapID) {
    CWorld::s_farClip = AdjustFarClip(CWorldParam::cvar_farClip->GetFloat(), mapID);
    CWorld::s_nearClip = 0.2f;
    CWorld::s_prevFarClip = CWorld::s_farClip;

    // TODO

    CMap::Load(mapName, mapID);

    // TODO
}

int32_t CWorld::OnTick(const EVENT_DATA_TICK* data, void* param) {
    CWorld::SetUpdateTime(data->tickTimeSec, data->curTimeMs);

    return 1;
}

void CWorld::SetFarClip(float farClip) {
    farClip = AdjustFarClip(farClip, CMap::s_mapID);

    if (CWorld::s_farClip == farClip) {
        return;
    }

    CWorld::s_prevFarClip = CWorld::s_farClip;
    CWorld::s_farClip = farClip;

    // TODO CMapRenderChunk::DirtyPools();

    CWorld::s_nearClip = 0.2f;

    // TODO dword_D1C410 = 1;
    // TODO dword_ADEEE0 = 1;
}

void CWorld::SetUpdateTime(float tickTimeSec, uint32_t curTimeMs) {
    auto tickTimeFixed = CWorld::GetFixedPrecisionTime(tickTimeSec);

    CWorld::s_curTimeMs = curTimeMs;
    CWorld::s_curTimeSec = static_cast<float>(curTimeMs) * 0.001f;

    CWorld::s_gameTimeFixed += tickTimeFixed;
    CWorld::s_gameTimeSec += tickTimeSec;

    CWorld::s_tickTimeFixed = tickTimeFixed;
    CWorld::s_tickTimeMs = static_cast<uint32_t>(tickTimeSec * 1000.0f);
    CWorld::s_tickTimeSec = tickTimeSec;
}

void CWorld::Update(const C3Vector& cameraPos, const C3Vector& cameraTarget, const C3Vector& targetPos) {
    // TODO
}
