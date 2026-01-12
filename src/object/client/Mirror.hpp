#ifndef OBJECT_CLIENT_MIRROR_HPP
#define OBJECT_CLIENT_MIRROR_HPP

#include <cstdint>

class CDataStore;
class CGObject_C;

int32_t FillInPartialObjectData(CGObject_C* object, uint64_t guid, CDataStore* msg, bool forFullUpdate, bool zeroZeroBits);

#endif
