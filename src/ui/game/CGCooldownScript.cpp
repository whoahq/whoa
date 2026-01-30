#include "ui/game/CGCooldownScript.hpp"
#include "ui/FrameScript.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t CGCooldown_SetCooldown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGCooldown_SetReverse(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGCooldown_GetReverse(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGCooldown_SetDrawEdge(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CGCooldown_GetDrawEdge(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

FrameScript_Method CGCooldownMethods[] = {
    { "SetCooldown",    &CGCooldown_SetCooldown },
    { "SetReverse",     &CGCooldown_SetReverse },
    { "GetReverse",     &CGCooldown_GetReverse },
    { "SetDrawEdge",    &CGCooldown_SetDrawEdge },
    { "GetDrawEdge",    &CGCooldown_GetDrawEdge },
};
