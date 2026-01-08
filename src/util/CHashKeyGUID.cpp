#include "util/CHashKeyGUID.hpp"

CHashKeyGUID::CHashKeyGUID() {
    this->m_guid = 0;
}

CHashKeyGUID::CHashKeyGUID(uint64_t guid) {
    this->m_guid = guid;
}
