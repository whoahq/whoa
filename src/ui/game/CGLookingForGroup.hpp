#ifndef UI_GAME_C_G_LOOKING_FOR_GROUP_HPP
#define UI_GAME_C_G_LOOKING_FOR_GROUP_HPP

#include <cstdint>

#define CLASS_ROLE_INVALID  0x0
#define CLASS_ROLE_VALID    0x1
#define CLASS_ROLE_TANK     0x2
#define CLASS_ROLE_HEALER   0x4
#define CLASS_ROLE_DAMAGE   0x8

extern uint8_t g_validClassRoles[];

class CGLookingForGroup {
    public:
        // Static functions
        static uint8_t GetClassRoles(int32_t classID);
};

#endif
