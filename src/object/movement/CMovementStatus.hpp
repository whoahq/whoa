#ifndef OBJECT_MOVEMENT_C_MOVEMENT_STATUS_HPP
#define OBJECT_MOVEMENT_C_MOVEMENT_STATUS_HPP

#include "util/GUID.hpp"
#include <common/DataStore.hpp>
#include <tempest/Vector.hpp>
#include <cstdint>

struct CMovementStatus {
    uint32_t uint0 = 0;
    // TODO
    WOWGUID transport = 0;
    uint32_t moveFlags = 0x0;
    uint16_t uint14 = 0;
    // TODO
    C3Vector position18;
    float facing24 = 0.0f;
    C3Vector position28;
    float facing34 = 0.0f;
    float float38 = 0.0f;
    uint32_t uint3C = 0;
    float float40 = 0.0f;
    float float44 = 0.0f;
    float float48 = 0.0f;
    float float4C = 0.0f;
    float float50 = 0.0f;
    // TODO
};

CDataStore& operator>>(CDataStore& msg, CMovementStatus& move);

#endif
