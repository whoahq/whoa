#include "object/movement/CPassenger.hpp"
#include <tempest/Matrix.hpp>

C3Vector CPassenger::GetPosition() const {
    return GetPosition(this->m_position);
}

C3Vector CPassenger::GetPosition(const C3Vector& position) const {
    // If on transport, transform position by transport matrix
    if (this->m_transportGUID) {
        C44Matrix transportMatrix;
        // MovementGetTransportMtxX(this->m_transportGUID, &transportMatrix);

        return position * transportMatrix;
    }

    return position;
}
