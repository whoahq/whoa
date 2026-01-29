#include "client/ClientHandlers.hpp"
#include "Client.hpp"
#include "console/Console.hpp"
#include "db/Db.hpp"
#include "object/Client.hpp"
#include "util/Time.hpp"
#include "util/Unimplemented.hpp"
#include "world/World.hpp"
#include <common/DataStore.hpp>
#include <cstdint>
#include <tempest/Vector.hpp>

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

int32_t ReceiveGameTimeUpdate(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t ReceiveNewGameSpeed(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t ReceiveNewGameTime(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t ReceiveNewTimeSpeed(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    uint32_t encodedTime;
    msg->Get(encodedTime);

    float newSpeed;
    msg->Get(newSpeed);

    uint32_t holidayOffset;
    msg->Get(holidayOffset);

    if (!msg->IsRead()) {
        STORM_ASSERT(msg->IsFinal());
        // TODO ConsoleWriteA("Malformed message received: Id = %d, Len = %d, Read = %d\n", DEFAULT_COLOR, msgId, msg->Size(), msg->Tell());

        return 0;
    }

    WowTime newTime;
    WowTime::WowDecodeTime(encodedTime, &newTime);
    newTime.m_holidayOffset = holidayOffset;

    g_clientGameTime.GameTimeSetTime(newTime, true);

    // TODO UpdateTime();

    auto oldSpeed = g_clientGameTime.GameTimeSetMinutesPerSecond(newSpeed);

    char logStr[256];
    SStrPrintf(logStr, sizeof(logStr), "Gamespeed set from %.03f to %.03f", oldSpeed, newSpeed);
    ConsoleWrite(logStr, DEFAULT_COLOR);

    return 1;
}

int32_t ReceiveServerTime(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t TransferAbortedHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    // TODO

    return 0;
}

int32_t TransferPendingHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    // TODO

    return 0;
}
