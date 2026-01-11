#include "object/client/Utils.hpp"

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
