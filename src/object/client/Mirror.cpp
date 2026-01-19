#include "object/client/Mirror.hpp"
#include "object/client/CGContainer.hpp"
#include "object/client/CGCorpse.hpp"
#include "object/client/CGDynamicObject.hpp"
#include "object/client/CGGameObject.hpp"
#include "object/client/CGItem.hpp"
#include "object/client/CGObject_C.hpp"
#include "object/client/CGPlayer.hpp"
#include "object/client/CGUnit.hpp"
#include "object/client/ObjMgr.hpp"
#include "object/client/Util.hpp"
#include "object/Types.hpp"
#include <common/DataStore.hpp>

#define MAX_CHANGE_MASKS 42

static uint32_t s_objMirrorBlocks[] = {
    CGObject::TotalFields(),
    CGItem::TotalFields(),
    CGContainer::TotalFields(),
    CGUnit::TotalFields(),
    CGPlayer::TotalFields(),
    CGGameObject::TotalFields(),
    CGDynamicObject::TotalFields(),
    CGCorpse::TotalFields(),
};

/**
 * Given a message data store, extract the dirty change masks contained inside. Any masks not
 * present are zeroed out. This function assumes the provided masks pointer has enough space for
 * MAX_CHANGE_MASKS.
 */
int32_t ExtractDirtyMasks(CDataStore* msg, uint8_t* maskCount, uint32_t* masks) {
    uint8_t count;
    msg->Get(count);

    *maskCount = count;

    if (count > MAX_CHANGE_MASKS) {
        return 0;
    }

    for (int32_t i = 0; i < count; i++) {
        msg->Get(masks[i]);
    }

    // Zero out masks that aren't present
    memset(&masks[count], 0, (MAX_CHANGE_MASKS - count) * sizeof(uint32_t));

    return 1;
}

/**
 * Given an object type hierarchy and GUID, return the number of DWORD blocks backing the object's
 * data storage.
 */
uint32_t GetNumDwordBlocks(OBJECT_TYPE type, WOWGUID guid) {
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

int32_t IsMaskBitSet(uint32_t* masks, uint32_t block) {
    return masks[block / 32] & (1 << (block % 32));
}

int32_t CallMirrorHandlers(CDataStore* msg, bool a2, WOWGUID guid) {
    if (!a2) {
        SmartGUID _guid;
        *msg >> _guid;

        guid = _guid;
    }

    auto object = FindActiveObject(guid);

    if (!object) {
        return SkipPartialObjectUpdate(msg);
    }

    uint8_t changeMaskCount;
    uint32_t changeMasks[MAX_CHANGE_MASKS];
    if (!ExtractDirtyMasks(msg, &changeMaskCount, changeMasks)) {
        return 0;
    }

    OBJECT_TYPE_ID typeID = ID_OBJECT;
    uint32_t blockOffset = 0;
    uint32_t numBlocks = GetNumDwordBlocks(object->m_obj->m_type, guid);

    for (int32_t block = 0; block < numBlocks; block++) {
        if (block >= s_objMirrorBlocks[typeID]) {
            blockOffset = s_objMirrorBlocks[typeID];
            typeID = IncTypeID(object, typeID);
        }

        // TODO

        if (IsMaskBitSet(changeMasks, block)) {
            uint32_t blockValue = 0;
            msg->Get(blockValue);
        }

        // TODO
    }

    return 1;
}

int32_t FillInPartialObjectData(CGObject_C* object, WOWGUID guid, CDataStore* msg, bool forFullUpdate, bool zeroZeroBits) {
    uint8_t changeMaskCount;
    uint32_t changeMasks[MAX_CHANGE_MASKS];
    if (!ExtractDirtyMasks(msg, &changeMaskCount, changeMasks)) {
        return 0;
    }

    OBJECT_TYPE_ID typeID = ID_OBJECT;
    uint32_t blockOffset = 0;
    uint32_t numBlocks = GetNumDwordBlocks(object->m_obj->m_type, guid);

    for (int32_t block = 0; block < numBlocks; block++) {
        if (block >= s_objMirrorBlocks[typeID]) {
            blockOffset = s_objMirrorBlocks[typeID];
            typeID = IncTypeID(object, typeID);
        }

        if (!forFullUpdate) {
            // TODO
        }

        if (IsMaskBitSet(changeMasks, block)) {
            uint32_t blockValue;
            msg->GetArray(reinterpret_cast<uint8_t*>(&blockValue), sizeof(blockValue));

            object->SetBlock(block, blockValue);
        } else if (zeroZeroBits) {
            object->SetBlock(block, 0);
        }
    }

    // TODO

    return 1;
}
