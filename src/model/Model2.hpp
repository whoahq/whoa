#ifndef MODEL_MODEL2_HPP
#define MODEL_MODEL2_HPP

#include "model/CM2Light.hpp"
#include "model/CM2Model.hpp"
#include "model/CM2Scene.hpp"

class C3Vector;

int32_t M2ConvertModelFileName(const char*, char*, uint32_t, uint32_t);

CM2Scene* M2CreateScene(void);

uint32_t M2GetCacheFlags(void);

void M2Initialize(uint16_t flags, uint32_t a2);

uint32_t M2RegisterCVars();

void M2SetGlobalOptFlags(uint16_t flags);

int32_t ModelBlobQuery(const char*, C3Vector&, C3Vector&);

#endif
