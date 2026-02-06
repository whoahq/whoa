#ifndef OBJECT_CLIENT_MIRROR_HPP
#define OBJECT_CLIENT_MIRROR_HPP

#include "util/GUID.hpp"
#include <cstdint>

class CDataStore;
class CGObject_C;

int32_t CallMirrorHandlers(CDataStore* msg, bool a2, WOWGUID guid);

int32_t FillInPartialObjectData(CGObject_C* object, WOWGUID guid, CDataStore* msg, bool forFullUpdate, bool zeroZeroBits);

int32_t SkipPartialObjectUpdate(CDataStore* msg);

#endif
