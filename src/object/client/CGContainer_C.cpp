#include "object/client/CGContainer_C.hpp"

CGContainer_C::CGContainer_C(uint32_t time, CClientObjCreate& objCreate) : CGItem_C(time, objCreate) {
    // TODO
}

CGContainer_C::~CGContainer_C() {
    // TODO
}

void CGContainer_C::SetStorage(uint32_t* storage, uint32_t* saved) {
    this->CGItem_C::SetStorage(storage, saved);

    this->m_cont = reinterpret_cast<CGContainerData*>(&storage[CGContainer::GetBaseOffset()]);
    this->m_contSaved = &saved[CGContainer::GetBaseOffsetSaved()];
}
