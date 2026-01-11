#include "object/client/ObjMgr.hpp"
#include "client/ClientServices.hpp"
#include "console/Command.hpp"
#include "net/Connection.hpp"
#include "object/client/CGContainer_C.hpp"
#include "object/client/CGCorpse_C.hpp"
#include "object/client/CGDynamicObject_C.hpp"
#include "object/client/CGGameObject_C.hpp"
#include "object/client/CGItem_C.hpp"
#include "object/client/CGObject_C.hpp"
#include "object/client/CGPlayer_C.hpp"
#include "object/client/CGUnit_C.hpp"
#include "object/client/MessageHandlers.hpp"
#include "util/Unimplemented.hpp"
#include <common/ObjectAlloc.hpp>
#include <storm/Memory.hpp>

static bool s_heapsAllocated;
static uint32_t s_objHeapId[8];
static ClntObjMgr* s_savMgr;

#if defined(WHOA_SYSTEM_WIN)
static thread_local ClntObjMgr* s_curMgr;
#else
static ClntObjMgr* s_curMgr;
#endif

static uint32_t s_objTotalSize[] = {
    static_cast<uint32_t>(sizeof(CGObject_C)            + CGObject::GetDataSize()           + CGObject::GetDataSizeSaved()),
    static_cast<uint32_t>(sizeof(CGItem_C)              + CGItem::GetDataSize()             + CGItem::GetDataSizeSaved()),
    static_cast<uint32_t>(sizeof(CGContainer_C)         + CGContainer::GetDataSize()        + CGContainer::GetDataSizeSaved()),
    static_cast<uint32_t>(sizeof(CGUnit_C)              + CGUnit::GetDataSize()             + CGUnit::GetDataSizeSaved()),
    static_cast<uint32_t>(sizeof(CGPlayer_C)            + CGPlayer::GetRemoteDataSize()     + CGPlayer::GetRemoteDataSizeSaved()),
    static_cast<uint32_t>(sizeof(CGGameObject_C)        + CGGameObject::GetDataSize()       + CGGameObject::GetDataSizeSaved()),
    static_cast<uint32_t>(sizeof(CGDynamicObject_C)     + CGDynamicObject::GetDataSize()    + CGDynamicObject::GetDataSizeSaved()),
    static_cast<uint32_t>(sizeof(CGCorpse_C)            + CGCorpse::GetDataSize()           + CGCorpse::GetDataSizeSaved()),
};

static const char* s_objNames[] = {
    "CGObject_C",
    "CGItem_C",
    "CGContainer_C",
    "CGUnit_C",
    "CGPlayer_C",
    "CGGameObject_C",
    "CGDynamicObject_C",
    "CGCorpse_C",
};

static uint32_t s_heapSizes[] = {
    0,
    512,
    32,
    64,
    64,
    64,
    32,
    32,
};

int32_t CCommand_ObjUsage(const char* command, const char* arguments) {
    WHOA_UNIMPLEMENTED(0);
}

void MirrorInitialize() {
    // TODO
}

void* ClntObjMgrAllocObject(OBJECT_TYPE_ID typeID, uint64_t guid) {
    auto playerGUID = ClntObjMgrGetActivePlayer();

    // Heap allocate player object for current player
    if (guid == playerGUID) {
        return STORM_ALLOC(sizeof(CGPlayer_C) + CGPlayer::GetDataSize() + CGPlayer::GetDataSizeSaved());
    }

    // TODO GarbageCollect(typeID, 10000);

    uint32_t memHandle;
    void* mem;

    if (!ObjectAlloc(s_objHeapId[typeID], &memHandle, &mem, false)) {
        return nullptr;
    }

    // TODO pointer should be fetched via ObjectPtr
    static_cast<CGObject_C*>(mem)->m_memHandle = memHandle;

    return mem;
}

uint64_t ClntObjMgrGetActivePlayer() {
    if (!s_curMgr) {
        return 0;
    }

    return s_curMgr->m_activePlayer;
}

ClntObjMgr* ClntObjMgrGetCurrent() {
    return s_curMgr;
}

uint32_t ClntObjMgrGetMapID() {
    if (!s_curMgr) {
        return 0;
    }

    return s_curMgr->m_mapID;
}

void ClntObjMgrInitializeShared() {
    if (!s_heapsAllocated) {
        for (int32_t i = ID_ITEM; i < NUM_CLIENT_OBJECT_TYPES; i++) {
            s_objHeapId[i] = ObjectAllocAddHeap(s_objTotalSize[i], s_heapSizes[i], s_objNames[i], true);
        }

        s_heapsAllocated = true;
    }

    MirrorInitialize();

    ConsoleCommandRegister("ObjUsage", &CCommand_ObjUsage, GAME, nullptr);
}

void ClntObjMgrInitializeStd(uint32_t mapID) {
    // TODO last instance time

    auto mgr = STORM_NEW(ClntObjMgr);

    g_clientConnection->SetObjMgr(mgr);
    mgr->m_net = g_clientConnection;

    s_curMgr = mgr;

    ClntObjMgrSetHandlers();

    mgr->m_mapID = mapID;
}

void ClntObjMgrPop() {
    if (!s_savMgr) {
        return;
    }

    s_curMgr = s_savMgr;
    s_savMgr = nullptr;
}

void ClntObjMgrPush(ClntObjMgr* mgr) {
    if (s_savMgr || mgr == s_curMgr) {
        return;
    }

    s_savMgr = s_curMgr;
    s_curMgr = mgr;
}

void ClntObjMgrSetActivePlayer(uint64_t guid) {
    s_curMgr->m_activePlayer = guid;
}

void ClntObjMgrSetHandlers() {
    s_curMgr->m_net->SetMessageHandler(SMSG_UPDATE_OBJECT, &ObjectUpdateHandler, nullptr);
    s_curMgr->m_net->SetMessageHandler(SMSG_COMPRESSED_UPDATE_OBJECT, &ObjectCompressedUpdateHandler, nullptr);
    s_curMgr->m_net->SetMessageHandler(SMSG_DESTROY_OBJECT, &OnObjectDestroy, nullptr);
}
