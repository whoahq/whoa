#ifndef SOUND_SE_SOUND_INTERNAL_HPP
#define SOUND_SE_SOUND_INTERNAL_HPP

#include <storm/List.hpp>
#include <fmod.hpp>
#include <cstdint>

class SESound;
class SFile;

struct SoundCacheNode : TSLinkedNode<SoundCacheNode> {
    // Member variables
    FMOD::Sound* sound;
    int32_t loaded;
    char filename[128];
    uint32_t hashval;
    // TODO dword94
    // TODO dword98
};

class SESoundInternal : public TSLinkedNode<SESoundInternal> {
    public:
        // Member variables
        // TODO
        FMOD::System* m_fmodSystem;
        FMOD::Channel* m_fmodChannel = nullptr;
        SESound* m_sound = nullptr;
        // TODO
        int32_t m_useCache = 0;
        int32_t m_type = 0;
        // TODO
        FMOD_MODE m_fmodMode = FMOD_DEFAULT;
        uint8_t m_playing = 0;
        // TODO
        int32_t m_nonblockingReady = 0;
        // TODO
        uint32_t m_uniqueID;

        // Member functions
        SESoundInternal();
        void Play();
};

class SEDiskSound : public SESoundInternal {
    public:
        // Member variables
        // TODO
        SFile* m_file = nullptr;
        // TODO
        FMOD::Sound* m_fmodSound = nullptr;
        // TODO
        SoundCacheNode* m_cacheNode = nullptr;

        // Member functions
        SEDiskSound();
        void Abort(FMOD_RESULT result);
        void CompleteNonBlockingLoad();
};

class SEMemorySound : public SESoundInternal {
    public:
        // TODO
};

class SEStreamedSound : public SESoundInternal {
    public:
        // TODO
};

#endif
