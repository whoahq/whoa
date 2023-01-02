#include "model/CM2Lighting.hpp"
#include "model/CM2Light.hpp"
#include "model/CM2Scene.hpp"
#include <cstring>

void CM2Lighting::AddAmbient(const C3Vector& ambColor) {
    this->m_sunAmbient = this->m_sunAmbient + ambColor;
}

void CM2Lighting::AddDiffuse(const C3Vector& dirColor, const C3Vector& dir) {
    C3Vector viewDir = dir;

    if (this->m_scene) {
        viewDir = {
            this->m_scene->m_view.a0 * dir.x + this->m_scene->m_view.b0 * dir.y + this->m_scene->m_view.c0 * dir.z,
            this->m_scene->m_view.a1 * dir.x + this->m_scene->m_view.b1 * dir.y + this->m_scene->m_view.c1 * dir.z,
            this->m_scene->m_view.a2 * dir.x + this->m_scene->m_view.b2 * dir.y + this->m_scene->m_view.c2 * dir.z
        };
    }

    this->vector18.x = viewDir.x * dirColor.x + this->vector18.x;
    this->vector18.y = viewDir.y * dirColor.x + this->vector18.y;
    this->vector18.z = viewDir.z * dirColor.x + this->vector18.z;

    this->vector24.x = viewDir.x * dirColor.y + this->vector24.x;
    this->vector24.y = viewDir.y * dirColor.y + this->vector24.y;
    this->vector24.z = viewDir.z * dirColor.y + this->vector24.z;

    this->vector30.x = viewDir.x * dirColor.z + this->vector30.x;
    this->vector30.y = viewDir.y * dirColor.z + this->vector30.y;
    this->vector30.z = viewDir.z * dirColor.z + this->vector30.z;

    float v7 = dirColor.y * 0.71516001f + dirColor.x * 0.212671f + dirColor.z * 0.072168998f;

    this->vector3C.x = viewDir.x * v7 + this->vector3C.x;
    this->vector3C.y = viewDir.y * v7 + this->vector3C.y;
    this->vector3C.z = viewDir.z * v7 + this->vector3C.z;

    this->vector48.x = dirColor.x + this->vector48.x;
    this->vector48.y = dirColor.y + this->vector48.y;
    this->vector48.z = dirColor.z + this->vector48.z;

    this->m_sunDir = dir;

    this->m_sunDiffuse = dirColor;
}

void CM2Lighting::AddLight(CM2Light* light) {
    if (!light->m_visible) {
        return;
    }

    if (light->m_type == 1) {
        // TODO
    } else {
        this->AddAmbient(light->m_ambColor);
        this->AddDiffuse(light->m_dirColor, light->m_dir);
        this->AddSpecular(light->m_specColor);
    }
}

void CM2Lighting::AddSpecular(const C3Vector& specColor) {
    this->m_sunSpecular = this->m_sunSpecular + specColor;
}

void CM2Lighting::CameraSpace() {
    // TODO
}

void CM2Lighting::Initialize(CM2Scene* scene, const CAaSphere& a3) {
    memset(this, 0, sizeof(CM2Lighting));

    this->m_scene = scene;
    this->m_flags |= 0x20u;
    this->sphere4 = a3;
}

void CM2Lighting::SetFog(const C3Vector& fogColor, float fogStart, float fogEnd) {
    this->m_fogStart = fogStart;
    this->m_fogEnd = fogEnd;
    this->m_fogScale = 1.0f / (fogEnd - fogStart);
    this->m_fogDensity = 1.0f;
    this->m_fogColor = fogColor;
}

void CM2Lighting::SetFog(const C3Vector& fogColor, float fogStart, float fogEnd, float fogDensity) {
    this->m_fogStart = fogStart;
    this->m_fogEnd = fogEnd;
    this->m_fogScale = 1.0f / (fogEnd - fogStart);
    this->m_fogDensity = fogDensity;
    this->m_fogColor = fogColor;
}

void CM2Lighting::SetupGxLights(const C3Vector* a2) {
    // TODO
}

void CM2Lighting::SetupSunlight() {
    if (this->m_flags & 0x2) {
        return;
    }

    this->m_sunDir.x = this->vector3C.x;
    this->m_sunDir.y = this->vector3C.y;
    this->m_sunDir.z = this->vector3C.z;

    if (this->m_sunDir.SquaredMag() <= 0.0000099999997) {
        this->m_sunDir = { 0.0f, 0.0f, -1.0f };
    } else {
        this->m_sunDir.Normalize();
    }

    float v6 = this->m_sunDir.z * this->vector18.z + this->m_sunDir.y * this->vector18.y + this->m_sunDir.x * this->vector18.x;
    float v7 = this->m_sunDir.z * this->vector24.z + this->m_sunDir.y * this->vector24.y + this->m_sunDir.x * this->vector24.x;
    float v8 = this->m_sunDir.z * this->vector30.z + this->m_sunDir.y * this->vector30.y + this->m_sunDir.x * this->vector30.x;

    this->m_sunDiffuse = {
        v6 * 1.25f - this->vector48.x * 0.25f,
        v7 * 1.25f - this->vector48.y * 0.25f,
        v8 * 1.25f - this->vector48.z * 0.25f
    };

    this->m_sunAmbient = {
        (this->vector48.x - v6) * 0.25f + this->m_sunAmbient.x,
        (this->vector48.y - v7) * 0.25f + this->m_sunAmbient.y,
        (this->vector48.z - v8) * 0.25f + this->m_sunAmbient.z
    };

    this->m_flags |= 0x2;
}
