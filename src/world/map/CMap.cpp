#include "world/map/CMap.hpp"
#include <storm/String.hpp>

STORM_EXPLICIT_LIST(CMapBaseObj, m_lameAssLink) CMap::s_entityList;
char CMap::s_mapName[256];
char CMap::s_mapPath[256];
char CMap::s_wdtFilename[256];

void CMap::Load(const char* mapName, int32_t zoneID) {
    // TODO

    auto nameOfs = SStrCopy(CMap::s_mapPath, "World\\Maps\\");
    SStrCopy(&CMap::s_mapPath[nameOfs], mapName);

    SStrCopy(CMap::s_mapName, mapName);

    SStrPrintf(CMap::s_wdtFilename, sizeof(CMap::s_wdtFilename), "%s\\%s.wdt", CMap::s_mapPath, CMap::s_mapName);

    // TODO
}
