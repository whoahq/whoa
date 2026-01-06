#include "object/client/ObjMgr.hpp"

#if defined(WHOA_SYSTEM_WIN)
static thread_local ClntObjMgr* s_curMgr;
#else
static ClntObjMgr* s_curMgr;
#endif

static ClntObjMgr* s_savMgr;

uint32_t ClntObjMgrGetMapID() {
    if (!s_curMgr) {
        return 0;
    }

    return s_curMgr->m_mapID;
}

void ClntObjMgrPush(ClntObjMgr* mgr) {
    if (s_savMgr || mgr == s_curMgr) {
        return;
    }

    s_savMgr = s_curMgr;
    s_curMgr = mgr;
}
