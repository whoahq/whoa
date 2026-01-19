#ifndef OBJECT_CLIENT_OBJ_MGR_HPP
#define OBJECT_CLIENT_OBJ_MGR_HPP

#include "object/client/CGObject_C.hpp"
#include "object/client/ClntObjMgr.hpp"
#include "object/Types.hpp"
#include <cstdint>

CGObject_C* ClntObjMgrAllocObject(OBJECT_TYPE_ID typeID, WOWGUID guid);

WOWGUID ClntObjMgrGetActivePlayer();

void ClntObjMgrFreeObject(CGObject_C* object);

ClntObjMgr* ClntObjMgrGetCurrent();

uint32_t ClntObjMgrGetMapID();

void ClntObjMgrInitializeShared();

void ClntObjMgrInitializeStd(uint32_t mapID);

void ClntObjMgrLinkInNewObject(CGObject_C* object);

CGObject_C* ClntObjMgrObjectPtr(WOWGUID guid, OBJECT_TYPE type, const char* fileName, int32_t lineNumber);

void ClntObjMgrPop();

void ClntObjMgrPush(ClntObjMgr* mgr);

void ClntObjMgrSetActivePlayer(WOWGUID guid);

void ClntObjMgrSetHandlers();

#endif
