#ifndef UI_GAME_C_G_PET_INFO_HPP
#define UI_GAME_C_G_PET_INFO_HPP

#include "util/GUID.hpp"
#include <cstdint>

class CGPetInfo {
    public:
        // Static functions
        static WOWGUID GetPet(uint32_t index);
};

#endif
