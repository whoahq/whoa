#ifndef SOUND_SOUND_KIT_PROPERTIES_HPP
#define SOUND_SOUND_KIT_PROPERTIES_HPP

#include <cstdint>

class SoundKitProperties {
    public:
        // Member variables
        uint32_t m_type;
        // TODO
        int32_t int20;
        uint32_t uint24;
        uint32_t uint28;
        // TODO
        uint8_t byte38;
        // TODO
        uint32_t uint3C;
        // TODO
        int32_t m_streaming;
        // TODO

        // Member functions
        void ResetToDefaults();
};

#endif
