#include "object/client/CGItem.hpp"
#include "object/client/CGObject.hpp"

uint32_t CGItem::TotalFieldsSaved() {
    return CGObject::TotalFieldsSaved() + 47;
}
