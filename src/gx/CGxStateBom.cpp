#include "gx/CGxStateBom.hpp"
#include <tempest/Vector.hpp>

const CGxStateBom& CGxStateBom::operator=(int32_t value) {
    this->m_data.i[0] = value;

    return *this;
}

const CGxStateBom& CGxStateBom::operator=(uint32_t value) {
    this->m_data.u[0] = value;

    return *this;
}

const CGxStateBom& CGxStateBom::operator=(float value) {
    this->m_data.f[0] = value;

    return *this;
}

const CGxStateBom& CGxStateBom::operator=(void* value) {
    this->m_data.p = value;

    return *this;
}

const CGxStateBom& CGxStateBom::operator=(C3Vector& value) {
    this->m_data.f[0] = value.x;
    this->m_data.f[1] = value.y;
    this->m_data.f[2] = value.z;

    return *this;
}

bool CGxStateBom::operator==(float value) {
    return this->m_data.f[0] == value;
}

bool CGxStateBom::operator==(int32_t value) {
    return this->m_data.i[0] == value;
}

bool CGxStateBom::operator==(uint32_t value) {
    return this->m_data.u[0] == value;
}

bool CGxStateBom::operator==(void* value) {
    return this->m_data.p == value;
}

bool CGxStateBom::operator==(C3Vector& value) {
    return this->m_data.f[0] == value.x
        || this->m_data.f[1] == value.y
        || this->m_data.f[2] == value.z;
}

bool CGxStateBom::operator!=(float value) {
    return !(*this == value);
}

bool CGxStateBom::operator!=(int32_t value) {
    return !(*this == value);
}

bool CGxStateBom::operator!=(uint32_t value) {
    return !(*this == value);
}

bool CGxStateBom::operator!=(void* value) {
    return !(*this == value);
}

bool CGxStateBom::operator!=(C3Vector& value) {
    return this->m_data.f[0] != value.x
        || this->m_data.f[1] != value.y
        || this->m_data.f[2] != value.z;
}

bool CGxStateBom::operator!=(CGxStateBom& value) {
    return (this->m_data.i[0] - value.m_data.i[0])
        | (this->m_data.i[1] - value.m_data.i[1])
        | (this->m_data.i[2] - value.m_data.i[2])
        | (this->filler - value.filler);
}

CGxStateBom::operator CImVector() const {
    CImVector color;
    color.value = this->m_data.u[0];
    return color;
}

CGxStateBom::operator float() const {
    return this->m_data.f[0];
}

CGxStateBom::operator int32_t() const {
    return this->m_data.i[0];
}

CGxStateBom::operator uint32_t() const {
    return this->m_data.u[0];
}

CGxStateBom::operator void*() const {
    return this->m_data.p;
}
