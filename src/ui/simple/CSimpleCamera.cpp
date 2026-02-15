#include "ui/simple/CSimpleCamera.hpp"

CSimpleCamera::CSimpleCamera(float nearZ, float farZ, float fov) {
    this->float10 = 0.0f;

    this->m_nearZ = nearZ;
    this->m_farZ = farZ;
    this->m_fov = fov;
    this->m_aspect = 1.0f;

    this->SetFacing(0.0f, 0.0f, 0.0f);
}

float CSimpleCamera::FOV() {
    return this->m_fov;
}

C3Vector CSimpleCamera::Forward() const {
    return { this->m_facing.a0, this->m_facing.a1, this->m_facing.a2 };
}

C3Vector CSimpleCamera::Right() const {
    return { this->m_facing.b0, this->m_facing.b1, this->m_facing.b2 };
}

void CSimpleCamera::SetFacing(const C3Vector& forward) {
    // TODO
}

void CSimpleCamera::SetFacing(const C3Vector& forward, const C3Vector& up) {
    // TODO
}

void CSimpleCamera::SetFacing(float yaw, float pitch, float roll) {
    this->m_facing.FromEulerAnglesZYX(yaw, pitch, roll);
}

void CSimpleCamera::SetFarZ(float farZ) {
    this->m_farZ = farZ;
}

void CSimpleCamera::SetFieldOfView(float fov) {
    this->m_fov = fov;
}

void CSimpleCamera::SetNearZ(float nearZ) {
    this->m_nearZ = nearZ;
}

C3Vector CSimpleCamera::Up() const {
    return { this->m_facing.c0, this->m_facing.c1, this->m_facing.c2 };
}
