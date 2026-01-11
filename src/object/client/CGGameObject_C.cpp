#include "object/client/CGGameObject_C.hpp"

void CGGameObject_C::SetStorage(uint32_t* storage, uint32_t* saved) {
    this->CGObject_C::SetStorage(storage, saved);

    this->m_gameObj = reinterpret_cast<CGGameObjectData*>(&storage[CGGameObject::GetBaseOffset()]);
    this->m_gameObjSaved = &saved[CGGameObject::GetBaseOffsetSaved()];
}
