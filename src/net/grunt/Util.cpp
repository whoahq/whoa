#include "net/grunt/Util.hpp"
#include <common/DataStore.hpp>

// Invented name. Appears to be private to Grunt. Found at 8CBBF0 in 3.3.5a (Windows).
bool CanRead(const CDataStore& msg, uint32_t bytes) {
    return msg.IsValid() && msg.Size() - msg.Tell() >= bytes;
}
