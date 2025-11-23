#ifndef SOUND_SE_SOUND_HPP
#define SOUND_SE_SOUND_HPP

#include <fmod.hpp>
#include <cstdint>

class SESound {
    public:
        // Static variables
        static int32_t s_Initialized;
        static FMOD::System* s_pGameSystem;

        // Static functions
        static FMOD::SoundGroup* CreateSoundGroup(const char* name, int32_t maxAudible);
        static void Init(int32_t maxChannels, int32_t (*a2), int32_t enableReverb, int32_t enableSoftwareHRTF, int32_t* numChannels, int32_t* outputDriverIndex, const char* outputDriverName, void (*a8), int32_t a9);
        static void Log_Write(int32_t line, const char* file, FMOD_RESULT result, const char* fmt, ...);
};

#endif
