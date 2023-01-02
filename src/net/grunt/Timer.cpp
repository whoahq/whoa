#include "net/grunt/Timer.hpp"
#include <common/Time.hpp>

uint32_t Grunt::Timer::ThreadProc(void* param) {
    auto timer = static_cast<Timer*>(param);

    while (true) {
        auto timeout = timer->Pump();

        if (timer->m_event.Wait(timeout) == 0) {
            break;
        }
    }

    return 1;
}

Grunt::Timer::Timer() {
    SThread::Create(Grunt::Timer::ThreadProc, this, this->m_thread, "GruntTimerEvt", 0);
}

void Grunt::Timer::Insert(Grunt::Timer::Event& newEvent) {
    this->m_critSect.Enter();

    newEvent.m_schedTime = OsGetAsyncTimeMsPrecise() + newEvent.m_interval;

    for (auto event = this->m_eventList.Head(); event; event = this->m_eventList.Link(event)->Next()) {
        // Keep event listed sorted by scheduled time
        if (newEvent.m_schedTime - event->m_schedTime < 0) {
            this->m_eventList.LinkNode(&newEvent, 2, event);

            this->m_critSect.Leave();

            return;
        }
    }

    this->m_eventList.LinkToTail(&newEvent);

    this->m_critSect.Leave();
}

uint32_t Grunt::Timer::Pump() {
    this->m_critSect.Enter();

    auto* event = this->m_eventList.Head();

    if (!event) {
        this->m_critSect.Leave();

        return 100;
    }

    int32_t timeUntilSched = event->m_schedTime - OsGetAsyncTimeMsPrecise();

    if (timeUntilSched <= 0) {
        this->m_eventList.UnlinkNode(event);

        this->m_critSect.Leave();

        event->Call();
        this->Insert(*event);

        return 100;
    }

    this->m_critSect.Leave();

    return timeUntilSched;
}
