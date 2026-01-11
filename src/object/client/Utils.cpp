#include "object/client/Utils.hpp"
#include "object/client/CGContainer.hpp"
#include "object/client/CGCorpse.hpp"
#include "object/client/CGDynamicObject.hpp"
#include "object/client/CGGameObject.hpp"
#include "object/client/CGItem.hpp"
#include "object/client/CGObject_C.hpp"
#include "object/client/CGPlayer.hpp"
#include "object/client/CGUnit.hpp"
#include "object/client/ObjMgr.hpp"

/**
 * Given an object type hierarchy and GUID, return the number of DWORD blocks backing the object's
 * data storage.
 */
uint32_t GetNumDwordBlocks(OBJECT_TYPE type, uint64_t guid) {
    switch (type) {
        case HIER_TYPE_OBJECT:
            return CGObject::TotalFields();

        case HIER_TYPE_ITEM:
            return CGItem::TotalFields();

        case HIER_TYPE_CONTAINER:
            return CGContainer::TotalFields();

        case HIER_TYPE_UNIT:
            return CGUnit::TotalFields();

        case HIER_TYPE_PLAYER:
            return guid == ClntObjMgrGetActivePlayer() ? CGPlayer::TotalFields() : CGPlayer::TotalRemoteFields();

        case HIER_TYPE_GAMEOBJECT:
            return CGGameObject::TotalFields();

        case HIER_TYPE_DYNAMICOBJECT:
            return CGDynamicObject::TotalFields();

        case HIER_TYPE_CORPSE:
            return CGCorpse::TotalFields();

        default:
            return 0;
    }
}

/**
 * Accounting for the full hierarchy of the given object, return the next inherited type ID after
 * the given current type ID. If there is no next inherited type, return NUM_CLIENT_OBJECT_TYPES
 * to indicate the end of the hierarchy.
 */
OBJECT_TYPE_ID IncTypeID(CGObject_C* object, OBJECT_TYPE_ID curTypeID) {
    switch (object->m_obj->m_type) {
        // ID_OBJECT -> ID_ITEM -> ID_CONTAINER
        case HIER_TYPE_ITEM:
        case HIER_TYPE_CONTAINER:
            if (curTypeID == ID_OBJECT) {
                return ID_ITEM;
            }

            if (curTypeID == ID_ITEM) {
                return ID_CONTAINER;
            }

            return NUM_CLIENT_OBJECT_TYPES;

        // ID_OBJECT -> ID_UNIT -> ID_PLAYER
        case HIER_TYPE_UNIT:
        case HIER_TYPE_PLAYER:
            if (curTypeID == ID_OBJECT) {
                return ID_UNIT;
            }

            if (curTypeID == ID_UNIT) {
                return ID_PLAYER;
            }

            return NUM_CLIENT_OBJECT_TYPES;

        // ID_OBJECT -> ID_GAMEOBJECT
        case HIER_TYPE_GAMEOBJECT:
            if (curTypeID == ID_OBJECT) {
                return ID_GAMEOBJECT;
            }

            return NUM_CLIENT_OBJECT_TYPES;

        // ID_OBJECT -> ID_DYNAMICOBJECT
        case HIER_TYPE_DYNAMICOBJECT:
            if (curTypeID == ID_OBJECT) {
                return ID_DYNAMICOBJECT;
            }

            return NUM_CLIENT_OBJECT_TYPES;

        // ID_OBJECT -> ID_CORPSE
        case HIER_TYPE_CORPSE:
            if (curTypeID == ID_OBJECT) {
                return ID_CORPSE;
            }

            return NUM_CLIENT_OBJECT_TYPES;

        default:
            return NUM_CLIENT_OBJECT_TYPES;
    }
}
