#include "object/client/CGDynamicObject_C.hpp"

CGDynamicObject_C::CGDynamicObject_C(uint32_t time, CClientObjCreate& objCreate) : CGObject_C(time, objCreate) {
    // TODO
}

CGDynamicObject_C::~CGDynamicObject_C() {
    // TODO
}

void CGDynamicObject_C::SetStorage(uint32_t* storage, uint32_t* saved) {
    this->CGObject_C::SetStorage(storage, saved);

    this->m_dynamicObj = reinterpret_cast<CGDynamicObjectData*>(&storage[CGDynamicObject::GetBaseOffset()]);
    this->m_dynamicObjSaved = &saved[CGDynamicObject::GetBaseOffsetSaved()];
}
