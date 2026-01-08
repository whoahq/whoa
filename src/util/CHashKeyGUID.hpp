#ifndef UTIL_C_HASH_KEY_GUID_HPP
#define UTIL_C_HASH_KEY_GUID_HPP

#include <cstdint>

class CHashKeyGUID {
    public:
        // Public member functions
        CHashKeyGUID();
        CHashKeyGUID(uint64_t guid);

    private:
        // Private member variables
        uint64_t m_guid;
};

#endif
