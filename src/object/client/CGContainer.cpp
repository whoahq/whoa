#include "object/client/CGContainer.hpp"
#include "object/client/CGItem.hpp"

uint32_t CGContainer::TotalFieldsSaved() {
    return CGItem::TotalFieldsSaved() + 72;
}
