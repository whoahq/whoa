#include "util/time/CGameTime.hpp"
#include "common/Time.hpp"

void CGameTime::GameTimeSetTime(const WowTime& time, bool shouldTick) {
    WowTime biasTime = time;

    if (this->m_timeBias) {
        auto minutes = biasTime.GetHourAndMinutes() + this->m_timeBias;

        if (minutes < 0) {
            minutes += 1440;
        } else {
            minutes %= 1440;
        }

        biasTime.SetHourAndMinutes(minutes);
    }

    if (this->m_dateBias) {
        biasTime.AddDays(this->m_dateBias, false);
    }

    static_cast<WowTime&>(*this) = biasTime;

    if (shouldTick) {
        // Rewind time by exactly one minute
        if (this->m_minute != 0) {
            this->m_minute--;
        } else {
            this->m_minute = 59;

            if (this->m_hour != 0) {
                this->m_hour--;
            } else {
                this->m_hour = 23;
                this->AddDays(-1, false);
            }
        }

        // Tick ahead exactly one minute (ensures callbacks fire and various counters update)
        this->TickMinute();
    }
}

void CGameTime::PerformCallbacks(int32_t minutes) {
    // TODO
}

void CGameTime::TickMinute() {
    // Increment minute
    int32_t minutes = (this->GetHourAndMinutes() + 1) % 1440;

    // Update hours and minutes
    this->SetHourAndMinutes(minutes);

    // Increment day if minute crossed day boundary
    if (minutes == 0) {
        this->AddDays(1, false);
    }

    this->m_gameMinutesElapsed++;

    this->PerformCallbacks(minutes);

    this->m_lastTickMinute = OsGetAsyncTimeMsPrecise();

    this->uint40 = 1;

    this->m_dayProgression = this->m_gameMinutesThisTick + static_cast<float>(minutes);
}
