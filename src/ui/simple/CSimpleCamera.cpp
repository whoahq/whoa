#include "ui/simple/CSimpleCamera.hpp"
#include "gx/Transform.hpp"
#include "model/Model2.hpp"
#include <tempest/Math.hpp>

namespace {

void FaceDirection(const C3Vector& direction, C3Vector& xPrime, C3Vector& yPrime, C3Vector& zPrime) {
    STORM_ASSERT(CMath::fnotequal(direction.SquaredMag(), 0.0f));

    // Forward
    xPrime = direction;

    // Right
    if (CMath::fequal(xPrime.SquaredMag(), 0.0f)) {
        yPrime.x = 1.0f;
        yPrime.y = 0.0f;
        yPrime.z = 0.0f;
    } else {
        yPrime.x = -xPrime.y;
        yPrime.y = xPrime.x;
        yPrime.z = 0.0f;

        CMath::normalize(yPrime.x, yPrime.y);
    }

    // Up (Forward cross Right)
    zPrime = C3Vector::Cross(xPrime, yPrime);
}

void BuildBillboardMatrix(const C3Vector& direction, C33Matrix& rotation) {
    C3Vector xPrime = {};
    C3Vector yPrime = {};
    C3Vector zPrime = {};

    FaceDirection(direction, xPrime, yPrime, zPrime);

    // Forward
    rotation.a0 = xPrime.x;
    rotation.a1 = xPrime.y;
    rotation.a2 = xPrime.z;

    // Right
    rotation.b0 = yPrime.x;
    rotation.b1 = yPrime.y;
    rotation.b2 = yPrime.z;

    // Up
    rotation.c0 = zPrime.x;
    rotation.c1 = zPrime.y;
    rotation.c2 = zPrime.z;
}

}

CSimpleCamera::CSimpleCamera(float nearZ, float farZ, float fov) {
    this->m_scene = nullptr;

    this->m_nearZ = nearZ;
    this->m_farZ = farZ;
    this->m_fov = fov;
    this->m_aspect = 1.0f;

    this->SetFacing(0.0f, 0.0f, 0.0f);
}

float CSimpleCamera::FOV() const {
    return this->m_fov;
}

C3Vector CSimpleCamera::Forward() const {
    return { this->m_facing.a0, this->m_facing.a1, this->m_facing.a2 };
}

CM2Scene* CSimpleCamera::GetScene() {
    if (!this->m_scene) {
        this->m_scene = M2CreateScene();
    }

    return this->m_scene;
}

C3Vector CSimpleCamera::Right() const {
    return { this->m_facing.b0, this->m_facing.b1, this->m_facing.b2 };
}

void CSimpleCamera::SetFacing(const C3Vector& forward) {
    BuildBillboardMatrix(forward, this->m_facing);
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

void CSimpleCamera::SetGxProjectionAndView(const CRect& projRect) {
    // Projection

    this->m_aspect = (projRect.maxX - projRect.minX) / (projRect.maxY - projRect.minY);

    C44Matrix projMat;
    GxuXformCreateProjection_Exact(this->FOV() * 0.6f, this->m_aspect, this->m_nearZ, this->m_farZ, projMat);

    GxXformSetProjection(projMat);

    // View

    C3Vector eye;
    C44Matrix viewMat;
    GxuXformCreateLookAtSgCompat(eye, this->Forward(), this->Up(), viewMat);

    GxXformSetView(viewMat);
}

C3Vector CSimpleCamera::Up() const {
    return { this->m_facing.c0, this->m_facing.c1, this->m_facing.c2 };
}
