#include "object/client/CGItem_C.hpp"

void CGItem_C::SetStorage(uint32_t* storage, uint32_t* saved) {
    this->CGObject_C::SetStorage(storage, saved);

    this->m_item = reinterpret_cast<CGItemData*>(&storage[CGItem::GetBaseOffset()]);
    this->m_itemSaved = &saved[CGItem::GetBaseOffsetSaved()];
}
