#ifndef OBJECT_CLIENT_UTIL_HPP
#define OBJECT_CLIENT_UTIL_HPP

#include "object/Types.hpp"
#include "util/GUID.hpp"
#include <cstdint>

class CClientObjCreate;
class CGObject_C;

CGObject_C* FindActiveObject(WOWGUID guid);

CGObject_C* GetUpdateObject(WOWGUID guid, int32_t* reenable);

int32_t HandleObjectOutOfRangePass1(CGObject_C* object, OUT_OF_RANGE_TYPE type);

void InitObject(CGObject_C* object, uint32_t time, CClientObjCreate& objCreate);

#endif
