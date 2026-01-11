#include "object/client/CGCorpse_C.hpp"

void CGCorpse_C::SetStorage(uint32_t* storage, uint32_t* saved) {
    this->CGObject_C::SetStorage(storage, saved);

    this->m_corpse = reinterpret_cast<CGCorpseData*>(&storage[CGCorpse::GetBaseOffset()]);
    this->m_corpseSaved = &saved[CGCorpse::GetBaseOffsetSaved()];
}
