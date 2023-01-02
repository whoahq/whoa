#ifndef MATH_TYPES_HPP
#define MATH_TYPES_HPP

#include <cstdint>

struct fixed16 {
    int16_t n;

    explicit operator float() const;
};

#endif
