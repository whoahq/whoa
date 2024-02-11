#include "event/Scheduler.hpp"
#include "event/Context.hpp"
#include "event/Event.hpp"
#include "event/EvtContext.hpp"
#include "event/EvtThread.hpp"
#include "event/Input.hpp"
#include "event/Queue.hpp"
#include "event/Synthesize.hpp"
#include "event/Timer.hpp"
#include <algorithm>
#include <cstring>
#include <common/Call.hpp>
#include <common/Prop.hpp>
#include <common/Time.hpp>
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include <storm/Thread.hpp>

#if defined(WHOA_SYSTEM_MAC)
    #include "event/mac/Event.h"
#endif

void DestroySchedulerThread(uint32_t a1) {
    // TODO
}

HEVENTCONTEXT IEvtSchedulerCreateContext(int32_t interactive, int32_t (*initializeHandler)(const void*, void*), int32_t (*destroyHandler)(const void*, void*), uint32_t idleTime, uint32_t debugFlags) {
    if (idleTime < 1) {
        idleTime = 1;
    }

    char contextName[256];
    void* callContext = nullptr;

    if (debugFlags & 0x1) {
        SStrPrintf(contextName, 256, "Context: interactive = %u, idleTime = %u", interactive, idleTime);
        callContext = OsCallInitializeContext(contextName);
    }

    auto m = SMemAlloc(sizeof(EvtContext), __FILE__, __LINE__, 0x0);
    auto context = new (m) EvtContext(
        interactive != 0 ? 2 : 0,
        idleTime,
        interactive != 0 ? 1000 : 1,
        callContext,
        (debugFlags >> 1) & 1
    );

    if (interactive) {
        SInterlockedIncrement(&Event::s_interactiveCount);
    }

    if (initializeHandler) {
        IEvtQueueRegister(context, EVENT_ID_INITIALIZE, initializeHandler, 0, 1000.0);
    }

    if (destroyHandler) {
        IEvtQueueRegister(context, EVENT_ID_DESTROY, destroyHandler, 0, 1000.0);
    }

    return AttachContextToThread(context);
}

void IEvtSchedulerInitialize(int32_t threadCount, int32_t netServer) {
    if (Event::s_threadSlotCount) {
        // SErrDisplayAppFatal("IEvtScheduler already initialized");
    }

    Event::s_netServer = netServer;

    // TODO
    // Thread::s_originalThreadPriority = SGetCurrentThreadPriority();

    int32_t threadSlotCount = 1;

    while (threadSlotCount < threadCount) {
        threadSlotCount *= 2;
    }

    Event::s_threadSlotCount = threadSlotCount;

    // Allocate SCritSects for each thread slot
    int32_t v4 = sizeof(SCritSect) * threadSlotCount;

    auto slotMem = SMemAlloc((v4 + 4), __FILE__, __LINE__, 0x0);
    Event::s_threadSlotCritsects = new (slotMem) SCritSect[threadSlotCount];

    // Allocate EvtThread pointers for each thread slot
    Event::s_threadSlots = static_cast<EvtThread**>(SMemAlloc(sizeof(EvtThread*) * threadSlotCount, __FILE__, __LINE__, 0));
    memset(Event::s_threadSlots, 0, sizeof(EvtThread*) * threadSlotCount);

    Event::s_startEvent.Reset();
    Event::s_shutdownEvent.Reset();

    Event::s_mainThread = InitializeSchedulerThread();

    for (int32_t i = 0; i < threadCount - 1; ++i) {
        auto threadMem = SMemAlloc(sizeof(SThread), __FILE__, __LINE__, 0x0);
        auto thread = new (threadMem) SThread();

        Event::s_schedulerThreads.SetCount(Event::s_schedulerThreads.Count() + 1);

        Event::s_schedulerThreads[Event::s_schedulerThreads.Count() - 1] = thread;

        char threadName[16];
        SStrPrintf(threadName, 16, "EvtSched#%d", i);

        if (!SThread::Create(&SchedulerThreadProc, 0, *thread, threadName, 0)) {
            // TODO
        }
    }
}

void IEvtSchedulerProcess() {
    #if defined(WHOA_SYSTEM_WIN) || defined(WHOA_SYSTEM_LINUX)
        Event::s_startEvent.Set();

        SchedulerThreadProc(reinterpret_cast<void*>(1));

        Event::s_mainThread = 0;
    #endif

    #if defined(WHOA_SYSTEM_MAC)
        Event::s_startEvent.Set();

        if (OsInputIsUsingCocoaEventLoop()) {
            PropSelectContext(0);

            Event::s_startEvent.Wait(0xFFFFFFFF);

            uintptr_t v0 = SGetCurrentThreadId();
            char v2[64];
            SStrPrintf(v2, 64, "Engine %x", v0);

            OsCallInitialize(v2);

            RunCocoaEventLoop();

            DestroySchedulerThread(Event::s_mainThread);
            OsCallDestroy();

            Event::s_mainThread = 0;
        } else {
            // Legacy
            // sub_890180(1);
            // dword_141B3C8 = 0;
        }
    #endif
}

void IEvtSchedulerShutdown() {
    // TODO
}

uint32_t InitializeSchedulerThread() {
    SInterlockedIncrement(&Event::s_threadListContention);

    Event::s_threadListCritsect.Enter();

    uint32_t slot = Event::s_threadSlotCount;

    for (int32_t i = 0; i < Event::s_threadSlotCount; ++i) {
        if (slot == Event::s_threadSlotCount
            || Event::s_threadSlots[i] == nullptr
            || Event::s_threadSlots[i]->m_threadCount < Event::s_threadSlots[slot]->m_threadCount)
        {
            slot = i;

            if (!Event::s_threadSlots[i]) {
                break;
            }
        }
    }

    EvtThread* v4 = Event::s_threadSlots[slot];

    if (!v4) {
        v4 = Event::s_threadList.NewNode(1, 0, 0x8);

        v4->m_threadCount = 0;
        v4->m_weightTotal = 0;
        v4->m_weightAvg = 0;
        v4->m_contextCount = 0;
        v4->m_rebalance = 0;
        v4->m_threadSlot = slot;

        Event::s_threadSlotCritsects[slot].Enter();

        Event::s_threadSlots[slot] = v4;

        Event::s_threadSlotCritsects[slot].Leave();
    }

    ++v4->m_threadCount;

    Event::s_threadListCritsect.Leave();

    SInterlockedDecrement(&Event::s_threadListContention);

    return slot;
}

bool SchedulerMainProcess() {
    return SchedulerThreadProcProcess(Event::s_mainThread) != 0;
}

uint32_t SchedulerThreadProc(void* mainThread) {
    uint32_t v1;

    if (mainThread) {
        v1 = Event::s_mainThread;
    } else {
        v1 = InitializeSchedulerThread();
    }

    PropSelectContext(0);

    Event::s_startEvent.Wait(0xFFFFFFFF);

    uintptr_t v2 = SGetCurrentThreadId();
    char v4[64];
    SStrPrintf(v4, 64, "Engine %x", v2);

    OsCallInitialize(v4);

    while (!SchedulerThreadProcProcess(v1));

    DestroySchedulerThread(v1);
    OsCallDestroy();

    return 0;
}

int32_t SchedulerThreadProcProcess(uint32_t a1) {
    // TODO
    // if (SGetCurrentThreadPriority() != Event::s_originalThreadPriority) {
    //     SSetCurrentThreadPriority(Event::s_originalThreadPriority);
    // }

    if (!Event::s_shutdownEvent.Wait(0)) {
        return 1;
    }

    EvtContext* context = GetNextContext(a1);

    int32_t v11;

    if (context) {
        v11 = context->m_schedNextWakeTime.m_val - OsGetAsyncTimeMs();

        if (v11 < 0) {
            v11 = 0;
        }
    }

    uint32_t v14;

    if (Event::s_netServer) {
        if (v11 == -1) {
            v11 = 100;
        }

        OsNetPump(v11);

        v14 = 258;
    } else {
        v14 = Event::s_threadSlots[a1]->m_wakeEvent.Wait(v11);
    }

    if (!context) {
        return 0;
    }

    PropSelectContext(context->m_propContext);
    PropSet(PROP_EVENTCONTEXT, &context->m_id);
    OsCallSetContext(context->m_callContext);

    uint32_t currTime = OsGetAsyncTimeMs();
    uint32_t v19 = context->m_id;

    if (v14 == 258) {
        if (SynthesizeInitialize(context)) {
            if (context->m_startWatchdog) {
                // nullsub_5(20, 1);
                // *a2 = 1;
            }
        }

        uint32_t v9 = (currTime - context->m_schedLastIdle);
        context->m_schedLastIdle = currTime;
        double elapsedSec = v9 * 0.001;

        // TODO
        // FrameTime::Update(currTime, elapsedSec);

        IEvtTimerDispatch(context);

        if (context->m_schedFlags & 0x2) {
            int32_t shutdown = 0;
            IEvtInputProcess(context, &shutdown);

            if (shutdown) {
                context->m_critsect.Enter();

                if (context->m_schedState == EvtContext::SCHEDSTATE_ACTIVE) {
                    context->m_schedState = EvtContext::SCHEDSTATE_CLOSED;
                }

                context->m_critsect.Leave();

                IEvtSchedulerShutdown();
            }
        }

        SynthesizePoll(context);
        IEvtQueueDispatchAll(context);
        SynthesizeIdle(context, currTime, elapsedSec);
        SynthesizePaint(context);
    }

    if (a1 == Event::s_mainThread) {
        // TODO
        // dword_B417C4 = 0;
    }

    context->m_critsect.Enter();

    uint32_t closed = context->m_schedState == EvtContext::SCHEDSTATE_CLOSED;

    context->m_critsect.Leave();

    if (closed) {
        DetachContextFromThread(a1, context);
        SynthesizeDestroy(context);
        return 0;
    }

    uint32_t nextDelay;

    if (context->m_schedFlags & 0x4) {
        nextDelay = 0;
    } else {
        int32_t v15 = IEvtTimerGetNextTime(context, currTime);
        int32_t v16 = context->m_schedIdleTime;

        nextDelay = v15;

        if (v16 != context->m_schedInitialIdleTime) {
            nextDelay = context->m_schedIdleTime;
        }

        nextDelay = std::min(
            nextDelay,
            std::max((uint32_t)0, v16 + context->m_schedLastIdle - currTime)
        );
    }

    OsCallResetContext(context->m_callContext);
    PropSelectContext(nullptr);
    PutContext(nextDelay + currTime, context->m_schedSmoothWeight, context, a1);

    return 0;
}
