#ifndef WORLD_MAP_C_MAP_HPP
#define WORLD_MAP_C_MAP_HPP

#include <cstdint>

class CMap {
    public:
        // Static variables
        static char s_mapName[];
        static char s_mapPath[];
        static char s_wdtFilename[];

        // Static functions
        static void Load(const char* mapName, int32_t zoneID);
};

#endif
