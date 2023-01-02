#include "gx/shader/CShaderEffect.hpp"
#include "gx/Device.hpp"
#include "gx/Gx.hpp"
#include "gx/RenderState.hpp"
#include "gx/Shader.hpp"
#include "gx/Transform.hpp"
#include "model/CM2Lighting.hpp"
#include <algorithm>
#include <cstring>

CShaderEffect* CShaderEffect::s_curEffect;
int32_t CShaderEffect::s_enableShaders;
C4Vector CShaderEffect::s_fogColorAlphaRef;
float CShaderEffect::s_fogMul;
C4Vector CShaderEffect::s_fogParams;
int32_t CShaderEffect::s_lightEnabled;
uint32_t CShaderEffect::s_localLightCount;
CShaderEffect::LocalLights CShaderEffect::s_localLights;
C3Vector CShaderEffect::s_sunAmbient;
C3Vector CShaderEffect::s_sunDiffuse;
C3Vector CShaderEffect::s_sunDir;
int32_t CShaderEffect::s_useAlphaRef;
int32_t CShaderEffect::s_usePcfFiltering;

void CShaderEffect::ComputeLocalLights(LocalLights* localLights, uint32_t localLightsCount, CM2Light** lights, const C3Vector* a4) {
    // TODO
}

void CShaderEffect::InitShaderSystem(int32_t enableShaders, int32_t usePcf) {
    CShaderEffect::s_enableShaders = enableShaders;
    CShaderEffect::s_usePcfFiltering = enableShaders && usePcf ? 1 : 0;
    CShaderEffect::s_fogMul = 1.0f;

    CShaderEffect::s_useAlphaRef = GxCaps()->int130;
}

void CShaderEffect::SetAlphaRef(float alphaRef) {
    CShaderEffect::s_fogColorAlphaRef.w = alphaRef;

    if (CShaderEffect::s_useAlphaRef) {
        GxRsSet(GxRs_AlphaRef, static_cast<int32_t>(alphaRef * 255.0f));
    } else {
        GxShaderConstantsSet(GxSh_Pixel, 2, reinterpret_cast<float*>(&CShaderEffect::s_fogColorAlphaRef), 1);
    }
}

void CShaderEffect::SetDiffuse(const C4Vector& diffuse) {
    if (CShaderEffect::s_enableShaders) {
        GxShaderConstantsSet(GxSh_Vertex, 28, reinterpret_cast<const float*>(&diffuse), 1);
        return;
    }

    // TODO
    // - non-shader code path
}

void CShaderEffect::SetEmissive(const C4Vector& emissive) {
    if (CShaderEffect::s_enableShaders) {
        GxShaderConstantsSet(GxSh_Vertex, 29, reinterpret_cast<const float*>(&emissive), 1);
        return;
    }

    // TODO non-shader code path
}

void CShaderEffect::SetFogEnabled(int32_t fogEnabled) {
    if (fogEnabled && GxMasterEnable(GxMasterEnable_Fog)) {
        if (CShaderEffect::s_enableShaders && !GxCaps()->int138) {
            GxShaderConstantsSet(GxSh_Vertex, 30, reinterpret_cast<float*>(&CShaderEffect::s_fogParams), 1);
        } else {
            GxRsSet(GxRs_Fog, 1);
        }
    } else {
        if (CShaderEffect::s_enableShaders && !GxCaps()->int138) {
            float fogParams[] = { 0.0f, 1.0f, 1.0f, 0.0f };
            GxShaderConstantsSet(GxSh_Vertex, 30, fogParams, 1);
        } else {
            GxRsSet(GxRs_Fog, 0);
        }
    }
}

void CShaderEffect::SetFogParams(float fogStart, float fogEnd, float fogRate, const CImVector& fogColor) {
    if (CShaderEffect::s_enableShaders) {
        CShaderEffect::s_fogColorAlphaRef.x = fogColor.r / 255.0f;
        CShaderEffect::s_fogColorAlphaRef.y = fogColor.g / 255.0f;
        CShaderEffect::s_fogColorAlphaRef.z = fogColor.b / 255.0f;

        float v4 = 1.0f / (fogEnd - fogStart);
        CShaderEffect::s_fogParams.x = -(CShaderEffect::s_fogMul * v4);
        CShaderEffect::s_fogParams.y = fogEnd * v4;
        CShaderEffect::s_fogParams.z = fogRate;
        CShaderEffect::s_fogParams.w = 0.0f;

        if (!GxCaps()->int134) {
            GxShaderConstantsSet(GxSh_Pixel, 2, reinterpret_cast<float*>(&CShaderEffect::s_fogColorAlphaRef), 1);
            return;
        }
    } else {
        GxRsSet(GxRs_FogStart, fogStart);
        GxRsSet(GxRs_FogEnd, fogEnd);
    }

    GxRsSet(GxRs_FogColor, fogColor.value);
}

void CShaderEffect::SetLocalLighting(CM2Lighting* lighting, int32_t lightEnabled, const C3Vector* a3) {
    CShaderEffect::s_lightEnabled = lightEnabled;

    if (!CShaderEffect::s_enableShaders) {
        GxRsSet(GxRs_Lighting, lightEnabled);
    }

    CShaderEffect::s_localLightCount = lighting ? lighting->m_lightCount : 0;

    if (!lightEnabled) {
        return;
    }

    if (CShaderEffect::s_enableShaders) {
        CShaderEffect::s_sunDir = lighting->m_sunDir;

        if (CShaderEffect::s_sunDir.x != 0.0f || CShaderEffect::s_sunDir.y != 0.0f || CShaderEffect::s_sunDir.z != 0.0f) {
            CShaderEffect::s_sunDir.Normalize();
        }

        CShaderEffect::s_sunAmbient = lighting->m_sunAmbient;

        CShaderEffect::s_sunDiffuse = {
            std::min(lighting->m_sunDiffuse.x, 1.0f),
            std::min(lighting->m_sunDiffuse.y, 1.0f),
            std::min(lighting->m_sunDiffuse.z, 1.0f)
        };

        GxShaderConstantsSet(GxSh_Vertex, 10, reinterpret_cast<float*>(&CShaderEffect::s_sunDiffuse), 1);
        GxShaderConstantsSet(GxSh_Vertex, 11, reinterpret_cast<float*>(&CShaderEffect::s_sunAmbient), 1);
        GxShaderConstantsSet(GxSh_Vertex, 12, reinterpret_cast<float*>(&CShaderEffect::s_sunDir), 1);

        if (CShaderEffect::s_localLightCount) {
            CShaderEffect::ComputeLocalLights(
                &CShaderEffect::s_localLights,
                CShaderEffect::s_localLightCount,
                lighting->m_lights,
                a3
            );

            GxShaderConstantsSet(GxSh_Vertex, 17, reinterpret_cast<float*>(&CShaderEffect::s_localLights), 11);
        }

        // TODO
        // CShadowCache::SetShadowMapGenericInterior(lighting->m_flags & 0x8);
    } else {
        lighting->SetupGxLights(a3);
    }
}

void CShaderEffect::SetShaders(uint32_t vertexPermute, uint32_t pixelPermute) {
    int32_t useAlphaRef = 1;

    if (CShaderEffect::s_enableShaders) {
        GxRsSet(GxRs_VertexShader, CShaderEffect::s_curEffect->m_vertexShaders[vertexPermute]);
        GxRsSet(GxRs_PixelShader, CShaderEffect::s_curEffect->m_pixelShaders[pixelPermute]);

        useAlphaRef = (pixelPermute & 0x8) == 0;
    }

    if (CShaderEffect::s_useAlphaRef != useAlphaRef) {
        CShaderEffect::s_useAlphaRef = useAlphaRef;

        if (useAlphaRef) {
            GxRsSet(GxRs_AlphaRef, static_cast<uint8_t>(CShaderEffect::s_fogColorAlphaRef.w * 255.0f));
        } else {
            GxShaderConstantsSet(GxSh_Pixel, 2, reinterpret_cast<float*>(&CShaderEffect::s_fogColorAlphaRef), 1);
            GxRsSet(GxRs_AlphaRef, 0);
        }
    }
}

void CShaderEffect::SetTexMtx_Identity(uint32_t a1) {
    if (CShaderEffect::s_enableShaders) {
        float matrix[] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f
        };

        GxShaderConstantsSet(GxSh_Vertex, 2 * a1 + 6, matrix, 2);
    } else {
        // TODO
        // - non-shader code path
    }
}

void CShaderEffect::SetTexMtx(const C44Matrix& matrix, uint32_t a2) {
    // TODO
}

void CShaderEffect::SetTexMtx_SphereMap(uint32_t a1) {
    if (CShaderEffect::s_enableShaders) {
        float matrix[] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f
        };

        GxShaderConstantsSet(GxSh_Vertex, 2 * a1 + 6, matrix, 2);
    } else {
        // TODO
        // - non-shader code path
    }
}

void CShaderEffect::UpdateProjMatrix() {
    if (!CShaderEffect::s_enableShaders) {
        return;
    }

    C44Matrix proj;
    GxXformProjNativeTranspose(proj);

    GxShaderConstantsSet(GxSh_Vertex, 2, reinterpret_cast<float*>(&proj), 4);
}

void CShaderEffect::InitEffect(const char* vsName, const char* psName) {
    memset(this->m_vertexShaders, 0, sizeof(this->m_vertexShaders));
    memset(this->m_pixelShaders, 0, sizeof(this->m_pixelShaders));

    // TODO
    // this->dword18 = 0;

    if (CShaderEffect::s_enableShaders) {
        if (vsName && psName) {
            g_theGxDevicePtr->ShaderCreate(this->m_vertexShaders, GxSh_Vertex, "Shaders\\Vertex", vsName, 90);
            g_theGxDevicePtr->ShaderCreate(this->m_pixelShaders, GxSh_Pixel, "Shaders\\Pixel", psName, 16);
        }
    }
}

void CShaderEffect::SetCurrent() {
    CShaderEffect::s_curEffect = this;

    // TODO
    // - non-shader code path
}
