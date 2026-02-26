#include "object/client/CGContainer.hpp"
#include "object/client/CGItem.hpp"

uint32_t CGContainer::GetBaseOffset() {
    return CGItem::TotalFields();
}

uint32_t CGContainer::GetBaseOffsetSaved() {
    return CGItem::TotalFieldsSaved();
}

uint32_t CGContainer::GetDataSize() {
    return CGContainer::TotalFields() * sizeof(uint32_t);
}

uint32_t CGContainer::GetDataSizeSaved() {
    return CGContainer::TotalFieldsSaved() * sizeof(uint32_t);
}

uint32_t CGContainer::TotalFields() {
    return CGContainer::GetBaseOffset() + 74;
}

uint32_t CGContainer::TotalFieldsSaved() {
    return CGContainer::GetBaseOffsetSaved() + 72;
}

CGContainerData* CGContainer::Container() const {
    return this->m_cont;
}
