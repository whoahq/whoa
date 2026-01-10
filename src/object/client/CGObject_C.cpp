#include "object/client/CGObject_C.hpp"

void CGObject_C::SetTypeID(OBJECT_TYPE_ID typeID) {
    this->m_typeID = typeID;

    switch (typeID) {
        case ID_OBJECT:
            this->m_obj->m_type = TYPE_OBJECT;
            break;

        case ID_ITEM:
            this->m_obj->m_type = static_cast<OBJECT_TYPE>(TYPE_OBJECT | TYPE_ITEM);
            break;

        case ID_CONTAINER:
            this->m_obj->m_type = static_cast<OBJECT_TYPE>(TYPE_OBJECT | TYPE_ITEM | TYPE_CONTAINER);
            break;

        case ID_UNIT:
            this->m_obj->m_type = static_cast<OBJECT_TYPE>(TYPE_OBJECT | TYPE_UNIT);
            break;

        case ID_PLAYER:
            this->m_obj->m_type = static_cast<OBJECT_TYPE>(TYPE_OBJECT | TYPE_UNIT | TYPE_PLAYER);
            break;

        case ID_GAMEOBJECT:
            this->m_obj->m_type = static_cast<OBJECT_TYPE>(TYPE_OBJECT | TYPE_GAMEOBJECT);
            break;

        case ID_DYNAMICOBJECT:
            this->m_obj->m_type = static_cast<OBJECT_TYPE>(TYPE_OBJECT | TYPE_DYNAMICOBJECT);
            break;

        case ID_CORPSE:
            this->m_obj->m_type = static_cast<OBJECT_TYPE>(TYPE_OBJECT | TYPE_CORPSE);
            break;

        default:
            break;
    }
}
