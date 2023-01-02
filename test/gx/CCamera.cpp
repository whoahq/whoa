#include "catch.hpp"
#include "gx/CCamera.hpp"

TEST_CASE("CAngle::ClampTo2Pi", "[gx]") {
    SECTION("does not modify given angle 0.0f") {
        float angle = 0.0f;
        REQUIRE(CAngle::ClampTo2Pi(angle) == angle);
    }

    SECTION("does not modify given angle 1.570796371f") {
        float angle = 1.570796371f;
        REQUIRE(CAngle::ClampTo2Pi(angle) == angle);
    }

    SECTION("does not modify given angle 2.094395161f") {
        float angle = 2.094395161f;
        REQUIRE(CAngle::ClampTo2Pi(angle) == angle);
    }

    SECTION("does not modify given angle 6.283185482f") {
        float angle = 6.283185482f;
        REQUIRE(CAngle::ClampTo2Pi(angle) == angle);
    }
}
