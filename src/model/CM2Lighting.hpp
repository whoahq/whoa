#ifndef MODEL_C_M2_LIGHTING_HPP
#define MODEL_C_M2_LIGHTING_HPP

#include <tempest/Plane.hpp>
#include <tempest/Sphere.hpp>
#include <tempest/Vector.hpp>

class CM2Light;
class CM2Scene;

class CM2Lighting {
    public:
        // Member variables
        CM2Scene* m_scene;
        CAaSphere sphere4;
        uint32_t m_flags;
        C3Vector vector18;
        C3Vector vector24;
        C3Vector vector30;
        C3Vector vector3C;
        C3Vector vector48;
        C3Vector m_sunAmbient;
        C3Vector m_sunDiffuse;
        C3Vector m_sunSpecular;
        C3Vector m_sunDir;
        CM2Light* m_lights[4];
        uint32_t m_lightCount;
        float m_fogStart;
        float m_fogEnd;
        float m_fogScale;
        float m_fogDensity;
        C3Vector m_fogColor;
        C4Plane m_liquidPlane;

        // Member functions
        void AddAmbient(const C3Vector& ambColor);
        void AddDiffuse(const C3Vector& dirColor, const C3Vector& dir);
        void AddLight(CM2Light* light);
        void AddSpecular(const C3Vector& specColor);
        void CameraSpace();
        void Initialize(CM2Scene* scene, const CAaSphere& a3);
        void SetFog(const C3Vector& fogColor, float fogStart, float fogEnd);
        void SetFog(const C3Vector& fogColor, float fogStart, float fogEnd, float fogDensity);
        void SetupGxLights(const C3Vector* a2);
        void SetupSunlight();
};

#endif
