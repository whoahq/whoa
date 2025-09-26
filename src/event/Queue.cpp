#include "event/Queue.hpp"
#include "event/EvtContext.hpp"
#include "event/EvtHandler.hpp"
#include <storm/Error.hpp>

void UpdateSyncMouseState(EvtContext* context, MOUSEBUTTON button, int32_t down) {
    if (down) {
        context->m_queueSyncButtonState |= button;
    } else {
        context->m_queueSyncButtonState &= ~button;
    }
}

void UpdateSyncKeyState(EvtContext* context, KEY key, EVENTID& id) {
    context->m_critsect.Enter();

    int32_t keyDown = 0;

    auto list = &context->m_queueSyncKeyDownList;

    for (auto node = list->Head(); node;) {
        if (node->key == key) {
            keyDown = 1;
            auto dead = node;
            node = list->Next(node);
            list->UnlinkNode(dead);
            list->DeleteNode(dead);
        } else {
            node = list->Next(node);
        }
    }

    if (id == EVENT_ID_KEYDOWN) {
        if (keyDown) {
            id = EVENT_ID_KEYDOWN_REPEATING;
        }

        auto node = context->m_queueSyncKeyDownList.NewNode(2, 0, 0);
        node->key = key;
    }

    context->m_critsect.Leave();
}

void ResetSyncState(EvtContext* context) {
    context->m_queueSyncButtonState = 0;

    context->m_critsect.Enter();

    EvtKeyDown* node;

    auto list = &context->m_queueSyncKeyDownList;

    while (node = list->Head()) {
        list->UnlinkNode(node);
        list->DeleteNode(node);
    }

    context->m_critsect.Leave();
}

void UpdateSyncState(EvtContext* context, EVENTID& id, const void* data) {
    KEY key;
    MOUSEBUTTON button;

    STORM_ASSERT(context);

    switch (id) {
    case EVENT_ID_FOCUS:
        ResetSyncState(context);
        break;
    case EVENT_ID_KEYDOWN:
    case EVENT_ID_KEYUP:
        key = reinterpret_cast<const EVENT_DATA_KEY*>(data)->key;
        UpdateSyncKeyState(context, key, id);
        break;
    case EVENT_ID_MOUSEDOWN:
    case EVENT_ID_MOUSEUP:
        button = reinterpret_cast<const EVENT_DATA_MOUSE*>(data)->button;
        UpdateSyncMouseState(context, button, id == EVENT_ID_MOUSEDOWN);
        break;
    }
}

void IEvtQueueDispatch(EvtContext* context, EVENTID id, const void* data) {
    STORM_ASSERT(context);

    UpdateSyncState(context, id, data);

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

int32_t IEvtQueueCheckSyncKeyState(EvtContext* context, KEY key) {
    STORM_VALIDATE(context, ERROR_INVALID_PARAMETER, 0);

    context->m_critsect.Enter();

    int32_t keystate = 0;

    auto list = &context->m_queueSyncKeyDownList;

    for (auto node = list->Head(); node; node = list->Next(node)) {
        if (node->key == key) {
            keystate = 1;
            break;
        }
    }

    context->m_critsect.Leave();

    return keystate;
}
