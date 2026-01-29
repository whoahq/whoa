#include "util/time/CGameTime.hpp"
#include "catch.hpp"

TEST_CASE("CGameTime::CGameTime", "[util]") {
    SECTION("constructs correctly") {
        CGameTime time;

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

TEST_CASE("CGameTime::TickMinute", "[util]") {
    SECTION("ticks minute correctly") {
        CGameTime time;
        time.SetHourAndMinutes(0, 0);
        time.TickMinute();

        CHECK(time.m_hour == 0);
        CHECK(time.m_minute == 1);
    }
}
