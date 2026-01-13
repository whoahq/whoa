#include "object/client/CGGameObject_C.hpp"

CGGameObject_C::CGGameObject_C(uint32_t time, CClientObjCreate& objCreate) : CGObject_C(time, objCreate) {
    // TODO
}

void CGGameObject_C::SetStorage(uint32_t* storage, uint32_t* saved) {
    this->CGObject_C::SetStorage(storage, saved);

    this->m_gameObj = reinterpret_cast<CGGameObjectData*>(&storage[CGGameObject::GetBaseOffset()]);
    this->m_gameObjSaved = &saved[CGGameObject::GetBaseOffsetSaved()];
}
