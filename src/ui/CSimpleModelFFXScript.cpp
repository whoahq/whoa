#include "ui/CSimpleModelFFXScript.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t CSimpleModelFFX_ResetLights(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleModelFFX_AddLight(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleModelFFX_AddCharacterLight(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleModelFFX_AddPetLight(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

FrameScript_Method SimpleModelFFXMethods[NUM_SIMPLE_MODEL_FFX_SCRIPT_METHODS] = {
    { "ResetLights",        &CSimpleModelFFX_ResetLights },
    { "AddLight",           &CSimpleModelFFX_AddLight },
    { "AddCharacterLight",  &CSimpleModelFFX_AddCharacterLight },
    { "AddPetLight",        &CSimpleModelFFX_AddPetLight }
};
