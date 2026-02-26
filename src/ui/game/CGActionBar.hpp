#ifndef UI_GAME_C_G_ACTION_BAR_HPP
#define UI_GAME_C_G_ACTION_BAR_HPP

#include <cstdint>

class CGActionBar {
    public:
        // Public static variables
        static uint32_t s_currentPage;
        static uint32_t s_tempPageActiveFlags;

        // Public static functions
        static uint32_t GetBonusBarOffset();

    private:
        // Private static variables
        static uint32_t s_bonusBarOffset;
};

#endif
