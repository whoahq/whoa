#ifndef UTIL_GUID_C_HASH_KEY_GUID_HPP
#define UTIL_GUID_C_HASH_KEY_GUID_HPP

#include "util/guid/Types.hpp"
#include <cstdint>

class CHashKeyGUID {
    public:
        // Public member functions
        CHashKeyGUID();
        CHashKeyGUID(WOWGUID guid);

    private:
        // Private member variables
        WOWGUID m_guid;
};

#endif
