#include "object/client/CGObject.hpp"

uint32_t CGObject::GetBaseOffset() {
    return 0;
}

uint32_t CGObject::GetBaseOffsetSaved() {
    return 0;
}

uint32_t CGObject::GetDataSize() {
    return CGObject::TotalFields() * sizeof(uint32_t);
}

uint32_t CGObject::GetDataSizeSaved() {
    return CGObject::TotalFieldsSaved() * sizeof(uint32_t);
}

uint32_t CGObject::TotalFields() {
    return CGObject::GetBaseOffset() + 6;
}

uint32_t CGObject::TotalFieldsSaved() {
    return CGObject::GetBaseOffsetSaved() + 3;
}

WOWGUID CGObject::GetGUID() const {
    return this->m_obj->m_guid;
}

OBJECT_TYPE CGObject::GetType() const {
    return this->m_obj->m_type;
}

OBJECT_TYPE_ID CGObject::GetTypeID() const {
    return this->m_typeID;
}

int32_t CGObject::IsA(OBJECT_TYPE type) const {
    return (this->GetType() & type) != 0;
}

int32_t CGObject::IsExactlyA(OBJECT_TYPE_ID typeID) const {
    return this->m_typeID == typeID;
}

CGObjectData* CGObject::Obj() const {
    return this->m_obj;
}
