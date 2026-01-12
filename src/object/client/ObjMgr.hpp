#ifndef OBJECT_CLIENT_OBJ_MGR_HPP
#define OBJECT_CLIENT_OBJ_MGR_HPP

#include "object/client/CGObject_C.hpp"
#include "object/client/ClntObjMgr.hpp"
#include "object/Types.hpp"
#include <cstdint>

void* ClntObjMgrAllocObject(OBJECT_TYPE_ID typeID, WOWGUID guid);

WOWGUID ClntObjMgrGetActivePlayer();

ClntObjMgr* ClntObjMgrGetCurrent();

uint32_t ClntObjMgrGetMapID();

void ClntObjMgrInitializeShared();

void ClntObjMgrInitializeStd(uint32_t mapID);

void ClntObjMgrPop();

void ClntObjMgrPush(ClntObjMgr* mgr);

void ClntObjMgrSetActivePlayer(WOWGUID guid);

void ClntObjMgrSetHandlers();

#endif
