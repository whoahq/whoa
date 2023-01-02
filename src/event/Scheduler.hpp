#ifndef EVENT_SCHEDULER_HPP
#define EVENT_SCHEDULER_HPP

#include "event/Types.hpp"
#include <cstdint>

void DestroySchedulerThread(uint32_t a1);

HEVENTCONTEXT IEvtSchedulerCreateContext(int32_t interactive, int32_t (*initializeHandler)(const void*, void*), int32_t (*destroyHandler)(const void*, void*), uint32_t idleTime, uint32_t debugFlags);

void IEvtSchedulerInitialize(int32_t threadCount, int32_t netServer);

void IEvtSchedulerProcess();

void IEvtSchedulerShutdown();

uint32_t InitializeSchedulerThread();

bool SchedulerMainProcess();

uint32_t SchedulerThreadProc(void* mainThread);

int32_t SchedulerThreadProcProcess(uint32_t a1);

#endif
