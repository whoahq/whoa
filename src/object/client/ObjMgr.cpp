#include "object/client/ObjMgr.hpp"
#include "client/ClientServices.hpp"
#include "net/Connection.hpp"
#include <storm/Memory.hpp>

#if defined(WHOA_SYSTEM_WIN)
static thread_local ClntObjMgr* s_curMgr;
#else
static ClntObjMgr* s_curMgr;
#endif

static ClntObjMgr* s_savMgr;

ClntObjMgr* ClntObjMgrGetCurrent() {
    return s_curMgr;
}

uint32_t ClntObjMgrGetMapID() {
    if (!s_curMgr) {
        return 0;
    }

    return s_curMgr->m_mapID;
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

void ClntObjMgrSetHandlers() {
    // TODO
}
