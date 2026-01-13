#include "object/client/CGObject_C.hpp"
#include "object/client/ObjMgr.hpp"

CGObject_C::CGObject_C(uint32_t time, CClientObjCreate& objCreate) {
    // TODO

    ClntObjMgrLinkInNewObject(this);

    // TODO
}

void CGObject_C::AddWorldObject() {
    // TODO
}

void CGObject_C::SetBlock(uint32_t block, uint32_t value) {
    auto storage = reinterpret_cast<uint32_t*>(this->m_obj);
    storage[block] = value;
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
