#ifndef SOUND_SI2_HPP
#define SOUND_SI2_HPP

#include "sound/SI2Script.hpp"
#include "ui/Types.hpp"
#include <cstdint>

class CVar;

class SI2 {
    public:
        // Static variables
        static CVar* s_pCVar_Sound_EnableReverb;
        static CVar* s_pCVar_Sound_EnableSoftwareHRTF;
        static CVar* s_pCVar_Sound_NumChannels;
        static CVar* s_pCVar_Sound_OutputDriverIndex;
        static CVar* s_pCVar_Sound_OutputDriverName;
        static FrameScript_Method s_ScriptFunctions[NUM_SCRIPT_FUNCTIONS_SI2];

        // Static functions
        static int32_t Init(int32_t a1);
        static void RegisterCVars();
        static void RegisterScriptFunctions();
};

#endif
