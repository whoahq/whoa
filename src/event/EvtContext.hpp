#ifndef EVENT_EVT_CONTEXT_HPP
#define EVENT_EVT_CONTEXT_HPP

#include "event/EvtHandler.hpp"
#include "event/EvtKeyDown.hpp"
#include "event/EvtMessage.hpp"
#include "event/EvtTimer.hpp"
#include "event/Types.hpp"
#include <cstdint>
#include <common/Instance.hpp>
#include <common/Prop.hpp>
#include <storm/Queue.hpp>
#include <storm/Thread.hpp>

class EvtContext : public TSingletonInstanceId<EvtContext, offsetof(TInstanceId<EvtContext>, m_id)> {
    public:
        // Types
        enum SCHEDSTATE {
            SCHEDSTATE_ACTIVE = 0x0,
            SCHEDSTATE_CLOSED = 0x1,
            SCHEDSTATE_DESTROYED = 0x2,
            _UNIQUE_SYMBOL_SCHEDSTATE_96 = 0xFFFFFFFF,
        };

        // Member variables
        SCritSect m_critsect;
        uint32_t m_currTime;
        SCHEDSTATE m_schedState;
        TSTimerPriority<uint32_t> m_schedNextWakeTime;
        uint32_t m_schedLastIdle;
        uint32_t m_schedFlags;
        uint32_t m_schedIdleTime;
        uint32_t m_schedInitialIdleTime;
        uint32_t m_schedWeight;
        uint32_t m_schedSmoothWeight;
        int32_t m_schedRebalance;
        TSExplicitList<EvtHandler, offsetof(EvtHandler, link)> m_queueHandlerList[EVENTIDS];
        TSExplicitList<EvtMessage, offsetof(EvtMessage, link)> m_queueMessageList;
        uint32_t m_queueSyncButtonState;
        TSExplicitList<EvtKeyDown, offsetof(EvtKeyDown, link)> m_queueSyncKeyDownList;
        // TODO
        // EvtIdTable<EvtTimer*> m_timerIdTable;
        EvtTimerQueue m_timerQueue;
        HPROPCONTEXT m_propContext;
        void* m_callContext;
        uint32_t m_startWatchdog;

        // Member functions
        EvtContext(uint32_t, uint32_t, uint32_t, void*, int32_t);
};

class EvtContextQueue : public TSPriorityQueue<EvtContext> {
    public:
        EvtContextQueue();
};

#endif
