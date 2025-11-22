#ifndef SOUND_SOUND_KIT_DEF_HPP
#define SOUND_SOUND_KIT_DEF_HPP

#include "db/Db.hpp"
#include <cstdint>

namespace FMOD {
    class SoundGroup;
}

class SOUNDKITDEF {
    public:
        const char* name;
        const char* files[10] = {};
        float minDistance;
        float distanceCutoff;
        float volume;
        int32_t ID;
        uint32_t flags;
        int32_t fileCount = 0;
        int32_t selectedIndex = -1;
        // TODO: 0x48 - 0x94
        FMOD::SoundGroup* soundGroup1;
        FMOD::SoundGroup* soundGroup2;
        // TODO: 0xA0 - 0xA4
        int32_t advancedID = 0;
        SoundEntriesAdvancedRec* advanced;
};

#endif
