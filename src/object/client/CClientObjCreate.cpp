#include "object/client/CClientObjCreate.hpp"
#include "util/DataStore.hpp"
#include "util/GUID.hpp"
#include "util/Unimplemented.hpp"
#include <common/DataStore.hpp>

int32_t CClientObjCreate::Get(CDataStore* msg) {
    uint16_t flags;
    msg->Get(flags);

    this->flags = flags;

    if (this->flags & 0x20) {
        *msg >> this->move;
    } else if (this->flags & 0x100) {
        SmartGUID guid;
        *msg >> guid;
        this->move.status.transport = guid;

        *msg >> this->move.status.position28;
        *msg >> this->move.status.position18;

        msg->Get(this->move.status.facing34);
        msg->Get(this->move.status.facing24);
    } else if (this->flags & 0x40) {
        this->move.status.transport = 0;

        *msg >> this->move.status.position28;
        this->move.status.position18 = this->move.status.position28;

        msg->Get(this->move.status.facing34);
    }

    if (this->flags & 0x8) {
        msg->Get(this->uint2AC);
    } else {
        this->uint2AC = 0;
    }

    if (this->flags & 0x10) {
        msg->Get(this->uint2B0);
    } else {
        this->uint2B0 = 0;
    }

    if (this->flags & 0x4) {
        SmartGUID guid;
        *msg >> guid;
        this->guid2B8 = guid;
    } else {
        this->guid2B8 = 0;
    }

    if (this->flags & 0x2) {
        msg->Get(this->uint2C0);
    }

    if (this->flags & 0x80) {
        msg->Get(this->uint2C4);
        msg->Get(this->float2C8);
    }

    if (this->flags & 0x200) {
        SmartGUID guid;
        *msg >> guid;
        this->guid2D4 = guid;
    } else {
        this->guid2D4 = 0;
    }

    return msg->Size() >= msg->Tell();
}
