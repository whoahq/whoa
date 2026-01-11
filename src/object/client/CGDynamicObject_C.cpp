#include "object/client/CGDynamicObject_C.hpp"

void CGDynamicObject_C::SetStorage(uint32_t* storage, uint32_t* saved) {
    this->CGObject_C::SetStorage(storage, saved);

    this->m_dynamicObj = reinterpret_cast<CGDynamicObjectData*>(&storage[CGDynamicObject::GetBaseOffset()]);
    this->m_dynamicObjSaved = &saved[CGDynamicObject::GetBaseOffsetSaved()];
}
