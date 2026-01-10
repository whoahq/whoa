#include "object/client/CGContainer.hpp"
#include "object/client/CGItem.hpp"

uint32_t CGContainer::TotalFields() {
    return CGItem::TotalFields() + 74;
}

uint32_t CGContainer::TotalFieldsSaved() {
    return CGItem::TotalFieldsSaved() + 72;
}
