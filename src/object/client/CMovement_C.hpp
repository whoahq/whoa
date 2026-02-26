#ifndef OBJECT_CLIENT_C_MOVEMENT_C_HPP
#define OBJECT_CLIENT_C_MOVEMENT_C_HPP

#include "object/client/CMovementData_C.hpp"
#include "util/GUID.hpp"
#include <tempest/Vector.hpp>

class CMovement_C : public CMovementData_C {
    public:
        // Public member functions
        CMovement_C(const C3Vector& position, float facing, const WOWGUID& guid, CGUnit_C* unit)
            : CMovementData_C(position, facing, guid, unit) {};
};

#endif
