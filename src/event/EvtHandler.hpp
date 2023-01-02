#ifndef EVENT_EVT_HANDLER_HPP
#define EVENT_EVT_HANDLER_HPP

#include <cstdint>
#include <storm/List.hpp>

class EvtHandler {
    public:
        // Member variables
        TSLink<EvtHandler> link;
        int32_t (*func)(const void*, void*);
        void* param;
        float priority;
        int32_t marker;
};

#endif
