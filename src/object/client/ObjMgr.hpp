#ifndef OBJECT_CLIENT_OBJ_MGR_HPP
#define OBJECT_CLIENT_OBJ_MGR_HPP

#include "object/client/ClntObjMgr.hpp"
#include <cstdint>

uint32_t ClntObjMgrGetMapID();

void ClntObjMgrPush(ClntObjMgr* mgr);

#endif
