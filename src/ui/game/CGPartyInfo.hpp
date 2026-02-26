#ifndef UI_GAME_C_G_PARTY_INFO_HPP
#define UI_GAME_C_G_PARTY_INFO_HPP

#include "util/GUID.hpp"

class CGPartyInfo {
    public:
        // Public static functions
        static uint32_t NumMembers();

    private:
        // Private static variables
        static WOWGUID m_members[];
};

#endif
