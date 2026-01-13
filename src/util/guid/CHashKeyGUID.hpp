#ifndef UTIL_GUID_C_HASH_KEY_GUID_HPP
#define UTIL_GUID_C_HASH_KEY_GUID_HPP

#include "util/guid/Types.hpp"
#include <cstdint>

class CHashKeyGUID {
    public:
        // Public member functions
        CHashKeyGUID();
        CHashKeyGUID(WOWGUID guid);
        bool operator==(WOWGUID guid) const;
        bool operator==(const CHashKeyGUID& key) const;

    private:
        // Private member variables
        WOWGUID m_guid;
};

#endif
