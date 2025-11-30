#ifndef WORLD_C_WORLD_HPP
#define WORLD_C_WORLD_HPP

#include <tempest/Vector.hpp>
#include <cstdint>

class Weather;

class CWorld {
    public:
        enum Enables {
            Enable_1 = 0x1,
            Enable_2 = 0x2,
            Enable_Lod = 0x4,
            Enable_8 = 0x8,
            Enable_10 = 0x10,
            Enable_Culling = 0x20,
            Enable_Shadow = 0x40,
            Enable_80 = 0x80,
            Enable_100 = 0x100,
            Enable_200 = 0x200,
            Enable_400 = 0x400,
            Enable_800 = 0x800,
            Enable_1000 = 0x1000,
            Enable_4000 = 0x4000,
            Enable_8000 = 0x8000,
            Enable_10000 = 0x10000,
            Enable_20000 = 0x20000,
            Enable_40000 = 0x40000,
            Enable_80000 = 0x80000,
            Enable_DetailDoodads = 0x100000,
            Enable_200000 = 0x200000,
            Enable_400000 = 0x400000,
            Enable_800000 = 0x800000,
            Enable_1000000 = 0x1000000,
            Enable_Particulates = 0x2000000,
            Enable_LowDetail = 0x4000000,
            Enable_8000000 = 0x8000000,
            Enable_PixelShader = 0x10000000
        };

        enum Enables2 {
            Enable_VertexShader = 0x1,
            Enable_HwPcf = 0x2
        };

        // Static variables
        static uint32_t s_enables;
        static uint32_t s_enables2;
        static Weather* s_weather;

        // Static functions
        static void Initialize(void);
        static void LoadMap(const char* mapName, const C3Vector& position, int32_t zoneID);
};

#endif
