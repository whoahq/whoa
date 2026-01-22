#include "world/CWorld.hpp"
#include "gx/Gx.hpp"
#include "gx/Shader.hpp"
#include "model/Model2.hpp"
#include "world/Map.hpp"
#include "world/Weather.hpp"
#include <storm/Memory.hpp>

uint32_t CWorld::s_curTimeMs;
float CWorld::s_curTimeSec;
uint32_t CWorld::s_enables;
uint32_t CWorld::s_enables2;
uint32_t CWorld::s_gameTimeFixed;
float CWorld::s_gameTimeSec;
uint32_t CWorld::s_tickTimeFixed;
uint32_t CWorld::s_tickTimeMs;
float CWorld::s_tickTimeSec;
Weather* CWorld::s_weather;

uint32_t CWorld::GetCurTimeMs() {
    return CWorld::s_curTimeMs;
}

float CWorld::GetCurTimeSec() {
    return CWorld::s_curTimeSec;
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

    // TODO

    if (GxCaps().m_shaderTargets[GxSh_Pixel] > GxShPS_none) {
        CWorld::s_enables |= Enables::Enable_PixelShader;
    }

    if (GxCaps().m_shaderTargets[GxSh_Vertex] > GxShVS_none) {
        CWorld::s_enables2 |= Enables2::Enable_VertexShader;
    }

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

void CWorld::LoadMap(const char* mapName, const C3Vector& position, int32_t zoneID) {
    // TODO

    CMap::Load(mapName, zoneID);

    // TODO
}

int32_t CWorld::OnTick(const EVENT_DATA_TICK* data, void* param) {
    CWorld::SetUpdateTime(data->tickTimeSec, data->curTimeMs);

    return 1;
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
