#include "object/movement/CPassenger.hpp"
#include <tempest/Matrix.hpp>

float CPassenger::GetFacing() const {
    return this->GetFacing(this->m_facing);
}

float CPassenger::GetFacing(float facing) const {
    // If on transport, transform facing by transport facing
    if (this->m_transportGUID) {
        float transportFacing = 0.0f;
        // TODO MovementGetTransportFacing(this->m_transportGUID);

        return facing + transportFacing;
    }

    return facing;
}

C3Vector CPassenger::GetPosition() const {
    return this->GetPosition(this->m_position);
}

C3Vector CPassenger::GetPosition(const C3Vector& position) const {
    // If on transport, transform position by transport matrix
    if (this->m_transportGUID) {
        C44Matrix transportMatrix;
        // TODO MovementGetTransportMtxX(this->m_transportGUID, &transportMatrix);

        return position * transportMatrix;
    }

    return position;
}
