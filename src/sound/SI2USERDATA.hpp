#ifndef SOUND_SI2_USER_DATA_HPP
#define SOUND_SI2_USER_DATA_HPP

#include "sound/SEUserData.hpp"
#include <cstdint>

class SOUNDKITDEF;

class SI2USERDATA : public SEUserData {
    public:
        // Member variables
        const char* m_name = nullptr;
        int32_t m_ID = 0;
        SOUNDKITDEF* m_def = nullptr;
        int32_t m_type = 0;
        // TODO
};

#endif
