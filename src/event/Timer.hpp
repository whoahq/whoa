#ifndef EVENT_TIMER_HPP
#define EVENT_TIMER_HPP

#include <cstdint>

class EvtContext;

int32_t IEvtTimerDispatch(EvtContext* context);

uint32_t IEvtTimerGetNextTime(EvtContext* context, uint32_t currTime);

#endif
