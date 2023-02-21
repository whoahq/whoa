#include "ui/CSimpleCheckboxScript.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t CSimpleCheckbox_SetChecked(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleCheckbox_GetChecked(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleCheckbox_GetCheckedTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleCheckbox_SetCheckedTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleCheckbox_GetDisabledCheckedTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleCheckbox_SetDisabledCheckedTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

FrameScript_Method SimpleCheckboxMethods[NUM_SIMPLE_CHECKBOX_SCRIPT_METHODS] = {
    { "SetChecked",                  &CSimpleCheckbox_SetChecked },
    { "GetChecked",                  &CSimpleCheckbox_GetChecked },
    { "GetCheckedTexture",           &CSimpleCheckbox_GetCheckedTexture },
    { "SetCheckedTexture",           &CSimpleCheckbox_SetCheckedTexture },
    { "GetDisabledCheckedTexture",   &CSimpleCheckbox_GetDisabledCheckedTexture },
    { "SetDisabledCheckedTexture",   &CSimpleCheckbox_SetDisabledCheckedTexture }
};
