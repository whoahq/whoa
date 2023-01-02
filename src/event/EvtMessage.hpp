#ifndef EVENT_EVT_MESSAGE_HPP
#define EVENT_EVT_MESSAGE_HPP

#include "event/Types.hpp"
#include <common/Instance.hpp>
#include <storm/List.hpp>

class EvtMessage : public TExtraInstanceRecyclable<EvtMessage> {
    public:
        // Member variables
        TSLink<EvtMessage> link;
        EVENTID id;
        char data[4];
};

#endif
