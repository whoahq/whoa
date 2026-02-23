#ifndef SOUND_SOUND_KIT_DEF_HPP
#define SOUND_SOUND_KIT_DEF_HPP

#include "db/Db.hpp"
#include <cstdint>

#if !defined(WHOA_SYSTEM_WEB)
namespace FMOD {
    class SoundGroup;
}
#endif

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
#if !defined(WHOA_SYSTEM_WEB)
        FMOD::SoundGroup* soundGroup1;
        FMOD::SoundGroup* soundGroup2;
#else
        void* soundGroup1;
        void* soundGroup2;
#endif
        // TODO: 0xA0 - 0xA4
        int32_t advancedID = 0;
        SoundEntriesAdvancedRec* advanced;
};

#endif
