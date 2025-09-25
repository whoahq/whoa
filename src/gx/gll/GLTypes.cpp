#include "gx/gll/GLTypes.h"
#include <cstring>

GLColor4f GLColor4f::ZERO = { 0.0, 0.0, 0.0, 0.0 };
GLColor4f GLColor4f::WHITE = { 1.0, 1.0, 1.0, 1.0 };
GLColor4f GLColor4f::BLACK = { 0.0, 0.0, 0.0, 1.0 };

bool GLTransform::operator==(const float m[16]) const {
    return this->m[0] == m[0]
        && this->m[1] == m[1]
        && this->m[2] == m[2]
        && this->m[3] == m[3]
        && this->m[4] == m[4]
        && this->m[5] == m[5]
        && this->m[6] == m[6]
        && this->m[7] == m[7]
        && this->m[8] == m[8]
        && this->m[9] == m[9]
        && this->m[10] == m[10]
        && this->m[11] == m[11]
        && this->m[12] == m[12]
        && this->m[13] == m[13]
        && this->m[14] == m[14]
        && this->m[15] == m[15];
}

bool GLTransform::operator!=(const float m[16]) const {
    return !(*this == m);
}

void GLTransform::Set(const float m[16]) {
    memcpy(this->m, m, sizeof(this->m));
    this->isDirty = true;
    this->isIdentity = this->a0 == 1.0f
        && this->a1 == 0.0f
        && this->a2 == 0.0f
        && this->a3 == 0.0f
        && this->b0 == 0.0f
        && this->b1 == 1.0f
        && this->b2 == 0.0f
        && this->b3 == 0.0f
        && this->c0 == 0.0f
        && this->c1 == 0.0f
        && this->c2 == 1.0f
        && this->c3 == 0.0f
        && this->d0 == 0.0f
        && this->d1 == 0.0f
        && this->d2 == 0.0f
        && this->d3 == 1.0f;
}

void GLTransform::SetIdentity() {
    memset(this->m, 0, sizeof(this->m));
    this->a0 = 1.0f;
    this->b1 = 1.0f;
    this->c2 = 1.0f;
    this->d3 = 1.0f;
}
