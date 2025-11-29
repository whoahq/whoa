#ifndef SOUND_SE_CHANNEL_GROUP_HPP
#define SOUND_SE_CHANNEL_GROUP_HPP

#include <cstdint>

class SEChannelGroup {
    public:
        // Member variables
        uint32_t m_nameHash;
        int32_t m_parentChannelGroup;
        float m_volume;
        float m_muteVolume;
        bool m_dirty;
};

#endif
