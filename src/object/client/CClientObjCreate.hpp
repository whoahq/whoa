#ifndef OBJECT_CLIENT_C_CLIENT_OBJ_CREATE_HPP
#define OBJECT_CLIENT_C_CLIENT_OBJ_CREATE_HPP

#include "object/client/CClientMoveUpdate.hpp"
#include "util/GUID.hpp"
#include <cstdint>

class CDataStore;

struct CClientObjCreate {
    CClientMoveUpdate move;
    uint32_t flags = 0x0;
    uint32_t uint2AC;
    uint32_t uint2B0;
    // TODO
    WOWGUID guid2B8 = 0;
    uint32_t uint2C0;
    uint32_t uint2C4;
    float float2C8;
    // TODO
    WOWGUID guid2D4 = 0;

    static void Skip(CDataStore* msg);
    int32_t Get(CDataStore* msg);
};

#endif
