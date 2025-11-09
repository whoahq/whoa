#include "ui/game/CGVideoOptions.hpp"

void CGVideoOptions::RegisterScriptFunctions() {
    for (int32_t i = 0; i < NUM_SCRIPT_FUNCTIONS_VIDEO_OPTIONS; i++) {
        FrameScript_RegisterFunction(
            CGVideoOptions::s_ScriptFunctions[i].name,
            CGVideoOptions::s_ScriptFunctions[i].method
        );
    }
}

void CGVideoOptions::UnregisterScriptFunctions() {
    // TODO
}
