#ifndef MODEL_MODEL2_HPP
#define MODEL_MODEL2_HPP

#include "model/CM2Light.hpp"
#include "model/CM2Model.hpp"
#include "model/CM2Scene.hpp"

class C3Vector;

int32_t M2ConvertModelFileName(const char* source, char* dest, uint32_t a3, uint32_t a4);

CM2Scene* M2CreateScene();

uint32_t M2GetCacheFlags();

void M2Initialize(uint16_t flags, uint32_t a2);

uint32_t M2RegisterCVars();

void M2SetGlobalOptFlags(uint16_t flags);

int32_t ModelBlobQuery(const char* a1, C3Vector& a2, C3Vector& a3);

#endif
