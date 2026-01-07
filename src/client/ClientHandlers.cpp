#include "client/ClientHandlers.hpp"
#include "console/Console.hpp"
#include "db/Db.hpp"
#include "object/Client.hpp"
#include "world/World.hpp"
#include <common/DataStore.hpp>
#include <tempest/Vector.hpp>
#include <cstdint>

static float s_newFacing;
static C3Vector s_newPosition;
static uint32_t s_newZoneID;
static const char* s_newMapname;

void LoadNewWorld(const void* eventData, void* param) {
    // TODO

    ClntObjMgrInitializeStd(s_newZoneID);

    // TODO

    CWorld::LoadMap(s_newMapname, s_newPosition, s_newZoneID);

    // TODO
};

int32_t LoginVerifyWorldHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    uint32_t zoneID;
    msg->Get(zoneID);

    C3Vector position = { 0.0f, 0.0f, 0.0f };
    msg->Get(position.x);
    msg->Get(position.y);
    msg->Get(position.z);

    float facing;
    msg->Get(facing);

    if (zoneID == ClntObjMgrGetMapID()) {
        return 1;
    }

    s_newFacing = facing;
    s_newPosition = position;
    s_newZoneID = zoneID;

    auto map = g_mapDB.GetRecord(zoneID);

    if (!map) {
        ConsoleWrite("Bad SMSG_NEW_WORLD zoneID\n", DEFAULT_COLOR);

        return 0;
    }

    s_newMapname = map->m_directory;

    LoadNewWorld(nullptr, nullptr);

    return 1;
}

int32_t NewWorldHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    // TODO

    return 0;
}

int32_t NotifyHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    // TODO

    return 0;
}

int32_t PlayedTimeHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    // TODO

    return 0;
}

int32_t TransferAbortedHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    // TODO

    return 0;
}

int32_t TransferPendingHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    // TODO

    return 0;
}
