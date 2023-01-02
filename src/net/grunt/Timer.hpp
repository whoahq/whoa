#ifndef NET_GRUNT_TIMER_HPP
#define NET_GRUNT_TIMER_HPP

#include "net/grunt/Grunt.hpp"
#include "storm/List.hpp"
#include "storm/Thread.hpp"

class Grunt::Timer {
    public:
        // Types
        class Event {
            public:
                // Member variables
                TSLink<Event> m_link;
                uint32_t m_schedTime;
                uint32_t m_interval;

                // Virtual methods
                virtual void Call() = 0;
        };

        // Static functions
        static uint32_t ThreadProc(void* param);

        // Member variables
        SEvent m_event = SEvent(0, 0);
        SThread m_thread;
        SCritSect m_critSect;
        STORM_EXPLICIT_LIST(Event, m_link) m_eventList;

        // Member functions
        Timer();
        void Insert(Event&);
        uint32_t Pump();
};

#endif
