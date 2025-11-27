#ifndef SOUND_SE_SOUND_INTERNAL_HPP
#define SOUND_SE_SOUND_INTERNAL_HPP

#include <storm/List.hpp>
#include <fmod.hpp>
#include <cstdint>

class SESound;
class SEUserData;
class SFile;

class SoundCacheNode : public TSLinkedNode<SoundCacheNode> {
    public:
        // Member variables
        FMOD::Sound* m_fmodSound = nullptr;
        int32_t m_loaded = 0;
        char m_filename[128];
        uint32_t m_hashval = 0;
        // TODO dword94
        // TODO dword98

        // Member functions
        SoundCacheNode();
};

class SESoundInternal : public TSLinkedNode<SESoundInternal> {
    public:
        // Member variables
        // TODO
        FMOD::System* m_fmodSystem;
        FMOD::Channel* m_fmodChannel = nullptr;
        SESound* m_sound = nullptr;
        SEUserData* m_userData = nullptr;
        float m_volume = 1.0f;
        float m_fadeVolume = 0.0f;
        float m_fadeInTime = 0.0f;
        float m_fadeOutTime = 0.0f;
        uint8_t m_fadeIn = 0;
        uint8_t m_fadeOut = 0;
        // TODO
        int32_t m_useCache = 0;
        int32_t m_type = 0;
        // TODO
        FMOD_MODE m_fmodMode = FMOD_DEFAULT;
        uint8_t m_playing = 0;
        uint8_t m_stopped = 0;
        // TODO
        int32_t m_nonblockingReady = 0;
        // TODO
        uint32_t m_uniqueID;

        // Member functions
        SESoundInternal();
        float GetVolume();
        void Play();
};

class SEDiskSound : public SESoundInternal {
    public:
        // Member variables
        // TODO
        TSLink<SEDiskSound> m_readyLink;
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
