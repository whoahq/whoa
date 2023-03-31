#ifndef EVENT_EVENT_HPP
#define EVENT_EVENT_HPP

#include "event/CEvent.hpp"
#include "event/Types.hpp"
#include <cstdint>
#include <storm/Array.hpp>
#include <storm/Atomic.hpp>
#include <storm/List.hpp>
#include <storm/Thread.hpp>

class EvtContext;
class EvtThread;

namespace Event {
    extern SEvent s_startEvent;
    extern SEvent s_shutdownEvent;
    extern int32_t s_netServer;
    extern int32_t s_originalThreadPriority;
    extern int32_t s_threadSlotCount;
    extern SCritSect* s_threadSlotCritsects;
    extern EvtThread** s_threadSlots;
    extern uint32_t s_mainThread;
    extern TSGrowableArray<SThread*> s_schedulerThreads;
    extern ATOMIC32 s_threadListContention;
    extern SCritSect s_threadListCritsect;
    extern TSList<EvtThread, TSGetLink<EvtThread>> s_threadList;
    extern EvtContext* s_currentEvtContext;
    extern ATOMIC32 s_interactiveCount;

    #if defined(WHOA_SYSTEM_MAC)
        extern bool s_shouldLoopTerminate;
    #endif
}

HEVENTCONTEXT EventCreateContextEx(int32_t interactive, int32_t (*initializeHandler)(const void*, void*), int32_t (*destroyHandler)(const void*, void*), uint32_t idleTime, uint32_t debugFlags);

void EventDoMessageLoop();

HEVENTCONTEXT EventGetCurrentContext();

void EventInitialize(int32_t threadCount, int32_t netServer);

void EventInitiateShutdown();

int32_t EventIsControlKeyDown();

int32_t EventIsKeyDown(KEY key);

int32_t EventIsShiftKeyDown();

void EventPostClose();

void EventPostCloseEx(HEVENTCONTEXT contextHandle);

void EventRegister(EVENTID id, int32_t (*handler)(const void*, void*));

void EventRegisterEx(EVENTID id, int32_t (*handler)(const void*, void*), void* param, float priority);

void OsNetPump(uint32_t timeout);

#endif
