#ifndef OBJECT_CLIENT_C_MOVEMENT_DATA_C_HPP
#define OBJECT_CLIENT_C_MOVEMENT_DATA_C_HPP

#include "object/movement/CMovementShared.hpp"

class CGUnit_C;

class CMovementData_C : public CMovementShared {
    public:
        // Public member functions
        CMovementData_C(const C3Vector& position, float facing, const WOWGUID& guid, CGUnit_C* unit);
};

#endif
