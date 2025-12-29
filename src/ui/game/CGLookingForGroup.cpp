#include "ui/game/CGLookingForGroup.hpp"

uint8_t g_validClassRoles[] = {
    CLASS_ROLE_INVALID,                                                         // Non-existent
    CLASS_ROLE_VALID | CLASS_ROLE_TANK | CLASS_ROLE_DAMAGE,                     // Warrior
    CLASS_ROLE_VALID | CLASS_ROLE_TANK | CLASS_ROLE_HEALER | CLASS_ROLE_DAMAGE, // Paladin
    CLASS_ROLE_VALID | CLASS_ROLE_DAMAGE,                                       // Hunter
    CLASS_ROLE_VALID | CLASS_ROLE_DAMAGE,                                       // Rogue
    CLASS_ROLE_VALID | CLASS_ROLE_HEALER | CLASS_ROLE_DAMAGE,                   // Priest
    CLASS_ROLE_VALID | CLASS_ROLE_TANK | CLASS_ROLE_DAMAGE,                     // Death Knight
    CLASS_ROLE_VALID | CLASS_ROLE_HEALER | CLASS_ROLE_DAMAGE,                   // Shaman
    CLASS_ROLE_VALID | CLASS_ROLE_DAMAGE,                                       // Mage
    CLASS_ROLE_VALID | CLASS_ROLE_DAMAGE,                                       // Warlock
    CLASS_ROLE_INVALID,                                                         // Non-existent
    CLASS_ROLE_VALID | CLASS_ROLE_TANK | CLASS_ROLE_HEALER | CLASS_ROLE_DAMAGE, // Druid
    CLASS_ROLE_INVALID,                                                         // Non-existent
};

uint8_t CGLookingForGroup::GetClassRoles(int32_t classID) {
    return g_validClassRoles[classID];
}
