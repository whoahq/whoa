#include "object/client/Util.hpp"
#include "object/client/CClientObjCreate.hpp"
#include "object/client/CGContainer_C.hpp"
#include "object/client/CGCorpse_C.hpp"
#include "object/client/CGDynamicObject_C.hpp"
#include "object/client/CGGameObject_C.hpp"
#include "object/client/CGItem_C.hpp"
#include "object/client/CGObject_C.hpp"
#include "object/client/CGPlayer_C.hpp"
#include "object/client/CGUnit_C.hpp"
#include "object/client/ObjMgr.hpp"

CGObject_C* FindActiveObject(WOWGUID guid) {
    return ClntObjMgrGetCurrent()->m_objects.Ptr(guid, CHashKeyGUID(guid));
}

CGObject_C* GetUpdateObject(WOWGUID guid, int32_t* reenabled) {
    *reenabled = false;

    // Active object

    auto activeObject = FindActiveObject(guid);

    if (activeObject) {
        activeObject->SetDisablePending(false);

        return activeObject;
    }

    // Disabled object

    auto disabledObject = ClntObjMgrGetCurrent()->m_lazyCleanupObjects.Ptr(guid, CHashKeyGUID(guid));

    if (disabledObject) {
        ClntObjMgrGetCurrent()->m_lazyCleanupObjects.Unlink(disabledObject);
        disabledObject->m_link.Unlink();

        ClntObjMgrGetCurrent()->m_objects.Insert(disabledObject, guid, CHashKeyGUID(guid));

        // These link checks are guaranteed to pass because of the unlink above (both lists share
        // the same link). This check is either from an inlined function or is cruft left behind
        // after a refactor.
        if (
            !ClntObjMgrGetCurrent()->m_visibleObjects.IsLinked(disabledObject)
            && !ClntObjMgrGetCurrent()->m_reenabledObjects.IsLinked(disabledObject)
        ) {
            *reenabled = true;
            ClntObjMgrGetCurrent()->m_reenabledObjects.LinkToTail(disabledObject);
        }

        return disabledObject;
    }

    // Object not found

    return nullptr;
}

void InitObject(CGObject_C* object, uint32_t time, CClientObjCreate& objCreate) {
    switch (object->m_typeID) {
        case ID_ITEM: {
            new (object) CGItem_C(time, objCreate);

            break;
        }

        case ID_CONTAINER: {
            new (object) CGContainer_C(time, objCreate);

            break;
        }

        case ID_UNIT: {
            new (object) CGUnit_C(time, objCreate);
            object->AddWorldObject();

            break;
        }

        case ID_PLAYER: {
            new (object) CGPlayer_C(time, objCreate);
            object->AddWorldObject();

            break;
        }

        case ID_GAMEOBJECT: {
            new (object) CGGameObject_C(time, objCreate);
            object->AddWorldObject();

            break;
        }

        case ID_DYNAMICOBJECT: {
            new (object) CGDynamicObject_C(time, objCreate);
            object->AddWorldObject();

            break;
        }

        case ID_CORPSE: {
            new (object) CGCorpse_C(time, objCreate);
            object->AddWorldObject();

            break;
        }

        default: {
            break;
        }
    }
}
