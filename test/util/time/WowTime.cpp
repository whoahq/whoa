#include "util/time/WowTime.hpp"
#include "catch.hpp"
#include "storm/String.hpp"

TEST_CASE("WowTime::WowTime", "[util]") {
    SECTION("constructs correctly") {
        WowTime time;

        CHECK(time.m_minute == -1);
        CHECK(time.m_hour == -1);
        CHECK(time.m_weekday == -1);
        CHECK(time.m_monthday == -1);
        CHECK(time.m_month == -1);
        CHECK(time.m_year == -1);
        CHECK(time.m_flags == 0x0);
        CHECK(time.m_holidayOffset == 0);
    }
}

TEST_CASE("WowTime::WowDecodeTime", "[util]") {
    SECTION("decodes 1234567890 as expected") {
        uint32_t value = 1234567890;
        WowTime time;

        WowTime::WowDecodeTime(value, &time.m_minute, &time.m_hour, &time.m_weekday, &time.m_monthday, &time.m_month, &time.m_year, &time.m_flags);

        CHECK(time.m_minute == 18);
        CHECK(time.m_hour == 11);
        CHECK(time.m_weekday == 0);
        CHECK(time.m_monthday == 24);
        CHECK(time.m_month == 9);
        CHECK(time.m_year == 9);
        CHECK(time.m_flags == 0x2);
    }

    SECTION("decodes 0xFFFFFFFF as expected") {
        uint32_t value = 0xFFFFFFFF;
        WowTime time;

        WowTime::WowDecodeTime(value, &time.m_minute, &time.m_hour, &time.m_weekday, &time.m_monthday, &time.m_month, &time.m_year, &time.m_flags);

        CHECK(time.m_minute == -1);
        CHECK(time.m_hour == -1);
        CHECK(time.m_weekday == -1);
        CHECK(time.m_monthday == -1);
        CHECK(time.m_month == -1);
        CHECK(time.m_year == -1);
        CHECK(time.m_flags == -1);
    }
}

TEST_CASE("WowTime::WowEncodeTime", "[util]") {
    SECTION("encodes 10/25/2009 (Sun) 11:18 with flag 0x2 set as expected") {
        uint32_t value = 0;

        WowTime time;
        time.m_minute = 18;
        time.m_hour = 11;
        time.m_weekday = 0;
        time.m_monthday = 24;
        time.m_month = 9;
        time.m_year = 9;
        time.m_flags = 0x2;

        WowTime::WowEncodeTime(value, &time);

        REQUIRE(value == 1234567890);
    }

    SECTION("encodes empty time as expected") {
        uint32_t value = 0;
        WowTime time;

        WowTime::WowEncodeTime(value, &time);

        REQUIRE(value == 0x1FFFFFFF);
    }

    SECTION("encodes, decodes, and reencodes as expected") {
        uint32_t value = 0;

        WowTime time1;
        time1.m_minute = 18;
        time1.m_hour = 11;
        time1.m_weekday = 0;
        time1.m_monthday = 24;
        time1.m_month = 9;
        time1.m_year = 9;
        time1.m_flags = 0x2;

        WowTime time2;

        CHECK(time2.m_minute != time1.m_minute);
        CHECK(time2.m_hour != time1.m_hour);
        CHECK(time2.m_weekday != time1.m_weekday);
        CHECK(time2.m_monthday != time1.m_monthday);
        CHECK(time2.m_month != time1.m_month);
        CHECK(time2.m_year != time1.m_year);
        CHECK(time2.m_flags != time1.m_flags);

        WowTime::WowEncodeTime(value, &time1);
        WowTime::WowDecodeTime(value, &time2);

        CHECK(time2.m_minute == time1.m_minute);
        CHECK(time2.m_hour == time1.m_hour);
        CHECK(time2.m_weekday == time1.m_weekday);
        CHECK(time2.m_monthday == time1.m_monthday);
        CHECK(time2.m_month == time1.m_month);
        CHECK(time2.m_year == time1.m_year);
        CHECK(time2.m_flags == time1.m_flags);
    }
}

TEST_CASE("WowTime::WowGetTimeString", "[util]") {
    SECTION("gets expected string") {
        WowTime time;
        time.m_minute = 18;
        time.m_hour = 11;
        time.m_weekday = 0;
        time.m_monthday = 24;
        time.m_month = 9;
        time.m_year = 9;
        time.m_flags = 0x2;

        char buf[128];
        auto timeStr = WowTime::WowGetTimeString(&time, buf, sizeof(buf));

        REQUIRE(!SStrCmp(timeStr, "10/25/2009 (Sun) 11:18"));
    }
}

TEST_CASE("WowTime::GetHourAndMinutes", "[util]") {
    SECTION("gets expected hour and minutes for default constructed time") {
        WowTime time;

        REQUIRE(time.GetHourAndMinutes() == 0);
    }

    SECTION("gets expected hour and minutes for 11:18") {
        WowTime time;
        time.m_minute = 18;
        time.m_hour = 11;

        REQUIRE(time.GetHourAndMinutes() == 11 * 60 + 18);
    }
}
