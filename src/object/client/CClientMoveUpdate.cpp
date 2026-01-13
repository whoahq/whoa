#include "object/client/CClientMoveUpdate.hpp"

CDataStore& operator>>(CDataStore& msg, CClientMoveUpdate& move) {
    msg >> move.status;

    msg.Get(move.float60);
    msg.Get(move.float64);
    msg.Get(move.float68);
    msg.Get(move.float6C);
    msg.Get(move.float70);
    msg.Get(move.float74);
    msg.Get(move.float78);
    msg.Get(move.float7C);
    msg.Get(move.float80);

    if (move.status.moveFlags & 0x8000000) {
        msg >> move.spline;
    }

    return msg;
}
