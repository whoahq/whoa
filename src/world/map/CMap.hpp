#ifndef WORLD_MAP_C_MAP_HPP
#define WORLD_MAP_C_MAP_HPP

#include "world/map/CMapBaseObj.hpp"
#include <storm/List.hpp>
#include <cstdint>

class CMapEntity;

class CMap {
    public:
        // Static variables
        static uint32_t* s_areaHeap;
        static uint32_t* s_areaLowHeap;
        static uint32_t* s_baseObjLinkHeap;
        static uint32_t* s_cacheLightHeap;
        static uint32_t* s_chunkHeap;
        static uint32_t* s_chunkLiquidHeap;
        static uint32_t* s_doodadDefHeap;
        static uint32_t* s_entityHeap;
        static STORM_EXPLICIT_LIST(CMapBaseObj, m_lameAssLink) s_entityList;
        static uint32_t* s_lightHeap;
        static uint32_t* s_mapObjDefGroupHeap;
        static uint32_t* s_mapObjDefHeap;
        static uint32_t* s_mapObjGroupHeap;
        static uint32_t* s_mapObjHeap;
        static char s_mapName[];
        static char s_mapPath[];
        static char s_wdtFilename[];

        // Static functions
        static CMapEntity* AllocEntity(int32_t a1);
        static void Initialize();
        static void Load(const char* mapName, int32_t zoneID);
        static void MapMemInitialize();
};

#endif
