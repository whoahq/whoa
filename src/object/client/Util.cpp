#include "object/client/Util.hpp"
#include "object/client/ObjMgr.hpp"

CGObject_C* FindActiveObject(WOWGUID guid) {
    return ClntObjMgrGetCurrent()->m_objects.Ptr(guid, CHashKeyGUID(guid));
}
