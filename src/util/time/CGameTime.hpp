#ifndef UTIL_TIME_C_GAME_TIME_HPP
#define UTIL_TIME_C_GAME_TIME_HPP

#include "util/time/WowTime.hpp"

class CGameTime : public WowTime {
    public:
        // Public member functions
        void GameTimeSetTime(const WowTime& time, bool shouldTick);

    private:
        // Private member variables
        uint32_t m_lastTick = 0;
        int32_t m_timeBias = 0;
        int32_t m_dateBias = 0;
        uint32_t m_gameMinutesElapsed = 0;
        float m_gameMinutesPerRealSecond = 1.0f / 60.0f;
        float m_gameMinutesThisTick = 0.0f;
        uint32_t m_timeDifferential = 0;
        uint32_t m_lastTickMinute = 0;
        uint8_t uint40 = 0;
        float m_dayProgression = 0.0f;
        float float48 = 0.0f;
        float float4C = 0.0f;
        // TODO m_callbackLists

        // Private member functions
        void PerformCallbacks(int32_t minutes);
        void TickMinute();
};

#endif
