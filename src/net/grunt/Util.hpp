#ifndef NET_GRUNT_UTIL_HPP
#define NET_GRUNT_UTIL_HPP

#include <cstdint>

class CDataStore;

bool CanRead(const CDataStore& msg, uint32_t bytes);

#endif
