#include "event/Queue.hpp"
#include "event/EvtContext.hpp"
#include "event/EvtHandler.hpp"
#include <storm/Error.hpp>

void IEvtQueueDispatch(EvtContext* context, EVENTID id, const void* data) {
    STORM_ASSERT(context);

    // TODO
    // UpdateSyncState(data, &id, context, v3);

    // TODO
    // if (SErrIsDisplayingError()) {
    //     return;
    // }

    auto handlerList = &context->m_queueHandlerList[id];

    EvtHandler marker;
    marker.marker = 1;

    handlerList->LinkNode(&marker, 1, nullptr);

    EvtHandler* handler;

    while (1) {
        handler = marker.link.Next();

        if (!handler) {
            break;
        }

        handlerList->LinkNode(&marker, 1, marker.link.Next());

        if (!handler->marker && !handler->func(data, handler->param)) {
            break;
        }
    }

    handlerList->UnlinkNode(&marker);
}

void IEvtQueueDispatchAll(EvtContext* context) {
    // TODO
}

void IEvtQueueRegister(EvtContext* context, EVENTID id, int32_t (*handler)(const void*, void*), void* param, float priority) {
    STORM_ASSERT(context);

    auto handlerList = &context->m_queueHandlerList[id];

    auto m = SMemAlloc(sizeof(EvtHandler), __FILE__, __LINE__, 0x8);
    auto evtHandler = new (m) EvtHandler();

    evtHandler->priority = priority;
    evtHandler->param = param;
    evtHandler->func = handler;
    evtHandler->marker = 0;

    EvtHandler* h = handlerList->Head();

    while (h && (priority < h->priority || h->marker)) {
        h = handlerList->Link(h)->Next();
    }

    handlerList->LinkNode(evtHandler, 2, h);
}
