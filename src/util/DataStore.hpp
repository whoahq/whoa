#ifndef UTIL_DATA_STORE_HPP
#define UTIL_DATA_STORE_HPP

#include <common/DataStore.hpp>
#include <tempest/Vector.hpp>

CDataStore& operator>>(CDataStore& msg, C3Vector& vector);

#endif
