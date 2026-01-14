#include "object/client/MessageHandlers.hpp"
#include "object/client/CClientObjCreate.hpp"
#include "object/client/CGContainer_C.hpp"
#include "object/client/CGCorpse_C.hpp"
#include "object/client/CGDynamicObject_C.hpp"
#include "object/client/CGGameObject_C.hpp"
#include "object/client/CGItem_C.hpp"
#include "object/client/CGObject_C.hpp"
#include "object/client/CGPlayer_C.hpp"
#include "object/client/CGUnit_C.hpp"
#include "object/client/Mirror.hpp"
#include "object/client/ObjMgr.hpp"
#include "object/client/Util.hpp"
#include "util/GUID.hpp"
#include "util/Unimplemented.hpp"
#include "util/Zlib.hpp"
#include <common/DataStore.hpp>
#include <storm/Error.hpp>
#include <storm/Memory.hpp>
#include <new>

enum UPDATE_TYPE {
    UPDATE_PARTIAL      = 0,
    UPDATE_MOVEMENT     = 1,
    UPDATE_FULL         = 2,
    UPDATE_3            = 3,
    UPDATE_OUT_OF_RANGE = 4,
    UPDATE_IN_RANGE     = 5,
};

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

void UpdateOutOfRangeObjects(CDataStore* msg) {
    WHOA_UNIMPLEMENTED();
}

int32_t UpdateObject(CDataStore* msg) {
    WHOA_UNIMPLEMENTED(0);
}

void UpdateObjectMovement(CDataStore* msg) {
    WHOA_UNIMPLEMENTED();
}

void SetupObjectStorage(OBJECT_TYPE_ID typeID, CGObject_C* object, WOWGUID guid) {
    auto ptr = reinterpret_cast<char*>(object);

    switch (typeID) {
        case ID_OBJECT: {
            auto storage = reinterpret_cast<uint32_t*>(ptr + sizeof(CGObject_C));
            auto saved = storage + CGObject::TotalFields();

            object->SetStorage(storage, saved);
            memset(storage, 0, CGObject::GetDataSize());

            break;
        }

        case ID_ITEM: {
            auto storage = reinterpret_cast<uint32_t*>(ptr + sizeof(CGItem_C));
            auto saved = storage + CGItem::TotalFields();

            static_cast<CGItem_C*>(object)->SetStorage(storage, saved);
            memset(storage, 0, CGItem::GetDataSize());

            break;
        }

        case ID_CONTAINER: {
            auto storage = reinterpret_cast<uint32_t*>(ptr + sizeof(CGContainer_C));
            auto saved = storage + CGContainer::TotalFields();

            static_cast<CGContainer_C*>(object)->SetStorage(storage, saved);
            memset(storage, 0, CGContainer::GetDataSize());

            break;
        }

        case ID_UNIT: {
            auto storage = reinterpret_cast<uint32_t*>(ptr + sizeof(CGUnit_C));
            auto saved = storage + CGUnit::TotalFields();

            static_cast<CGUnit_C*>(object)->SetStorage(storage, saved);
            memset(storage, 0, CGUnit::GetDataSize());

            break;
        }

        case ID_PLAYER: {
            // TODO something at ptr + 0x614 (within CGPlayer_C)

            if (guid == ClntObjMgrGetActivePlayer()) {
                auto storage = reinterpret_cast<uint32_t*>(ptr + sizeof(CGPlayer_C));
                auto saved = storage + CGPlayer::TotalFields();

                static_cast<CGPlayer_C*>(object)->SetStorage(storage, saved);
                memset(storage, 0, CGPlayer::GetDataSize());
            } else {
                auto storage = reinterpret_cast<uint32_t*>(ptr + sizeof(CGPlayer_C));
                auto saved = storage + CGPlayer::TotalRemoteFields();

                static_cast<CGPlayer_C*>(object)->SetStorage(storage, saved);
                memset(storage, 0, CGPlayer::GetRemoteDataSize());
            }

            break;
        }

        case ID_GAMEOBJECT: {
            auto storage = reinterpret_cast<uint32_t*>(ptr + sizeof(CGGameObject_C));
            auto saved = storage + CGGameObject::TotalFields();

            static_cast<CGGameObject_C*>(object)->SetStorage(storage, saved);
            memset(storage, 0, CGGameObject::GetDataSize());

            break;
        }

        case ID_DYNAMICOBJECT: {
            auto storage = reinterpret_cast<uint32_t*>(ptr + sizeof(CGDynamicObject_C));
            auto saved = storage + CGDynamicObject::TotalFields();

            static_cast<CGDynamicObject_C*>(object)->SetStorage(storage, saved);
            memset(storage, 0, CGDynamicObject::GetDataSize());

            break;
        }

        case ID_CORPSE: {
            auto storage = reinterpret_cast<uint32_t*>(ptr + sizeof(CGCorpse_C));
            auto saved = storage + CGCorpse::TotalFields();

            static_cast<CGCorpse_C*>(object)->SetStorage(storage, saved);
            memset(storage, 0, CGCorpse::GetDataSize());

            break;
        }
    }
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

int32_t CreateObject(CDataStore* msg, uint32_t time) {
    SmartGUID guid;
    *msg >> guid;

    uint8_t _typeID;
    msg->Get(_typeID);
    auto typeID = static_cast<OBJECT_TYPE_ID>(_typeID);

    int32_t reenabled;
    auto existingObject = GetUpdateObject(guid, &reenabled);

    if (existingObject) {
        CClientObjCreate::Skip(msg);

        if (!FillInPartialObjectData(existingObject, existingObject->m_obj->m_guid, msg, false, true)) {
            return 0;
        }

        if (reenabled) {
            existingObject->Reenable();
        }

        return 1;
    }

    CClientObjCreate objCreate;
    if (!objCreate.Get(msg)) {
        return 0;
    }

    if (objCreate.flags & 0x1) {
        ClntObjMgrSetActivePlayer(guid);
    }

    auto newObject = static_cast<CGObject_C*>(ClntObjMgrAllocObject(typeID, guid));

    SetupObjectStorage(typeID, newObject, guid);

    newObject->SetTypeID(typeID);

    if (!FillInPartialObjectData(newObject, guid, msg, true, false)) {
        return 0;
    }

    InitObject(newObject, time, objCreate);

    // TODO link to one of the ClntObjMgr lists

    return 1;
}

void UpdateInRangeObjects(CDataStore* msg) {
    WHOA_UNIMPLEMENTED();
}

int32_t ObjectUpdateFirstPass(CDataStore* msg, uint32_t time, uint32_t updateIdx, uint32_t updateCount) {
    for (uint32_t i = updateIdx; i < updateCount; i++) {
        uint8_t updateType;
        msg->Get(updateType);

        switch (updateType) {
            case UPDATE_PARTIAL: {
                if (!UpdateObject(msg)) {
                    return 0;
                }

                break;
            }

            case UPDATE_MOVEMENT: {
                // Skipped in first pass

                SmartGUID guid;
                *msg >> guid;

                CClientMoveUpdate::Skip(msg);

                break;
            }

            case UPDATE_FULL:
            case UPDATE_3: {
                if (!CreateObject(msg, time)) {
                    return 0;
                }

                break;
            }

            case UPDATE_IN_RANGE: {
                UpdateInRangeObjects(msg);

                break;
            }

            default: {
                STORM_APP_FATAL("Unknown client update packet type (%d)!", updateType);
            }
        }
    }

    return 1;
}

int32_t ObjectUpdateSecondPass(CDataStore* msg, uint32_t time, uint32_t updateCount) {
    // TODO
    return 0;
}

int32_t ObjectCompressedUpdateHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    uint32_t origSize;
    msg->Get(origSize);

    uint32_t sourceSize = msg->Size() - msg->Tell();

    void* source;
    msg->GetDataInSitu(source, sourceSize);

    void* buffer;
    bool bufferOnStack;

    // Stack allocate buffer if original size is less than 8KB
    if (origSize >= 8192) {
        buffer = STORM_ALLOC(origSize);
        bufferOnStack = false;
    } else {
        buffer = alloca(origSize);
        bufferOnStack = true;
    }

    auto dest = buffer;
    auto destSize = origSize;

    auto zlibResult = ZlibDecompress(dest, &destSize, source, sourceSize);

    // Error during decompression

    if (zlibResult != 0) {
        if (!bufferOnStack) {
            STORM_FREE(buffer);
        }

        return 0;
    }

    // Successful decompression

    STORM_ASSERT(origSize == destSize);

    // TODO WDataStore
    CDataStore decompMsg;
    decompMsg.PutData(dest, destSize);
    decompMsg.Finalize();

    int32_t result = ObjectUpdateHandler(nullptr, SMSG_UPDATE_OBJECT, time, &decompMsg);

    if (!bufferOnStack) {
        STORM_FREE(buffer);
    }

    return result;
}

int32_t ObjectUpdateHandler(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    uint32_t updateCount;
    msg->Get(updateCount);

    auto startPos = msg->Tell();

    uint8_t firstUpdateType;
    msg->Get(firstUpdateType);

    uint32_t updateIdx = 0;

    if (firstUpdateType == UPDATE_OUT_OF_RANGE) {
        UpdateOutOfRangeObjects(msg);
        updateIdx = 1;
    } else {
        msg->Seek(startPos);
    }

    int32_t result = 0;

    if (ObjectUpdateFirstPass(msg, time, updateIdx, updateCount)) {
        msg->Seek(startPos);
        result = ObjectUpdateSecondPass(msg, time, updateCount);
    }

    // TODO

    return result;
}

int32_t OnObjectDestroy(void* param, NETMESSAGE msgId, uint32_t time, CDataStore* msg) {
    WHOA_UNIMPLEMENTED(0);
}
