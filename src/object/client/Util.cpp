#include "object/client/Util.hpp"
#include "object/client/ObjMgr.hpp"

CGObject_C* FindActiveObject(WOWGUID guid) {
    return ClntObjMgrGetCurrent()->m_objects.Ptr(guid, CHashKeyGUID(guid));
}

CGObject_C* GetUpdateObject(WOWGUID guid, int32_t* reenabled) {
    *reenabled = false;

    auto activeObject = FindActiveObject(guid);

    if (activeObject) {
        activeObject->SetDisablePending(false);

        return activeObject;
    }

    // TODO handle reenabling object
    return nullptr;
}
