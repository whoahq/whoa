#include "model/CM2Light.hpp"
#include "model/CM2Scene.hpp"

void CM2Light::Initialize(CM2Scene* scene) {
    this->m_scene = scene;

    // TODO
}

void CM2Light::Link() {
    if (!this->m_visible || !this->m_scene) {
        return;
    }

    if (this->m_type == M2LIGHT_1) {
        // TODO
    } else {
        if (!(this->m_scene->m_flags & 0x1)) {
            this->m_lightPrev = &this->m_scene->m_lightList;
            this->m_lightNext = this->m_scene->m_lightList;
            this->m_scene->m_lightList = this;

            if (this->m_lightNext) {
                this->m_lightNext->m_lightPrev = &this->m_lightNext;
            }
        }
    }
}

void CM2Light::SetDirection(const C3Vector& dir) {
    this->m_dir = dir;

    if (this->m_dir.SquaredMag() > 0.00000023841858) {
        this->m_dir.Normalize();
    }
}

void CM2Light::SetLightType(M2LIGHTTYPE lightType) {
    if (this->m_type == lightType) {
        return;
    }

    this->m_type = lightType;

    if (this->m_visible && this->m_scene) {
        this->Unlink();
        this->Link();
    }
}

void CM2Light::SetVisible(int32_t visible) {
    if (this->m_visible == visible) {
        return;
    }

    this->m_visible = visible;

    if (this->m_scene) {
        if (this->m_visible) {
            this->Link();
        } else {
            this->Unlink();
        }
    }
}

void CM2Light::Unlink() {
    if (this->m_lightPrev) {
        *this->m_lightPrev = this->m_lightNext;
    }

    if (this->m_lightNext) {
        this->m_lightNext->m_lightPrev = this->m_lightPrev;
    }

    this->m_lightPrev = nullptr;
    this->m_lightNext = nullptr;
}
