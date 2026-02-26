#include "object/client/CMovementData_C.hpp"

CMovementData_C::CMovementData_C(const C3Vector& position, float facing, const WOWGUID& guid, CGUnit_C* unit)
    : CMovementShared(0, position, facing, guid)
{
    // TODO
}
