#include "event/Event.hpp"
#include "event/EvtContext.hpp"
#include "event/EvtThread.hpp"
#include "event/Input.hpp"
#include "event/Queue.hpp"
#include "event/Scheduler.hpp"
#include <common/Prop.hpp>
#include <common/Time.hpp>

SEvent Event::s_startEvent = SEvent(1, 0);
SEvent Event::s_shutdownEvent = SEvent(1, 0);
int32_t Event::s_netServer;
int32_t Event::s_threadSlotCount;
SCritSect* Event::s_threadSlotCritsects;
EvtThread** Event::s_threadSlots;
uint32_t Event::s_mainThread;
TSGrowableArray<SThread*> Event::s_schedulerThreads;
ATOMIC32 Event::s_threadListContention { -1 };
SCritSect Event::s_threadListCritsect;
TSList<EvtThread, TSGetLink<EvtThread>> Event::s_threadList;
EvtContext* Event::s_currentEvtContext;
ATOMIC32 Event::s_interactiveCount;

#if defined(WHOA_SYSTEM_MAC)
    bool Event::s_shouldLoopTerminate;
#endif

void OsNetPump(uint32_t timeout) {
    // TODO
}

void EventInitialize(int32_t threadCount, int32_t netServer) {
    IEvtInputInitialize();

    int32_t v2 = threadCount;

    if (threadCount < 1) {
        v2 = 1;
    }

    IEvtSchedulerInitialize(v2, netServer);

    // TODO
    // OsInputSetEventPollProc(&sub_47DCA0);
}

void EventInitiateShutdown() {
    IEvtSchedulerShutdown();
}

int32_t EventIsControlKeyDown() {
    return EventIsKeyDown(KEY_LCONTROL) || EventIsKeyDown(KEY_RCONTROL);
}

int32_t EventIsKeyDown(KEY key) {
    auto hContext = PropGet(PROP_EVENTCONTEXT);
    auto contextId = *reinterpret_cast<uint32_t*>(hContext);

    int32_t findMask;
    auto context = TSingletonInstanceId<EvtContext, offsetof(EvtContext, m_id)>::s_idTable.Ptr(
        contextId,
        0,
        &findMask
    );

    if (!context) {
        return 0;
    }

    auto keystate = IEvtQueueCheckSyncKeyState(context, key);

    if (findMask != -1) {
        TSingletonInstanceId<EvtContext, offsetof(EvtContext, m_id)>::s_idTable.Unlock(
            findMask & (INSTANCE_TABLE_SLOT_COUNT - 1),
            findMask >= INSTANCE_TABLE_SLOT_COUNT
        );
    }

    return keystate;
}

int32_t EventIsShiftKeyDown() {
    return EventIsKeyDown(KEY_LSHIFT) || EventIsKeyDown(KEY_RSHIFT);
}

HEVENTCONTEXT EventCreateContextEx(int32_t interactive, int32_t (*initializeHandler)(const void*, void*), int32_t (*destroyHandler)(const void*, void*), uint32_t idleTime, uint32_t debugFlags) {
    return IEvtSchedulerCreateContext(interactive, initializeHandler, destroyHandler, idleTime, debugFlags);
}

void EventDoMessageLoop() {
    IEvtSchedulerProcess();
}

HEVENTCONTEXT EventGetCurrentContext() {
    return PropGet(PROP_EVENTCONTEXT);
}

void EventPostClose() {
    EventPostCloseEx(nullptr);
}

void EventPostCloseEx(HEVENTCONTEXT contextHandle) {
    if (!contextHandle) {
        contextHandle = PropGet(PROP_EVENTCONTEXT);
    }

    if (contextHandle) {
        uint32_t contextId = *reinterpret_cast<uint32_t*>(contextHandle);
        int32_t findMask;
        EvtContext* context = TSingletonInstanceId<EvtContext, offsetof(EvtContext, m_id)>::s_idTable.Ptr(
            contextId,
            0,
            &findMask
        );

        if (context) {
            context->m_critsect.Enter();

            if (context->m_schedState == EvtContext::SCHEDSTATE_ACTIVE) {
                context->m_schedState = EvtContext::SCHEDSTATE_CLOSED;
            }

            context->m_critsect.Leave();

            if (findMask != -1) {
                TSingletonInstanceId<EvtContext, offsetof(EvtContext, m_id)>::s_idTable.Unlock(
                    findMask & (INSTANCE_TABLE_SLOT_COUNT - 1),
                    findMask >= INSTANCE_TABLE_SLOT_COUNT
                );
            }
        }
    }
}

void EventRegister(EVENTID id, EVENTHANDLERFUNC handler) {
    EventRegisterEx(id, handler, nullptr, 0.0f);
}

void EventRegisterEx(EVENTID id, EVENTHANDLERFUNC handler, void* param, float priority) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(id >= 0);
    STORM_VALIDATE(id < EVENTIDS);
    STORM_VALIDATE(handler);
    STORM_VALIDATE_END_VOID;

    HEVENTCONTEXT hContext = PropGet(PROP_EVENTCONTEXT);

    uint32_t contextId = *reinterpret_cast<uint32_t*>(hContext);
    int32_t findMask;

    EvtContext* context = TSingletonInstanceId<EvtContext, offsetof(EvtContext, m_id)>::s_idTable.Ptr(
        contextId,
        0,
        &findMask
    );

    if (context) {
        IEvtQueueRegister(context, id, handler, param, priority);

        if (findMask != -1) {
            TSingletonInstanceId<EvtContext, offsetof(EvtContext, m_id)>::s_idTable.Unlock(
                findMask & (INSTANCE_TABLE_SLOT_COUNT - 1),
                findMask >= INSTANCE_TABLE_SLOT_COUNT
            );
        }
    }
}

void EventUnregisterEx(EVENTID id, EVENTHANDLERFUNC handler, void* param, uint32_t flags) {
    // TODO
}
