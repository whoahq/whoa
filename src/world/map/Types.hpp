#ifndef WORLD_MAP_TYPES_HPP
#define WORLD_MAP_TYPES_HPP

#include <tempest/Box.hpp>
#include <tempest/Vector.hpp>
#include <cstdint>

struct SMMapObjDef {
    uint32_t nameId;
    uint32_t uniqueId;
    C3Vector pos;
    C3Vector rot;
    CAaBox extents;
    uint16_t flags;
    uint16_t doodadSet;
    uint16_t nameSet;
    uint16_t pad;
};

#endif
