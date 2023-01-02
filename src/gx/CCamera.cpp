#include "gx/CCamera.hpp"
#include "gx/Shader.hpp"
#include "gx/Transform.hpp"
#include <cmath>
#include <tempest/Math.hpp>
#include <tempest/Matrix.hpp>
#include <tempest/Rect.hpp>

const C3Vector CCamera::DEFAULT_POSITION = { 100.0f, 0.0f, 0.0f };
const float CCamera::DEFAULT_DIST = 100.0f;
const float CCamera::DEFAULT_FARZ = 5000.0f;
const float CCamera::DEFAULT_NEARZ = 8.0f;

float CAngle::ClampTo2Pi(float angle) {
    double v1 = floor(static_cast<double>(CMath::OO_TWO_PI) * angle) * CMath::TWO_PI;

    if (angle < 0.0f) {
        v1 -= CMath::TWO_PI;
    }

    return angle - v1;
}

void CAngle::Set(const float& angle) {
    float clampedAngle = CAngle::ClampTo2Pi(angle);

    if (this->m_data != clampedAngle) {
        this->m_flags |= CBaseManaged::UPDATED;
        this->m_data = clampedAngle;
    }

    this->m_cos = cos(this->m_data);
    this->m_sin = sin(this->m_data);
}

void CCamera::SetupWorldProjection(const CRect& projectionRect, uint32_t flags) {
    C44Matrix projMat;
    float aspect = (projectionRect.maxX - projectionRect.minX) / (projectionRect.maxY - projectionRect.minY);
    GxuXformCreateProjection_SG(this->m_fov.m_data, aspect, this->m_zNear.m_data, this->m_zFar.m_data, projMat);
    GxXformSetProjection(projMat);

    C44Matrix viewMat;
    C3Vector cameraPos = { 0.0f, 0.0f, 0.0f };
    C3Vector cameraVec = {
        this->m_target.m_data.x - this->m_position.m_data.x,
        this->m_target.m_data.y - this->m_position.m_data.y,
        this->m_target.m_data.z - this->m_position.m_data.z
    };
    C3Vector upVec = {
        this->m_rotation.m_sin * this->m_roll.m_sin,
        -(this->m_rotation.m_cos * this->m_roll.m_sin),
        this->m_roll.m_cos
    };
    GxuXformCreateLookAtSgCompat(cameraPos, cameraVec, upVec, viewMat);
    GxXformSetView(viewMat);

    CShaderEffect::UpdateProjMatrix();
}
