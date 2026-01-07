#ifndef OBJECT_CLIENT_OBJ_MGR_HPP
#define OBJECT_CLIENT_OBJ_MGR_HPP

#include "object/client/ClntObjMgr.hpp"
#include <cstdint>

ClntObjMgr* ClntObjMgrGetCurrent();

uint32_t ClntObjMgrGetMapID();

void ClntObjMgrInitializeStd(uint32_t mapID);

void ClntObjMgrPop();

void ClntObjMgrPush(ClntObjMgr* mgr);

void ClntObjMgrSetHandlers();

#endif
