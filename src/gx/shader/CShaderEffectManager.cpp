#include "gx/shader/CShaderEffectManager.hpp"
#include "gx/shader/CShaderEffect.hpp"

TSHashTable<CShaderEffect, HASHKEY_STRI> CShaderEffectManager::s_shaderList;

CShaderEffect* CShaderEffectManager::CreateEffect(const char* effectKey) {
    return CShaderEffectManager::s_shaderList.New(effectKey, 0, 0);
}

CShaderEffect* CShaderEffectManager::GetEffect(const char* effectKey) {
    return CShaderEffectManager::s_shaderList.Ptr(effectKey);
}
