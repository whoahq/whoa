#include "sound/SI2.hpp"
#include "ui/FrameScript.hpp"

void SI2::RegisterScriptFunctions() {
    for (int32_t i = 0; i < NUM_SCRIPT_FUNCTIONS_SI2; ++i) {
        FrameScript_RegisterFunction(
            SI2::s_ScriptFunctions[i].name,
            SI2::s_ScriptFunctions[i].method
        );
    }
}
