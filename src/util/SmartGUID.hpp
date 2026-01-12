#ifndef UTIL_SMART_GUID_HPP
#define UTIL_SMART_GUID_HPP

#include <cstdint>

class CDataStore;

struct SmartGUID {
    uint64_t guid;

    operator uint64_t() const;
    SmartGUID& operator=(uint64_t value);
};

CDataStore& operator>>(CDataStore& msg, SmartGUID& guid);

#endif
