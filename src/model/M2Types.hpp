#ifndef MODEL_M2_TYPES_HPP
#define MODEL_M2_TYPES_HPP

#include "M2Data.hpp"

class CM2Model;
class CShaderEffect;

enum M2BLEND {
    M2BLEND_OPAQUE = 0x0,
    M2BLEND_ALPHA_KEY = 0x1,
    M2BLEND_ALPHA = 0x2,
    M2BLEND_NO_ALPHA_ADD = 0x3,
    M2BLEND_ADD = 0x4,
    M2BLEND_MOD = 0x5,
    M2BLEND_MOD_2X = 0x6,
    M2BLEND_COUNT = 0x7,
};

enum M2COMBINER {
    M2COMBINER_OPAQUE = 0x0,
    M2COMBINER_MOD = 0x1,
    M2COMBINER_DECAL = 0x2,
    M2COMBINER_ADD = 0x3,
    M2COMBINER_MOD2X = 0x4,
    M2COMBINER_FADE = 0x5,
    M2COMBINER_MOD2X_NA = 0x6,
    M2COMBINER_ADD_NA = 0x7,
    M2COMBINER_OP_MASK = 0x7,
    M2COMBINER_ENVMAP = 0x8,
    M2COMBINER_STAGE_SHIFT = 0x4,
};

enum M2LIGHTTYPE {
    M2LIGHT_0 = 0,
    M2LIGHT_1 = 1
};

enum M2PASS {
    M2PASS_0 = 0,
    M2PASS_1 = 1,
    M2PASS_2 = 2,
    M2PASS_COUNT = 3
};

struct M2Element {
    int32_t type;
    CM2Model* model;
    uint32_t flags;
    float alpha;
    float float10;
    float float14;
    int32_t index;
    int32_t priorityPlane;
    M2Batch* batch;
    M2SkinSection* skinSection;
    CShaderEffect* effect;
    uint32_t vertexPermute;
    uint32_t pixelPermute;
};

#endif
