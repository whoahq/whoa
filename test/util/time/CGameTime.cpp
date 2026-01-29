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

TEST_CASE("CGameTime::GameTimeSetTime", "[util]") {
    SECTION("sets game time correctly") {
        WowTime time;
        time.m_minute = 18;
        time.m_hour = 11;
        time.m_weekday = 3;
        time.m_monthday = 27;
        time.m_month = 0;
        time.m_year = 26;

        CGameTime gameTime;
        gameTime.GameTimeSetTime(time, true);

        CHECK(gameTime.m_minute == 18);
        CHECK(gameTime.m_hour == 11);
        CHECK(gameTime.m_weekday == 3);
        CHECK(gameTime.m_monthday == 27);
        CHECK(gameTime.m_month == 0);
        CHECK(gameTime.m_year == 26);
    }
}

TEST_CASE("CGameTime::GameTimeUpdate", "[util]") {
    SECTION("updates game time correctly") {
        WowTime time;
        time.m_minute = 18;
        time.m_hour = 11;
        time.m_weekday = 3;
        time.m_monthday = 27;
        time.m_month = 0;
        time.m_year = 26;

        CGameTime gameTime;
        gameTime.GameTimeSetTime(time, true);

        gameTime.GameTimeUpdate(60.0f);

        CHECK(gameTime.m_hour == 11);
        CHECK(gameTime.m_minute == 19);
    }
}
