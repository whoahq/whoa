#include "event/Timer.hpp"
#include "event/EvtContext.hpp"
#include "event/EvtTimer.hpp"
#include <storm/Error.hpp>

int32_t IEvtTimerDispatch(EvtContext* context) {
    // TODO

    return 0;
}

uint32_t IEvtTimerGetNextTime(EvtContext* context, uint32_t currTime) {
    STORM_ASSERT(context);

    context->m_critsect.Enter();

    uint32_t nextTime = -1;

    if (context->m_timerQueue.Count()) {
        auto queue = static_cast<EvtTimer*>(context->m_timerQueue[0]);
        auto targetTime = queue->targetTime.m_val;
        nextTime = targetTime - currTime;
        nextTime = nextTime < 0 ? 0 : nextTime;
    }

    context->m_critsect.Leave();

    return nextTime;
}
