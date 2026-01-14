#ifndef OBJECT_CLIENT_UTIL_HPP
#define OBJECT_CLIENT_UTIL_HPP

#include "util/GUID.hpp"
#include <cstdint>

class CGObject_C;

CGObject_C* FindActiveObject(WOWGUID guid);

CGObject_C* GetUpdateObject(WOWGUID guid, int32_t* reenabled);

#endif
