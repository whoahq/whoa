#include "object/client/ObjMgr.hpp"
#include "object/client/ClntObjMgr.hpp"

#if defined(WHOA_SYSTEM_WIN)
static thread_local ClntObjMgr* s_curMgr;
#else
static ClntObjMgr* s_curMgr;
#endif

uint32_t ClntObjMgrGetMapID() {
    if (!s_curMgr) {
        return 0;
    }

    return s_curMgr->m_mapID;
}
