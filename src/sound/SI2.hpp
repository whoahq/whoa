#ifndef SOUND_SI2_HPP
#define SOUND_SI2_HPP

#include "sound/SI2Script.hpp"
#include "storm/hash/Hashkey.hpp"
#include "storm/hash/TSHashTable.hpp"
#include "ui/Types.hpp"
#include <storm/Array.hpp>
#include <cstdint>

class SOUNDKITDEF;
struct SOUNDKITLOOKUP;

class SI2 {
    public:
        // Static variables
        static FrameScript_Method s_ScriptFunctions[NUM_SCRIPT_FUNCTIONS_SI2];
        static TSGrowableArray<SOUNDKITDEF*> s_SoundKitDefs;
        static TSHashTable<SOUNDKITLOOKUP, HASHKEY_CONSTSTRI> s_SoundKitLookupTable;

        // Static functions
        static SOUNDKITDEF* GetSoundKitDef(int32_t id);
        static int32_t Init(int32_t a1);
        static void InitSoundKitDefs();
        static void InitSoundKitGroups();
        static void RegisterCVars();
        static void RegisterScriptFunctions();
};

#endif
