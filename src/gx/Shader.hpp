#ifndef GX_SHADER_HPP
#define GX_SHADER_HPP

#include "gx/shader/CGxShader.hpp"
#include "gx/shader/CShaderEffect.hpp"
#include "gx/shader/CShaderEffectManager.hpp"
#include "gx/Types.hpp"
#include <cstdint>

char* GxShaderConstantsLock(EGxShTarget target);

void GxShaderConstantsSet(EGxShTarget, uint32_t, const float*, uint32_t);

void GxShaderConstantsUnlock(EGxShTarget target, uint32_t index, uint32_t count);

#endif
