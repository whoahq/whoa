#include "util/time/CGameTime.hpp"
#include "common/Time.hpp"

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
