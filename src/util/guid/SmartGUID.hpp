#ifndef UTIL_GUID_SMART_GUID_HPP
#define UTIL_GUID_SMART_GUID_HPP

#include "util/guid/Types.hpp"
#include <cstdint>

class CDataStore;

struct SmartGUID {
    WOWGUID guid;

    operator WOWGUID() const;
    SmartGUID& operator=(WOWGUID value);
};

CDataStore& operator>>(CDataStore& msg, SmartGUID& guid);

#endif
