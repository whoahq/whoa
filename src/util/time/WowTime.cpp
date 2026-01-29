#include "util/time/WowTime.hpp"
#include <storm/Error.hpp>
#include <storm/String.hpp>
#include <ctime>

static const char* s_weekdays[] = {
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat",
};

void WowTime::WowDecodeTime(uint32_t value, int32_t* minute, int32_t* hour, int32_t* weekday, int32_t* monthday, int32_t* month, int32_t* year, int32_t* flags) {
    // Minute: bits 0-5 (6 bits, max 63)
    if (minute) {
        auto m = static_cast<int32_t>(value & 63);
        *minute = (m == 63) ? -1 : m;
    }

    // Hour: bits 6-10 (5 bits, max 31)
    if (hour) {
        auto h = static_cast<int32_t>((value >> 6) & 31);
        *hour = (h == 31) ? -1 : h;
    }

    // Weekday: bits 11-13 (3 bits, max 7)
    if (weekday) {
        auto wd = static_cast<int32_t>((value >> 11) & 7);
        *weekday = (wd == 7) ? -1 : wd;
    }

    // Month day: bits 14-19 (6 bits, max 63)
    if (monthday) {
        auto md = static_cast<int32_t>((value >> 14) & 63);
        *monthday = (md == 63) ? -1 : md;
    }

    // Month: bits 20-23 (4 bits, max 15)
    if (month) {
        auto mo = static_cast<int32_t>((value >> 20) & 15);
        *month = (mo == 15) ? -1 : mo;
    }

    // Year: bits 24-28 (5 bits, max 31)
    if (year) {
        auto y = static_cast<int32_t>((value >> 24) & 31);
        *year = (y == 31) ? -1 : y;
    }

    // Flags: bits 29-30 (2 bits, max 3)
    if (flags) {
        auto f = static_cast<int32_t>((value >> 29) & 3);
        *flags = (f == 3) ? -1 : f;
    }
}

void WowTime::WowDecodeTime(uint32_t value, WowTime* time) {
    WowTime::WowDecodeTime(
        value,
        &time->m_minute,
        &time->m_hour,
        &time->m_weekday,
        &time->m_monthday,
        &time->m_month,
        &time->m_year,
        &time->m_flags
    );
}

void WowTime::WowEncodeTime(uint32_t& value, int32_t minute, int32_t hour, int32_t weekday, int32_t monthday, int32_t month, int32_t year, int32_t flags) {
    STORM_ASSERT(minute == -1 || (minute >= 0 && minute < 60));
    STORM_ASSERT(hour == -1 || (hour >= 0 && hour < 24));
    STORM_ASSERT(weekday == -1 || (weekday >= 0 && weekday < 7));
    STORM_ASSERT(monthday == -1 || (monthday >= 0 && monthday < 32));
    STORM_ASSERT(month == -1 || (month >= 0 && month < 12));
    STORM_ASSERT(year == -1 || year >= 0 && year <= ((1 << 5) - 1));
    STORM_ASSERT(flags >= 0 && flags <= ((1 << 2) - 1));

    value   = ((flags & 3) << 29)       // Flags: bits 29-30 (2 bits, max 3)
            | ((year & 31) << 24)       // Year: bits 24-28 (5 bits, max 31)
            | ((month & 15) << 20)      // Month: bits 20-23 (4 bits, max 15)
            | ((monthday & 63) << 14)   // Month day: bits 14-19 (6 bits, max 63)
            | ((weekday & 7) << 11)     // Weekday: bits 11-13 (3 bits, max 7)
            | ((hour & 31) << 6)        // Hour: bits 6-10 (5 bits, max 31)
            | (minute & 63);            // Minute: bits 0-5 (6 bits, max 63)
}

void WowTime::WowEncodeTime(uint32_t& value, const WowTime* time) {
    WowTime::WowEncodeTime(
        value,
        time->m_minute,
        time->m_hour,
        time->m_weekday,
        time->m_monthday,
        time->m_month,
        time->m_year,
        time->m_flags
    );
}

char* WowTime::WowGetTimeString(WowTime* time, char* str, int32_t len) {
    uint32_t encoded;
    WowTime::WowEncodeTime(encoded, time);

    if (encoded == 0) {
        SStrPrintf(str, len, "Not Set");
        return str;
    }

    char yearStr[8];
    char monthStr[8];
    char monthdayStr[8];
    char weekdayStr[8];
    char hourStr[8];
    char minuteStr[8];

    if (time->m_year >= 0) {
        SStrPrintf(yearStr, sizeof(yearStr), "%i", time->m_year + 2000);
    } else {
        SStrPrintf(yearStr, sizeof(yearStr), "A");
    }

    if (time->m_month >= 0) {
        SStrPrintf(monthStr, sizeof(monthStr), "%i", time->m_month + 1);
    } else {
        SStrPrintf(monthStr, sizeof(monthStr), "A");
    }

    if (time->m_monthday >= 0) {
        SStrPrintf(monthdayStr, sizeof(monthdayStr), "%i", time->m_monthday + 1);
    } else {
        SStrPrintf(monthdayStr, sizeof(monthdayStr), "A");
    }

    if (time->m_weekday >= 0) {
        SStrPrintf(weekdayStr, sizeof(weekdayStr), s_weekdays[time->m_weekday]);
    } else {
        SStrPrintf(weekdayStr, sizeof(weekdayStr), "Any");
    }

    if (time->m_hour >= 0) {
        SStrPrintf(hourStr, sizeof(hourStr), "%i", time->m_hour);
    } else {
        SStrPrintf(hourStr, sizeof(hourStr), "A");
    }

    if (time->m_minute >= 0) {
        SStrPrintf(minuteStr, sizeof(minuteStr), "%2.2i", time->m_minute);
    } else {
        SStrPrintf(minuteStr, sizeof(minuteStr), "A");
    }

    SStrPrintf(str, len, "%s/%s/%s (%s) %s:%s", monthStr, monthdayStr, yearStr, weekdayStr, hourStr, minuteStr);

    return str;
}

void WowTime::AddDays(int32_t days, bool includeTime) {
    // Validate date

    if (this->m_year < 0 || this->m_month < 0 || this->m_monthday < 0) {
        return;
    }

    // Convert WowTime to tm

    tm t = {};

    t.tm_year = this->m_year + 100;     // WowTime year is years since 2000; tm_year is years since 1900
    t.tm_mon = this->m_month;           // WowTime month and tm_mon are both 0-based
    t.tm_mday = this->m_monthday + 1;   // WowTime monthday is 0-based; tm_mday is 1-based
    t.tm_isdst = -1;                    // Let mktime determine DST

    if (includeTime) {
        t.tm_hour = this->m_hour;
        t.tm_min = this->m_minute;
    }

    // Convert tm to time_t and add the specified days

    auto time = mktime(&t);
    time += days * 86400;


    if (!includeTime) {
        time += 3600;                   // Tack on hour to ensure DST boundaries don't muck with days added
    }

    // Convert adjusted time back to tm

    auto t_ = localtime(&time);
    if (t_) {
        t = *t_;
    } else {
        t = {};
    }

    // Convert adjusted tm back to WowTime

    this->m_year = t.tm_year - 100;
    this->m_month = t.tm_mon;
    this->m_monthday = t.tm_mday - 1;
    this->m_weekday = t.tm_wday;

    if (includeTime) {
        this->m_hour = t.tm_hour;
        this->m_minute = t.tm_min;
    }
}

int32_t WowTime::GetHourAndMinutes() {
    if (this->m_hour < 0 || this->m_minute < 0) {
        return 0;
    }

    return this->m_hour * 60 + this->m_minute;
}

void WowTime::SetHourAndMinutes(int32_t minutes) {
    this->m_hour = minutes / 60;
    this->m_minute = minutes % 60;
}

int32_t WowTime::SetHourAndMinutes(uint32_t hour, uint32_t minutes) {
    if (hour >= 24 || minutes >= 60) {
        return 0;
    }

    this->m_hour = hour;
    this->m_minute = minutes;

    return 1;
}
