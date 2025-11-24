#ifndef SOUND_SI2_HPP
#define SOUND_SI2_HPP

#include "sound/SI2Script.hpp"
#include "ui/Types.hpp"
#include <storm/Array.hpp>
#include <storm/Hash.hpp>
#include <cstdint>

class SOUNDKITDEF;
struct SOUNDKITLOOKUP;
class SoundKitProperties;

class SI2 {
    public:
        // Static variables
        static FrameScript_Method s_ScriptFunctions[NUM_SCRIPT_FUNCTIONS_SI2];
        static TSGrowableArray<SOUNDKITDEF*> s_SoundKitDefs;
        static TSHashTable<SOUNDKITLOOKUP, HASHKEY_CONSTSTRI> s_SoundKitLookupTable;
        static char s_SoundCategory[32][18];

        // Static functions
        static SOUNDKITDEF* GetSoundKitDef(int32_t id);
        static int32_t GetSoundKitID(const char* name);
        static int32_t Init(int32_t a1);
        static void InitSoundKitDefs();
        static void InitSoundKitGroups();
        static int32_t PlaySoundKit(int32_t id, int32_t a2, void* handle, SoundKitProperties* properties, int32_t a5,  void* a6, int32_t a7, int32_t a8);
        static void PlayUISound(int32_t id);
        static void RegisterCVars();
        static void RegisterUserCVars();
        static void RegisterScriptFunctions();
        static void StartGlueMusic(const char* name);
};

#endif
