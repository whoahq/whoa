#ifndef COMPONENT_UTIL_HPP
#define COMPONENT_UTIL_HPP

#include "component/Types.hpp"
#include <cstdint>

class CharSectionsRec;

struct st_variation;

struct st_color {
    CharSectionsRec* rec;
};

struct st_race {
    // The use of "section" here refers to the groups of variations represented in
    // COMPONENT_VARIATIONS. It does NOT refer to COMPONENT_SECTIONS.
    struct {
        uint32_t variationCount;
        st_variation* variationArray;
    } sections[NUM_COMPONENT_VARIATIONS];
};

struct st_variation {
    uint32_t colorCount;
    st_color* colorArray;
};

#endif
