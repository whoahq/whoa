#include "object/client/CGUnit.hpp"
#include "object/client/CGObject.hpp"

uint32_t CGUnit::GetBaseOffset() {
    return CGObject::TotalFields();
}

uint32_t CGUnit::GetBaseOffsetSaved() {
    return CGObject::TotalFieldsSaved();
}

uint32_t CGUnit::GetDataSize() {
    return CGUnit::TotalFields() * sizeof(uint32_t);
}

uint32_t CGUnit::GetDataSizeSaved() {
    return CGUnit::TotalFieldsSaved() * sizeof(uint32_t);
}

uint32_t CGUnit::TotalFields() {
    return CGUnit::GetBaseOffset() + 142;
}

uint32_t CGUnit::TotalFieldsSaved() {
    return CGUnit::GetBaseOffsetSaved() + 123;
}

CGUnitData* CGUnit::Unit() const {
    return this->m_unit;
}
