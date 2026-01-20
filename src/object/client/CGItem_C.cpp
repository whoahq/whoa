#include "object/client/CGItem_C.hpp"

CGItem_C::CGItem_C(uint32_t time, CClientObjCreate& objCreate) : CGObject_C(time, objCreate) {
    // TODO
}

CGItem_C::~CGItem_C() {
    // TODO
}

void CGItem_C::PostInit(uint32_t time, const CClientObjCreate& init, bool a4) {
    this->CGObject_C::PostInit(time, init, a4);

    // TODO
}

void CGItem_C::SetStorage(uint32_t* storage, uint32_t* saved) {
    this->CGObject_C::SetStorage(storage, saved);

    this->m_item = reinterpret_cast<CGItemData*>(&storage[CGItem::GetBaseOffset()]);
    this->m_itemSaved = &saved[CGItem::GetBaseOffsetSaved()];
}
