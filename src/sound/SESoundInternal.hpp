#ifndef SOUND_SE_SOUND_INTERNAL_HPP
#define SOUND_SE_SOUND_INTERNAL_HPP

#include <fmod.hpp>
#include <cstdint>

class SESound;
class SFile;

class SESoundInternal {
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
        // TODO
        int32_t m_loaded = 0;
        // TODO
        uint32_t m_uniqueID;

        // Member functions
        SESoundInternal();
};

class SEDiskSound : public SESoundInternal {
    public:
        // Member variables
        // TODO
        SFile* m_file;
        // TODO
        FMOD::Sound* m_fmodSound;
        // TODO
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
