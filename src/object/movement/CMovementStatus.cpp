#include "object/movement/CMovementStatus.hpp"
#include "util/DataStore.hpp"

uint32_t CMovementStatus::Skip(CDataStore* msg) {
    uint32_t moveFlags = 0;
    msg->Get(moveFlags);

    uint16_t uint14;
    msg->Get(uint14);

    void* data;
    msg->GetDataInSitu(data, 20);

    uint32_t skipBytes = 0;

    if (moveFlags & 0x200) {
        SmartGUID guid;
        *msg >> guid;

        skipBytes += 21;

        if (uint14 & 0x400) {
            skipBytes += 4;
        }
    }

    if ((moveFlags & (0x200000 | 0x2000000)) || (uint14 & 0x20)) {
        skipBytes += 4;
    }

    skipBytes += 4;

    if (moveFlags & 0x1000) {
        skipBytes += 16;
    }

    if (moveFlags & 0x4000000) {
        skipBytes += 4;
    }

    msg->GetDataInSitu(data, skipBytes);

    return moveFlags;
}

CDataStore& operator>>(CDataStore& msg, CMovementStatus& move) {
    msg.Get(move.moveFlags);
    msg.Get(move.uint14);
    msg.Get(move.uint0);

    msg >> move.position28;
    msg.Get(move.facing34);

    if (move.moveFlags & 0x200) {
        // TODO
    } else {
        move.transport = 0;
        // TODO
    }

    if ((move.moveFlags & (0x200000 | 0x2000000)) || (move.uint14 & 0x20)) {
        msg.Get(move.float38);
    } else {
        move.float38 = 0.0f;
    }

    msg.Get(move.uint3C);

    if (move.moveFlags & 0x1000) {
        msg.Get(move.float40);
        msg.Get(move.float44);
        msg.Get(move.float48);
        msg.Get(move.float4C);
    }

    if (move.moveFlags & 0x4000000) {
        msg.Get(move.float50);
    }

    return msg;
}
