#ifndef EVENT_EVT_KEY_DOWN_HPP
#define EVENT_EVT_KEY_DOWN_HPP

#include "event/Types.hpp"
#include <storm/List.hpp>

class EvtKeyDown {
    public:
        // Member variables
        TSLink<EvtKeyDown> link;
        KEY key;
};

#endif
