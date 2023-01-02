#ifndef MODEL_C_M2_LIGHT_HPP
#define MODEL_C_M2_LIGHT_HPP

#include "model/M2Types.hpp"
#include <tempest/Vector.hpp>

class CM2Scene;

class CM2Light {
    public:
        // Member variables
        CM2Scene* m_scene = nullptr;
        int32_t m_type = 1;
        C3Vector m_pos;
        C3Vector m_dir;
        C3Vector m_ambColor;
        C3Vector m_dirColor;
        C3Vector m_specColor;
        float m_constantAttenuation = 0.0f;
        float m_linearAttenuation = 0.69999999f;
        float m_quadraticAttenuation = 0.029999999f;
        int32_t m_visible = 0;
        CM2Light** m_lightPrev = nullptr;
        CM2Light* m_lightNext = nullptr;

        // Member functions
        void Initialize(CM2Scene* scene);
        void Link();
        void SetDirection(const C3Vector& dir);
        void SetLightType(M2LIGHTTYPE lightType);
        void SetVisible(int32_t visible);
        void Unlink();
};

#endif
