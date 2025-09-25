#include "math/Types.hpp"

fixed16::operator float() const {
    return this->n / 32767.0f;
}
