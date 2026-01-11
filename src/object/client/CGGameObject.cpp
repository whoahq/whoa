#include "object/client/CGGameObject.hpp"
#include "object/client/CGObject.hpp"

uint32_t CGGameObject::GetBaseOffset() {
    return CGObject::TotalFields();
}

uint32_t CGGameObject::GetBaseOffsetSaved() {
    return CGObject::TotalFieldsSaved();
}

uint32_t CGGameObject::GetDataSize() {
    return CGGameObject::TotalFields() * sizeof(uint32_t);
}

uint32_t CGGameObject::GetDataSizeSaved() {
    return CGGameObject::TotalFieldsSaved() * sizeof(uint32_t);
}

uint32_t CGGameObject::TotalFields() {
    return CGGameObject::GetBaseOffset() + 12;
}

uint32_t CGGameObject::TotalFieldsSaved() {
    return CGGameObject::GetBaseOffsetSaved() + 4;
}
