#ifndef GX_SHADER_C_SHADER_EFFECT_HPP
#define GX_SHADER_C_SHADER_EFFECT_HPP

#include <cstdint>
#include <storm/Hash.hpp>

class C3Vector;
class C4Vector;
class C44Matrix;
class CGxShader;
class CImVector;
class CM2Light;
class CM2Lighting;

class CShaderEffect : public TSHashObject<CShaderEffect, HASHKEY_STRI> {
    public:
        // Structs
        struct LocalLights {
            float float0[44];
        };

        // Static variables
        static CShaderEffect* s_curEffect;
        static int32_t s_enableShaders;
        static C4Vector s_fogColorAlphaRef;
        static float s_fogMul;
        static C4Vector s_fogParams;
        static int32_t s_lightEnabled;
        static uint32_t s_localLightCount;
        static LocalLights s_localLights;
        static C3Vector s_sunAmbient;
        static C3Vector s_sunDiffuse;
        static C3Vector s_sunDir;
        static int32_t s_useAlphaRef;
        static int32_t s_usePcfFiltering;

        // Static functions
        static void ComputeLocalLights(LocalLights* localLights, uint32_t localLightsCount, CM2Light** lights, const C3Vector* a4);
        static void InitShaderSystem(int32_t enableShaders, int32_t usePcf);
        static void SetAlphaRef(float alphaRef);
        static void SetDiffuse(const C4Vector& diffuse);
        static void SetEmissive(const C4Vector& emissive);
        static void SetFogEnabled(int32_t fogEnabled);
        static void SetFogParams(float fogStart, float fogEnd, float fogRate, const CImVector& fogColor);
        static void SetLocalLighting(CM2Lighting* lighting, int32_t lightEnabled, const C3Vector* a3);
        static void SetShaders(uint32_t vertexPermute, uint32_t pixelPermute);
        static void SetTexMtx(const C44Matrix& matrix, uint32_t tcIndex);
        static void SetTexMtx_Identity(uint32_t tcIndex);
        static void SetTexMtx_SphereMap(uint32_t tcIndex);
        static void UpdateProjMatrix();

        // Member variables
        CGxShader* m_vertexShaders[90];
        CGxShader* m_pixelShaders[16];

        // Member functions
        void InitEffect(const char* vsName, const char* psName);
        void SetCurrent();
};

#endif
