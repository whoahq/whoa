#include "ui/game/CGCharacterModelBaseScript.hpp"
#include "ui/FrameScript.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t Script_SetUnit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetCreature(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RefreshUnit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetRotation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

FrameScript_Method CGCharacterModelBaseMethods[] = {
    { "SetUnit",        &Script_SetUnit },
    { "SetCreature",    &Script_SetCreature },
    { "RefreshUnit",    &Script_RefreshUnit },
    { "SetRotation",    &Script_SetRotation },
};
