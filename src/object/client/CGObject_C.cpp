#include "object/client/CGObject_C.hpp"
#include "object/client/ObjMgr.hpp"

CGObject_C::CGObject_C(uint32_t time, CClientObjCreate& objCreate) {
    // TODO

    this->m_lockCount = 0;
    this->m_disabled = false;
    this->m_inReenable = false;
    this->m_postInited = false;
    this->m_flag19 = false;
    this->m_disablePending = false;

    // TODO

    ClntObjMgrLinkInNewObject(this);

    // TODO
}

void CGObject_C::AddWorldObject() {
    // TODO
}

void CGObject_C::Disable() {
    this->m_disabled = true;
    // TODO other flag manipulation

    // TODO
}

int32_t CGObject_C::IsInReenable() {
    return this->m_inReenable;
}

int32_t CGObject_C::IsObjectLocked() {
    return this->m_lockCount != 0;
}

void CGObject_C::Reenable() {
    this->m_disabled = false;
    this->m_inReenable = true;

    // TODO
}

void CGObject_C::SetBlock(uint32_t block, uint32_t value) {
    auto storage = reinterpret_cast<uint32_t*>(this->m_obj);
    storage[block] = value;
}

void CGObject_C::SetDisablePending(int32_t pending) {
    if (pending) {
        this->m_disablePending = true;
    } else {
        this->m_disablePending = false;
    }
}

void CGObject_C::SetObjectLocked(int32_t locked) {
    if (locked) {
        if (this->m_lockCount != 0xFFFF) {
            this->m_lockCount++;
        }
    } else {
        if (this->m_lockCount != 0) {
            this->m_lockCount--;
        }
    }
}

void CGObject_C::SetStorage(uint32_t* storage, uint32_t* saved) {
    this->m_obj = reinterpret_cast<CGObjectData*>(&storage[CGObject::GetBaseOffset()]);
    this->m_objSaved = &saved[CGObject::GetBaseOffsetSaved()];
}

void CGObject_C::SetTypeID(OBJECT_TYPE_ID typeID) {
    this->m_typeID = typeID;

    switch (typeID) {
        case ID_OBJECT:
            this->m_obj->m_type = HIER_TYPE_OBJECT;
            break;

        case ID_ITEM:
            this->m_obj->m_type = HIER_TYPE_ITEM;
            break;

        case ID_CONTAINER:
            this->m_obj->m_type = HIER_TYPE_CONTAINER;
            break;

        case ID_UNIT:
            this->m_obj->m_type = HIER_TYPE_UNIT;
            break;

        case ID_PLAYER:
            this->m_obj->m_type = HIER_TYPE_PLAYER;
            break;

        case ID_GAMEOBJECT:
            this->m_obj->m_type = HIER_TYPE_GAMEOBJECT;
            break;

        case ID_DYNAMICOBJECT:
            this->m_obj->m_type = HIER_TYPE_DYNAMICOBJECT;
            break;

        case ID_CORPSE:
            this->m_obj->m_type = HIER_TYPE_CORPSE;
            break;

        default:
            break;
    }
}
