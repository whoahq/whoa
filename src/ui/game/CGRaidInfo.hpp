#ifndef UI_GAME_C_G_RAID_INFO_HPP
#define UI_GAME_C_G_RAID_INFO_HPP

#include <cstdint>

class CGRaidInfo {
    public:
        // Public static functions
        static uint32_t NumMembers();

    private:
        // Private static variables
        static uint32_t s_numMembers;
};

#endif
