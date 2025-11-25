#ifndef SOUND_SE_SOUND_HPP
#define SOUND_SE_SOUND_HPP

#include "sound/SESoundInternal.hpp"
#include <fmod.hpp>
#include <storm/Hash.hpp>
#include <storm/Thread.hpp>
#include <cstdint>

struct SOUND_INTERNAL_LOOKUP : TSHashObject<SOUND_INTERNAL_LOOKUP, HASHKEY_NONE> {
    SESoundInternal* m_internal;
};

class SESound {
    public:
        // Public static variables
        static SCritSect s_CritSect3;
        static int32_t s_Initialized;
        static SCritSect s_InternalCritSect;
        static STORM_LIST(SESoundInternal) s_InternalList;
        static TSHashTable<SOUND_INTERNAL_LOOKUP, HASHKEY_NONE> s_InternalLookupTable;
        static HASHKEY_NONE s_InternalLookupKey;
        static SCritSect s_LoadingCritSect;
        static FMOD::System* s_pGameSystem;
        static uint32_t s_UniqueID;

        // Public static functions
        static FMOD::SoundGroup* CreateSoundGroup(const char* name, int32_t maxAudible);
        static int32_t Heartbeat(const void* data, void* param);
        static void Init(int32_t maxChannels, int32_t (*a2), int32_t enableReverb, int32_t enableSoftwareHRTF, int32_t* numChannels, int32_t* outputDriverIndex, const char* outputDriverName, void (*a8), int32_t a9);
        static int32_t IsInitialized();
        static void Log_Write(int32_t line, const char* file, FMOD_RESULT result, const char* fmt, ...);

        // Public member functions
        void CompleteLoad();
        int32_t Load(const char* filename, int32_t a3, FMOD::SoundGroup* soundGroup1, FMOD::SoundGroup* soundGroup2, bool a6, bool a7, uint32_t a8, int32_t a9, uint32_t a10);

    private:
        // Private static functions
        static int32_t LoadDiskSound(FMOD::System* fmodSystem, const char* filename, FMOD_MODE fmodMode, SESound* sound, FMOD::SoundGroup* fmodSoundGroup1, FMOD::SoundGroup* fmodSoundGroup2, bool a7, int32_t a8, uint32_t a9, int32_t a10, uint32_t decodeBufferSize, int32_t a12, float a13, float a14, float a15, float* a16);
        static void ProcessLoadedDiskSounds();

        // Private member variables
        SESoundInternal* m_internal = nullptr;
};

#endif
