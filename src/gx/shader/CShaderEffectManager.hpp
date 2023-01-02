#ifndef GX_SHADER_C_SHADER_EFFECT_MANAGER_HPP
#define GX_SHADER_C_SHADER_EFFECT_MANAGER_HPP

#include <storm/Hash.hpp>

class CShaderEffect;

class CShaderEffectManager {
    public:
        // Static variables
        static TSHashTable<CShaderEffect, HASHKEY_STRI> s_shaderList;

        // Static functions
        static CShaderEffect* CreateEffect(const char* effectKey);
        static CShaderEffect* GetEffect(const char* effectKey);
};

#endif
