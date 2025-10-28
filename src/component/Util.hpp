#ifndef COMPONENT_UTIL_HPP
#define COMPONENT_UTIL_HPP

#include "component/Types.hpp"
#include <cstdint>

class CharSectionsRec;

struct ComponentData;
struct st_variation;

struct st_color {
    CharSectionsRec* rec;
};

struct st_race {
    // The use of "section" here refers to the groups of variations represented in
    // COMPONENT_VARIATIONS. It does NOT refer to COMPONENT_SECTIONS.
    struct {
        int32_t variationCount = 0;
        st_variation* variationArray = nullptr;
    } sections[NUM_COMPONENT_VARIATIONS];
};

struct st_variation {
    int32_t colorCount = 0;
    st_color* colorArray = nullptr;
};

int32_t BuildComponentArray(uint32_t varArrayLength, st_race** varArrayPtr);

int32_t ComponentGetHairGeoset(ComponentData* data);

int32_t ComponentGetNumColors(st_race* varArray, int32_t raceId, int32_t sexId, COMPONENT_VARIATIONS sectionIndex, int32_t variationIndex);

CharSectionsRec* ComponentGetSectionsRecord(st_race* varArray, int32_t raceId, int32_t sexId, COMPONENT_VARIATIONS sectionIndex, int32_t variationIndex, int32_t colorIndex, bool* found);

int32_t ComponentValidateBase(st_race* varArray, int32_t raceId, int32_t sexId, COMPONENT_VARIATIONS sectionIndex, int32_t variationIndex, int32_t colorIndex);

int32_t CountFacialFeatures(uint32_t varArrayLength, uint32_t** featuresListPtr);

#endif
