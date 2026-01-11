#include "object/client/CGCorpse.hpp"
#include "object/client/CGObject.hpp"

uint32_t CGCorpse::GetBaseOffset() {
    return CGObject::TotalFields();
}

uint32_t CGCorpse::GetBaseOffsetSaved() {
    return CGObject::TotalFieldsSaved();
}

uint32_t CGCorpse::GetDataSize() {
    return CGCorpse::TotalFields() * sizeof(uint32_t);
}

uint32_t CGCorpse::GetDataSizeSaved() {
    return CGCorpse::TotalFieldsSaved() * sizeof(uint32_t);
}

uint32_t CGCorpse::TotalFields() {
    return CGCorpse::GetBaseOffset() + 30;
}

uint32_t CGCorpse::TotalFieldsSaved() {
    return CGCorpse::GetBaseOffsetSaved() + 3;
}
