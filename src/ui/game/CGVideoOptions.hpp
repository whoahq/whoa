#ifndef UI_GAME_C_G_VIDEO_OPTIONS_HPP
#define UI_GAME_C_G_VIDEO_OPTIONS_HPP

#include "ui/FrameScript.hpp"
#include "ui/game/CGVideoOptionsScript.hpp"

class CGVideoOptions {
    public:
        // Static members
        static FrameScript_Method s_ScriptFunctions[NUM_SCRIPT_FUNCTIONS_VIDEO_OPTIONS];

        // Static functions
        static void RegisterScriptFunctions();
        static void UnregisterScriptFunctions();
};

#endif
