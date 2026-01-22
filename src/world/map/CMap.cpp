#include "world/map/CMap.hpp"
#include "world/map/CChunkLiquid.hpp"
#include "world/map/CMapArea.hpp"
#include "world/map/CMapAreaLow.hpp"
#include "world/map/CMapCacheLight.hpp"
#include "world/map/CMapChunk.hpp"
#include "world/map/CMapDoodadDef.hpp"
#include "world/map/CMapEntity.hpp"
#include "world/map/CMapLight.hpp"
#include "world/map/CMapObj.hpp"
#include "world/map/CMapObjDef.hpp"
#include "world/map/CMapObjDefGroup.hpp"
#include "world/map/CMapObjGroup.hpp"
#include <common/ObjectAlloc.hpp>
#include <storm/String.hpp>

uint32_t* CMap::s_areaHeap;
uint32_t* CMap::s_areaLowHeap;
uint32_t* CMap::s_baseObjLinkHeap;
uint32_t* CMap::s_cacheLightHeap;
uint32_t* CMap::s_chunkHeap;
uint32_t* CMap::s_chunkLiquidHeap;
uint32_t* CMap::s_doodadDefHeap;
uint32_t* CMap::s_entityHeap;
STORM_EXPLICIT_LIST(CMapBaseObj, m_lameAssLink) CMap::s_entityList;
uint32_t* CMap::s_lightHeap;
uint32_t* CMap::s_mapObjDefGroupHeap;
uint32_t* CMap::s_mapObjDefHeap;
uint32_t* CMap::s_mapObjGroupHeap;
uint32_t* CMap::s_mapObjHeap;
char CMap::s_mapName[256];
char CMap::s_mapPath[256];
char CMap::s_wdtFilename[256];

CMapEntity* CMap::AllocEntity(int32_t a1) {
    CMapEntity* entity;
    uint32_t memHandle;
    void* mem = nullptr;

    if (ObjectAlloc(*CMap::s_entityHeap, &memHandle, &mem, false)) {
        entity = new (mem) CMapEntity();
        entity->m_memHandle = memHandle;
    } else {
        entity = nullptr;
    }

    if (a1) {
        CMap::s_entityList.LinkToHead(entity);
    } else {
        CMap::s_entityList.LinkToTail(entity);
    }

    return entity;
}

void CMap::Initialize() {
    // TODO

    CMap::MapMemInitialize();

    // TODO
}

void CMap::Load(const char* mapName, int32_t zoneID) {
    // TODO

    auto nameOfs = SStrCopy(CMap::s_mapPath, "World\\Maps\\");
    SStrCopy(&CMap::s_mapPath[nameOfs], mapName);

    SStrCopy(CMap::s_mapName, mapName);

    SStrPrintf(CMap::s_wdtFilename, sizeof(CMap::s_wdtFilename), "%s\\%s.wdt", CMap::s_mapPath, CMap::s_mapName);

    // TODO
}

void CMap::MapMemInitialize() {
    CMap::s_lightHeap           = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapLight),         128,    "WLIGHT",           true));
    CMap::s_cacheLightHeap      = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapCacheLight),    256,    "WCACHELIGHT",      true));
    CMap::s_mapObjGroupHeap     = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapObjGroup),      128,    "WMAPOBJGROUP",     true));
    CMap::s_mapObjHeap          = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapObj),           32,     "WMAPOBJ",          true));
    CMap::s_baseObjLinkHeap     = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapBaseObjLink),   10000,  "WBASEOBJLINK",     true));
    CMap::s_areaHeap            = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapArea),          16,     "WAREA",            true));
    // CMap::s_areaMedHeap         = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapAreaMed),       16,     "WAREAMED",         true)); ??
    CMap::s_areaLowHeap         = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapAreaLow),       16,     "WAREALOW",         true));
    CMap::s_chunkHeap           = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapChunk),         256,    "WCHUNK",           true));
    CMap::s_doodadDefHeap       = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapDoodadDef),     5000,   "WDOODADDEF",       true));
    CMap::s_entityHeap          = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapEntity),        128,    "WENTITY",          true));
    CMap::s_mapObjDefGroupHeap  = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapObjDefGroup),   128,    "WMAPOBJDEFGROUP",  true));
    CMap::s_mapObjDefHeap       = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CMapObjDef),        64,     "WMAPOBJDEF",       true));
    CMap::s_chunkLiquidHeap     = STORM_NEW(uint32_t)(ObjectAllocAddHeap(sizeof(CChunkLiquid),      64,     "WCHUNKLIQUID",     true));
}
