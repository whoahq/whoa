#ifndef SOUND_SI2_HPP
#define SOUND_SI2_HPP

#include "sound/SI2Script.hpp"
#include "ui/Types.hpp"
#include <storm/Array.hpp>
#include <cstdint>

class SOUNDKITDEF;

class SI2 {
    public:
        // Static variables
        static FrameScript_Method s_ScriptFunctions[NUM_SCRIPT_FUNCTIONS_SI2];
        static TSGrowableArray<SOUNDKITDEF*> s_SoundKitDefs;

        // Static functions
        static int32_t Init(int32_t a1);
        static void InitSoundKitDefs();
        static void RegisterCVars();
        static void RegisterScriptFunctions();
};

#endif
