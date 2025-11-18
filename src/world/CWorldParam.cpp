#include "world/CWorldParam.hpp"
#include "console/CVar.hpp"

CVar* CWorldParam::cvar_baseMip;
CVar* CWorldParam::cvar_bspCache;
CVar* CWorldParam::cvar_environmentDetail;
CVar* CWorldParam::cvar_extShadowQuality;
CVar* CWorldParam::cvar_farClip;
CVar* CWorldParam::cvar_farClipOverride;
CVar* CWorldParam::cvar_footstepBias;
CVar* CWorldParam::cvar_groundEffectDensity;
CVar* CWorldParam::cvar_groundEffectDist;
CVar* CWorldParam::cvar_gxTextureCacheSize;
CVar* CWorldParam::cvar_horizonFarClip;
CVar* CWorldParam::cvar_horizonNearClip;
CVar* CWorldParam::cvar_hwPCF;
CVar* CWorldParam::cvar_lod;
CVar* CWorldParam::cvar_mapObjLightLOD;
CVar* CWorldParam::cvar_mapShadows;
CVar* CWorldParam::cvar_maxLights;
CVar* CWorldParam::cvar_nearClip;
CVar* CWorldParam::cvar_objectFade;
CVar* CWorldParam::cvar_objectFadeZFill;
CVar* CWorldParam::cvar_occlusion;
CVar* CWorldParam::cvar_particleDensity;
CVar* CWorldParam::cvar_projectedTextures;
CVar* CWorldParam::cvar_shadowLevel;
CVar* CWorldParam::cvar_showFootprints;
CVar* CWorldParam::cvar_specular;
CVar* CWorldParam::cvar_terrainAlphaBitDepth;
CVar* CWorldParam::cvar_texLodBias;
CVar* CWorldParam::cvar_waterLOD;
CVar* CWorldParam::cvar_worldPoolUsage;

bool CWorldParam::BaseMipCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::BSPCacheCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::EnvironmentDetailCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::ExtShadowQualityCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::FarClipCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::FarClipOverrideCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::FootstepBiasCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::GroundEffectDensityCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::GroundEffectDistCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::HorizonFarClipScaleCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::HorizonNearClipScaleCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::HwPCFCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

void CWorldParam::Initialize() {
    CWorldParam::cvar_farClipOverride = CVar::Register(
        "farClipOverride",
        "Override old world graphic settings",
        0x1,
        "0",
        &CWorldParam::FarClipOverrideCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_lod = CVar::Register(
        "lod",
        "Video option: Toggle Lod",
        0x1,
        "0",
        &CWorldParam::LodCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_mapShadows = CVar::Register(
        "mapShadows",
        "Video option: Toggle map shadows",
        0x1,
        "1",
        &CWorldParam::MapShadowsCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_maxLights = CVar::Register(
        "MaxLights",
        "Max number of hardware lights",
        0x1,
        "4",
        &CWorldParam::MaxLightsCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_shadowLevel = CVar::Register(
        "shadowLevel",
        "Terrain shadow map mip level",
        0x1,
        "1",
        &CWorldParam::ShadowLevelCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_texLodBias = CVar::Register(
        "texLodBias",
        "Texture LOD Bias",
        0x1,
        "0.0",
        &CWorldParam::TextureLodBiasCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_farClip = CVar::Register(
        "farclip",
        "Far clip plane distance",
        0x1,
        "350",
        &CWorldParam::FarClipCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_nearClip = CVar::Register(
        "nearclip",
        "Near clip plane distance",
        0x1,
        "0.2",
        &CWorldParam::NearClipCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_specular = CVar::Register(
        "specular",
        "Specularity",
        0x1,
        "0",
        &CWorldParam::SpecularCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_mapObjLightLOD = CVar::Register(
        "mapObjLightLOD",
        "Map object light LOD",
        0x1,
        "0",
        &CWorldParam::MapObjLightLODCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_particleDensity = CVar::Register(
        "particleDensity",
        "Video option: Particle density",
        0x1,
        "1.0",
        &CWorldParam::ParticleDensityCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_waterLOD = CVar::Register(
        "waterLOD",
        "Water geometry LOD",
        0x1,
        "0",
        &CWorldParam::WaterLODCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_baseMip = CVar::Register(
        "baseMip",
        "base mipmap level",
        0x1,
        "0",
        &CWorldParam::BaseMipCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_horizonFarClip = CVar::Register(
        "horizonFarclipScale",
        "Far clip plane scale for horizon",
        0x1,
        "4.0",
        &CWorldParam::HorizonFarClipScaleCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_horizonNearClip = CVar::Register(
        "horizonNearclipScale",
        "Near clip plane scale for horizon",
        0x1,
        "0.7",
        &CWorldParam::HorizonNearClipScaleCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_showFootprints = CVar::Register(
        "showfootprints",
        "toggles rendering of footprints",
        0x1,
        "1",
        &CWorldParam::ShowFootprintsCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_bspCache = CVar::Register(
        "bspcache",
        "BSP node caching",
        0x1,
        "1",
        &CWorldParam::BSPCacheCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_footstepBias = CVar::Register(
        "footstepBias",
        "Unit footstep depth bias",
        0x1,
        "0.125",
        &CWorldParam::FootstepBiasCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_occlusion = CVar::Register(
        "occlusion",
        "Use hardware occlusion test",
        0x1,
        "1",
        &CWorldParam::OcclusionCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_worldPoolUsage = CVar::Register(
        "worldPoolUsage",
        "CGxPool usage static/dynamic",
        0x1,
        "Dynamic",
        &CWorldParam::WorldPoolUsageCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_terrainAlphaBitDepth = CVar::Register(
        "terrainAlphaBitDepth",
        "Terrain alpha map bit depth",
        0x1,
        "8",
        &CWorldParam::TerrainAlphaBitDepthCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_groundEffectDensity = CVar::Register(
        "groundEffectDensity",
        "Ground effect density",
        0x1,
        "16",
        &CWorldParam::GroundEffectDensityCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_groundEffectDist = CVar::Register(
        "groundEffectDist",
        "Ground effect dist",
        0x1,
        "70.0",
        &CWorldParam::GroundEffectDistCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_objectFade = CVar::Register(
        "objectFade",
        "Fade objects into view",
        0x1,
        "1",
        &CWorldParam::ObjectFadeCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_objectFadeZFill = CVar::Register(
        "objectFadeZFill",
        "Fade objects using ZFill pass ",
        0x1,
        "0",
        &CWorldParam::ObjectFadeZFillCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_environmentDetail = CVar::Register(
        "environmentDetail",
        "Environment detail",
        0x1,
        "1.0",
        &CWorldParam::EnvironmentDetailCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_hwPCF = CVar::Register(
        "hwPCF",
        "Hardware PCF Filtering",
        0x1,
        "1",
        &CWorldParam::HwPCFCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_extShadowQuality = CVar::Register(
        "extShadowQuality",
        "Quality of exterior shadows (0-5)",
        0x1,
        "0",
        &CWorldParam::ExtShadowQualityCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_projectedTextures = CVar::Register(
        "projectedTextures",
        "Projected Textures",
        0x1,
        "0",
        &CWorldParam::ProjectedTexturesCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    CWorldParam::cvar_gxTextureCacheSize = CVar::Register(
        "gxTextureCacheSize",
        "GX Texture Cache Size",
        0x1,
        "0",
        &CWorldParam::TextureCacheSizeCallback,
        GRAPHICS,
        false,
        nullptr,
        false
    );

    // TODO
}

bool CWorldParam::LodCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::MapObjLightLODCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::MapShadowsCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::MaxLightsCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::NearClipCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::ObjectFadeCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::ObjectFadeZFillCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::OcclusionCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::ParticleDensityCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::ProjectedTexturesCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::ShadowLevelCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::ShowFootprintsCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::SpecularCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::TerrainAlphaBitDepthCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::TextureCacheSizeCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::TextureLodBiasCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::WaterLODCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}

bool CWorldParam::WorldPoolUsageCallback(CVar* var, const char* oldValue, const char* value, void* arg) {
    // TODO
    return true;
}
