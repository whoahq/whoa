#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include <cmath>
#include <cstdint>

#define WHOA_EPSILON_1 0.00000023841858
#define WHOA_EPSILON_2 0.0000099999997

inline bool AreEqual(float a, float b, double epsilon) {
    return std::abs(a - b) < epsilon;
}

inline bool NotEqual(float a, float b, double epsilon) {
    return std::abs(a - b) >= epsilon;
}

inline float SignOf(float value) {
    return value >= 0.0 ? 1.0f : -1.0f;
}

#endif
