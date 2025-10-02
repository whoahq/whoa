#ifndef EVENT_QUEUE_HPP
#define EVENT_QUEUE_HPP

#include "event/Types.hpp"

class EvtContext;

void IEvtQueueDispatch(EvtContext* context, EVENTID id, const void* data);

void IEvtQueueDispatchAll(EvtContext* context);

void IEvtQueueRegister(EvtContext* context, EVENTID id, int32_t (*handler)(const void*, void*), void* param, float priority);

int32_t IEvtQueueCheckSyncKeyState(EvtContext* context, KEY key);

#endif
