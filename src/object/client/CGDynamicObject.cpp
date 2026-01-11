#include "object/client/CGDynamicObject.hpp"
#include "object/client/CGObject.hpp"

uint32_t CGDynamicObject::GetBaseOffset() {
    return CGObject::TotalFields();
}

uint32_t CGDynamicObject::GetBaseOffsetSaved() {
    return CGObject::TotalFieldsSaved();
}

uint32_t CGDynamicObject::GetDataSize() {
    return CGDynamicObject::TotalFields() * sizeof(uint32_t);
}

uint32_t CGDynamicObject::GetDataSizeSaved() {
    return CGDynamicObject::TotalFieldsSaved() * sizeof(uint32_t);
}

uint32_t CGDynamicObject::TotalFields() {
    return CGDynamicObject::GetBaseOffset() + 6;
}

uint32_t CGDynamicObject::TotalFieldsSaved() {
    return CGDynamicObject::GetBaseOffsetSaved() + 0;
}
