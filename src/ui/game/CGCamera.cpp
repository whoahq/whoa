#include "ui/game/CGCamera.hpp"
#include "ui/game/Types.hpp"
#include "console/CVar.hpp"
#include "world/World.hpp"
#include <storm/String.hpp>
#include <tempest/Math.hpp>
#include <algorithm>

static CVar* s_cameraView;

CGCamera::CameraViewData CGCamera::s_cameraViewDataDefault[MAX_CAMERA_VIEWS] = {
    {  "0.0",   "0.0", "0.0" },     // VIEW_FIRST_PERSON
    {  "0.0",   "0.0", "0.0" },     // VIEW_THIRD_PERSON_A
    {  "5.55", "10.0", "0.0" },     // VIEW_THIRD_PERSON_B
    {  "5.55", "20.0", "0.0" },     // VIEW_THIRD_PERSON_C
    { "13.88", "30.0", "0.0" },     // VIEW_THIRD_PERSON_D
    { "13.88", "10.0", "0.0" },     // VIEW_THIRD_PERSON_E
    {  "0.0",   "0.0", "0.0" },     // VIEW_COMMENTATOR
    {  "5.0",  "10.0", "0.0" },     // VIEW_BARBER_SHOP
};

namespace {

bool ValidateCameraView(CVar* var, const char* oldValue, const char* value, void* arg) {
    auto view = SStrToFloat(value);
    auto min = static_cast<float>(VIEW_FIRST_PERSON);
    auto max = static_cast<float>(VIEW_BARBER_SHOP);

    if (view >= min && view <= max) {
        return true;
    }

    // TODO ConsoleWriteA("Value out of range (%f - %f)\n", DEFAULT_COLOR, min, max);

    return false;
}

}

CGCamera::CGCamera() : CSimpleCamera(CWorld::GetNearClip(), CWorld::GetFarClip(), 90.0f * CMath::DEG2RAD) {
    this->m_model = nullptr;

    this->m_target = 0;
    this->m_relativeTo = 0;

    this->m_view = s_cameraView->GetInt();

    this->m_distance = SStrToFloat(CGCamera::s_cameraViewDataDefault[this->m_view].m_distance);
    this->m_yaw = 0.0f;
    this->m_pitch = SStrToFloat(CGCamera::s_cameraViewDataDefault[this->m_view].m_pitch);
    this->m_roll = 0.0f;

    this->m_fovOffset = 0.0f;
}

float CGCamera::FOV() const {
    // Clamp offset-adjusted FOV between 0pi and 1pi
    return std::min(std::max(this->m_fov + this->m_fovOffset, 0.0f), CMath::PI);
}

C3Vector CGCamera::Forward() const {
    if (this->m_relativeTo) {
        return this->CSimpleCamera::Forward() * this->ParentToWorld();
    }

    return this->CSimpleCamera::Forward();
}

const WOWGUID& CGCamera::GetTarget() const {
    return this->m_target;
}

int32_t CGCamera::HasModel() const {
    return this->m_model != nullptr;
}

C33Matrix CGCamera::ParentToWorld() const {
    // TODO
    return {};
}

C3Vector CGCamera::Right() const {
    if (this->m_relativeTo) {
        return this->CSimpleCamera::Right() * this->ParentToWorld();
    }

    return this->CSimpleCamera::Right();
}

void CGCamera::SetupWorldProjection(const CRect& projRect) {
    this->SetGxProjectionAndView(projRect);
}

C3Vector CGCamera::Target() const {
    return this->m_position + this->Forward();
}

C3Vector CGCamera::Up() const {
    if (this->m_relativeTo) {
        return this->CSimpleCamera::Up() * this->ParentToWorld();
    }

    return this->CSimpleCamera::Up();
}

void CameraRegisterCVars() {
    // TODO

    s_cameraView = CVar::Register("cameraView", nullptr, 0x10, "2", &ValidateCameraView, DEFAULT);

    // TODO
}
