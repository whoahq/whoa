#ifndef COMPONENT_COMPONENT_DATA_HPP
#define COMPONENT_COMPONENT_DATA_HPP

#include "component/Types.hpp"
#include <storm/String.hpp>
#include <cstdint>

class CM2Model;

struct ComponentData {
    int32_t raceID = 0;
    int32_t sexID = 0;
    int32_t classID = 0;
    int32_t hairColorID = 0;
    int32_t skinColorID = 0;
    int32_t faceID = 0;
    int32_t facialHairStyleID = 0;
    int32_t hairStyleID = 0;
    CM2Model* model = nullptr;
    uint32_t flags = 0x0;
    char npcBakedTexturePath[STORM_MAX_PATH] = { '\0' };

    uint32_t geosets[NUM_GEOSET] = {
        001,
        101,
        201,
        301,
        401,
        501,
        601,
        702,
        801,
        901,
        1001,
        1101,
        1201,
        1301,
        1401,
        1501,
        1601,
        1701,
        1801,
    };

    void GetPreferences(CharacterPreferences* preferences);
    void SetPreferences(CharacterPreferences* preferences);
};

#endif
