#ifndef OBJECT_MOVEMENT_C_MOVEMENT_SHARED_HPP
#define OBJECT_MOVEMENT_C_MOVEMENT_SHARED_HPP

#include "object/movement/CPassenger.hpp"
#include "util/GUID.hpp"
#include <tempest/Vector.hpp>

struct CMoveSpline;

class CMovementShared : public CPassenger {
    public:
        // Public member functions
        CMovementShared(const WOWGUID& transportGUID, const C3Vector& position, float facing, const WOWGUID& guid);

    protected:
        // Protected member variables
        // TODO
        C3Vector m_anchorPosition;
        float m_anchorFacing;
        float m_anchorPitch;
        // TODO
        C3Vector m_direction;
        C2Vector m_direction2d;
        float m_cosAnchorPitch;
        float m_sinAnchorPitch;
        // TODO
        CMoveSpline* m_spline;
        // TODO
};

#endif
