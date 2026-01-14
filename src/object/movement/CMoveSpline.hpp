#ifndef OBJECT_MOVEMENT_C_MOVE_SPLINE_HPP
#define OBJECT_MOVEMENT_C_MOVE_SPLINE_HPP

#include "util/C3Spline.hpp"
#include "util/GUID.hpp"
#include <common/DataStore.hpp>
#include <tempest/Vector.hpp>

struct CMoveSpline {
    // TODO
    union {
        C3Vector spot = {};
        WOWGUID guid;
        float facing;
    } face;
    uint32_t flags;
    uint32_t start;
    // TODO
    uint32_t uint2C;
    uint32_t uint30;
    C3Spline_CatmullRom spline;
    C3Vector vector1F8;
    float float204;
    float float208;
    float float20C;
    uint32_t uint210;
    // TODO

    static void Skip(CDataStore* msg);
};

CDataStore& operator>>(CDataStore& msg, CMoveSpline& spline);

#endif
