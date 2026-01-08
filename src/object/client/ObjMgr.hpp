#ifndef OBJECT_CLIENT_OBJ_MGR_HPP
#define OBJECT_CLIENT_OBJ_MGR_HPP

#include "object/client/ClntObjMgr.hpp"
#include <cstdint>

uint64_t ClntObjMgrGetActivePlayer();

ClntObjMgr* ClntObjMgrGetCurrent();

uint32_t ClntObjMgrGetMapID();

void ClntObjMgrInitializeShared();

void ClntObjMgrInitializeStd(uint32_t mapID);

void ClntObjMgrPop();

void ClntObjMgrPush(ClntObjMgr* mgr);

void ClntObjMgrSetHandlers();

#endif
