#ifndef OBJECT_MOVEMENT_C_PASSENGER_HPP
#define OBJECT_MOVEMENT_C_PASSENGER_HPP

#include "util/GUID.hpp"
#include <tempest/Vector.hpp>

class CPassenger {
    public:
        // Public member functions
        CPassenger(const WOWGUID& transportGUID, const C3Vector& position, const WOWGUID& guid)
            : m_transportGUID(transportGUID)
            , m_position(position)
            , m_facing(0.0f)
            , m_guid(guid) {};
        C3Vector GetPosition() const;
        C3Vector GetPosition(const C3Vector& position) const;

    protected:
        // Protected member variables
        // TODO
        WOWGUID m_transportGUID;
        C3Vector m_position;
        // TODO
        float m_facing;
        // TODO
        const WOWGUID& m_guid;
        // TODO
};

#endif
