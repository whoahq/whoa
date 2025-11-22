#ifndef SOUND_SI2_HPP
#define SOUND_SI2_HPP

#include "SOUNDKITLOOKUP.hpp"
#include "sound/SI2Script.hpp"
#include "storm/hash/Hashkey.hpp"
#include "storm/hash/TSHashTable.hpp"
#include "ui/Types.hpp"
#include <cstdint>
#include <storm/Array.hpp>

class SOUNDKITDEF;

class SI2 {
    public:
        // Static variables
        static FrameScript_Method s_ScriptFunctions[NUM_SCRIPT_FUNCTIONS_SI2];
        static TSGrowableArray<SOUNDKITDEF*> s_SoundKitDefs;
        static TSHashTable<SOUNDKITLOOKUP, HASHKEY_CONSTSTRI> s_SoundKitLookupTable;

        // Static functions
        static int32_t Init(int32_t a1);
        static void InitSoundKitDefs();
        static void RegisterCVars();
        static void RegisterScriptFunctions();
};

#endif
