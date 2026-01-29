#ifndef UTIL_TIME_WOW_TIME_HPP
#define UTIL_TIME_WOW_TIME_HPP

#include <cstdint>

class WowTime {
    public:
        // Static functions
        static void WowDecodeTime(uint32_t value, int32_t* minute, int32_t* hour, int32_t* weekday, int32_t* monthday, int32_t* month, int32_t* year, int32_t* flags);
        static void WowDecodeTime(uint32_t value, WowTime* time);
        static void WowEncodeTime(uint32_t& value, int32_t minute, int32_t hour, int32_t weekday, int32_t monthday, int32_t month, int32_t year, int32_t flags);
        static void WowEncodeTime(uint32_t& value, const WowTime* time);
        static char* WowGetTimeString(WowTime* time, char* str, int32_t len);

        // Member variables
        int32_t m_minute = -1;
        int32_t m_hour = -1;
        int32_t m_weekday = -1;
        int32_t m_monthday = -1;
        int32_t m_month = -1;
        int32_t m_year = -1;
        int32_t m_flags = 0x0;
        int32_t m_holidayOffset = 0;

        // Member functions
        int32_t GetHourAndMinutes();
        void SetHourAndMinutes(int32_t minutes);
        int32_t SetHourAndMinutes(uint32_t hour, uint32_t minutes);
};

#endif
