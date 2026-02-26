#ifndef OBJECT_CLIENT_C_CLIENT_MOVE_UPDATE_HPP
#define OBJECT_CLIENT_C_CLIENT_MOVE_UPDATE_HPP

#include "object/movement/CMovementStatus.hpp"
#include "object/movement/CMoveSpline.hpp"
#include <common/datastore/CDataStore.hpp>

struct CClientMoveUpdate {
    CMovementStatus status;
    float float60;
    float float64;
    float float68;
    float float6C;
    float float70;
    float float74;
    float float78;
    float float7C;
    float float80;
    // TODO
    CMoveSpline spline;

    static void Skip(CDataStore* msg);
};

CDataStore& operator>>(CDataStore& msg, CClientMoveUpdate& move);

#endif
