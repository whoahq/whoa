#include "event/Context.hpp"
#include "event/Event.hpp"
#include "event/EvtThread.hpp"
#include <common/Time.hpp>
#include <storm/Atomic.hpp>

HEVENTCONTEXT AttachContextToThread(EvtContext* context) {
    SInterlockedIncrement(&Event::s_threadListContention);
    Event::s_threadListCritsect.Enter();

    // Select the thread with the highest weight total
    EvtThread* thread = nullptr;
    EvtThread* t = Event::s_threadList.Head();

    while (t) {
        if (!thread || t->m_weightTotal < thread->m_weightTotal) {
            thread = t;
        }

        t = t->Next();
    }

    if (thread) {
        TSingletonInstanceId<EvtContext, offsetof(EvtContext, m_id)>::s_idTable.Insert(context);

        uint32_t v13 = OsGetAsyncTimeMs();

        if (v13 != context->m_schedNextWakeTime.m_val) {
            context->m_schedNextWakeTime.m_val = v13;
            context->m_schedNextWakeTime.Relink();
        }

        Event::s_threadSlotCritsects[thread->m_threadSlot].Enter();

        thread->m_contextQueue.Enqueue(context);

        Event::s_threadSlotCritsects[thread->m_threadSlot].Leave();

        thread->m_wakeEvent.Set();

        uint32_t v14 = context->m_schedWeight + thread->m_weightTotal;
        uint32_t v15 = thread->m_contextCount + 1;
        thread->m_contextCount = v15;
        thread->m_weightTotal = v14;
        thread->m_weightAvg = v14 / v15;
    } else {
        // TODO
    }

    Event::s_threadListCritsect.Leave();
    SInterlockedDecrement(&Event::s_threadListContention);

    return context;
}

void DetachContextFromThread(uint32_t a1, EvtContext* a2) {
    // TODO
}

EvtContext* GetNextContext(uint32_t hThread) {
    EvtContext* context;

    Event::s_threadSlotCritsects[hThread].Enter();

    context = Event::s_threadSlots[hThread]->m_contextQueue.Dequeue();

    Event::s_threadSlotCritsects[hThread].Leave();

    if (hThread == Event::s_mainThread) {
        Event::s_currentEvtContext = context;
    }

    return context;
}

void PutContext(uint32_t nextWakeTime, uint32_t newSmoothWeight, EvtContext* context, uint32_t hThread) {
    if (nextWakeTime != context->m_schedNextWakeTime.m_val) {
        context->m_schedNextWakeTime.m_val = nextWakeTime;
        context->m_schedNextWakeTime.Relink();
    }

    if (context->m_schedSmoothWeight != newSmoothWeight) {
        uint32_t v8 = context->m_schedWeight;
        context->m_schedSmoothWeight = newSmoothWeight;

        int32_t v9;

        if (newSmoothWeight <= v8) {
            v9 = v8 - newSmoothWeight;
        } else {
            v9 = newSmoothWeight - v8;
        }

        context->m_schedRebalance = v9 >= v8 >> 3;
    }

    if (!SInterlockedIncrement(&Event::s_threadListContention)) {
        Event::s_threadListCritsect.Enter();

        // TODO

        Event::s_threadListCritsect.Leave();
    }

    SInterlockedDecrement(&Event::s_threadListContention);

    if (hThread < Event::s_threadSlotCount) {
        Event::s_threadSlotCritsects[hThread].Enter();
        Event::s_threadSlots[hThread]->m_contextQueue.Enqueue(context);
        Event::s_threadSlotCritsects[hThread].Leave();
    }
}
