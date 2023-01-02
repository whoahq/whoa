#include "math/Types.hpp"
#include "math/Utils.hpp"
#include <cmath>

fixed16::operator float() const {
    return this->n / 32767.0f;
}
