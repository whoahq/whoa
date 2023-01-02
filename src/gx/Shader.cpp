#include "gx/Shader.hpp"
#include "gx/Device.hpp"

char* GxShaderConstantsLock(EGxShTarget target) {
    return g_theGxDevicePtr->ShaderConstantsLock(target);
}

void GxShaderConstantsSet(EGxShTarget target, uint32_t index, const float* constants, uint32_t count) {
    g_theGxDevicePtr->ShaderConstantsSet(target, index, constants, count);
}

void GxShaderConstantsUnlock(EGxShTarget target, uint32_t index, uint32_t count) {
    g_theGxDevicePtr->ShaderConstantsUnlock(target, index, count);
}
