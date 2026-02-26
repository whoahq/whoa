#include "object/movement/CMovementShared.hpp"

CMovementShared::CMovementShared(const WOWGUID& transportGUID, const C3Vector& position, float facing, const WOWGUID& guid)
    : CPassenger(transportGUID, position, guid)
{
    this->m_facing = facing;

    this->m_anchorPosition = position;
    this->m_anchorFacing = facing;
    this->m_anchorPitch = 0.0f;
    this->m_cosAnchorPitch = 1.0f;
    this->m_sinAnchorPitch = 0.0f;

    this->m_spline = nullptr;
}
