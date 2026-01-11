#ifndef OBJECT_CLIENT_UTILS_HPP
#define OBJECT_CLIENT_UTILS_HPP

#include "object/Types.hpp"
#include <cstdint>

class CGObject_C;

uint32_t GetNumDwordBlocks(OBJECT_TYPE type, uint64_t guid);

OBJECT_TYPE_ID IncTypeID(CGObject_C* object, OBJECT_TYPE_ID curTypeID);

#endif
