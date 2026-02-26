#include "util/guid/SmartGUID.hpp"
#include <common/DataStore.hpp>

SmartGUID::operator WOWGUID() const {
    return this->guid;
}

SmartGUID& SmartGUID::operator=(WOWGUID guid) {
    this->guid = guid;
    return *this;
}

CDataStore& operator>>(CDataStore& msg, SmartGUID& guid) {
    guid = 0;

    uint8_t mask;
    msg.Get(mask);

    for (int32_t i = 0; i < 8; i++) {
        if (mask & (1 << i)) {
            uint8_t byte;
            msg.Get(byte);

            guid.guid |= static_cast<uint64_t>(byte) << (i * 8);
        }
    }

    return msg;
}
