#include "ui/game/CGDressUpModelFrameScript.hpp"
#include "ui/FrameScript.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t Script_Undress(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_Dress(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_TryOn(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

FrameScript_Method CGDressUpModelFrameMethods[] = {
    { "Undress",    &Script_Undress },
    { "Dress",      &Script_Dress },
    { "TryOn",      &Script_TryOn },
};
