#include "object/client/CGCorpse_C.hpp"

CGCorpse_C::CGCorpse_C(uint32_t time, CClientObjCreate& objCreate) : CGObject_C(time, objCreate) {
    // TODO
}

CGCorpse_C::~CGCorpse_C() {
    // TODO
}

void CGCorpse_C::PostInit(uint32_t time, const CClientObjCreate& init, bool a4) {
    // TODO

    this->CGObject_C::PostInit(time, init, a4);

    // TODO
}

void CGCorpse_C::SetStorage(uint32_t* storage, uint32_t* saved) {
    this->CGObject_C::SetStorage(storage, saved);

    this->m_corpse = reinterpret_cast<CGCorpseData*>(&storage[CGCorpse::GetBaseOffset()]);
    this->m_corpseSaved = &saved[CGCorpse::GetBaseOffsetSaved()];
}
