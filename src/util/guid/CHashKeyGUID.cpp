#include "util/guid/CHashKeyGUID.hpp"

CHashKeyGUID::CHashKeyGUID() {
    this->m_guid = 0;
}

CHashKeyGUID::CHashKeyGUID(WOWGUID guid) {
    this->m_guid = guid;
}
