#ifndef SOUND_SE_SOUND_HPP
#define SOUND_SE_SOUND_HPP

#include "sound/SESoundInternal.hpp"
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
        static int32_t IsInitialized();
        static void Log_Write(int32_t line, const char* file, FMOD_RESULT result, const char* fmt, ...);

        // Member variables
        SESoundInternal* m_internal = nullptr;

        // Member functions
        void CompleteLoad();
        int32_t Load(const char* filename, int32_t a3, FMOD::SoundGroup* soundGroup1, FMOD::SoundGroup* soundGroup2, bool a6, bool a7, uint32_t a8, int32_t a9, uint32_t a10);
};

#endif
