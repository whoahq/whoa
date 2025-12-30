#ifndef COMPONENT_TYPES_HPP
#define COMPONENT_TYPES_HPP

#include <cstdint>
#include <storm/String.hpp>

class CM2Model;

enum COMP_HAND_SLOT {
    HAND_RIGHT              = 0,
    HAND_LEFT               = 1,
};

enum COMPONENT_GEOSET {
    GEOSET_HEAD             = 0,
    GEOSET_BEARD            = 1,
    GEOSET_SIDEBURNS        = 2,
    GEOSET_MOUSTACHE        = 3,
    GEOSET_GLOVES           = 4,
    GEOSET_BOOTS            = 5,
    GEOSET_SHIRT            = 6,
    GEOSET_EARS             = 7,
    GEOSET_SLEEVES          = 8,
    GEOSET_LEGS             = 9,
    GEOSET_SHIRT_DOUBLET    = 10,
    GEOSET_PANT_DOUBLET     = 11,
    GEOSET_TABARD           = 12,
    GEOSET_ROBE             = 13,
    GEOSET_LOINCLOTH        = 14,
    GEOSET_CAPE             = 15,
    GEOSET_FACIAL_JEWELRY   = 16,
    GEOSET_EYE_EFFECTS      = 17,
    GEOSET_BELT             = 18,
    NUM_GEOSET
};

enum COMPONENT_SECTIONS {
    SECTION_ARM_UPPER       = 0,
    SECTION_ARM_LOWER       = 1,
    SECTION_HAND            = 2,
    SECTION_TORSO_UPPER     = 3,
    SECTION_TORSO_LOWER     = 4,
    SECTION_LEG_UPPER       = 5,
    SECTION_LEG_LOWER       = 6,
    SECTION_FOOT            = 7,
    SECTION_HEAD_UPPER      = 8,
    SECTION_HEAD_LOWER      = 9,
    NUM_COMPONENT_SECTIONS
};

enum COMPONENT_VARIATIONS {
    VARIATION_SKIN          = 0,
    VARIATION_FACE          = 1,
    VARIATION_FACIAL_HAIR   = 2,
    VARIATION_HAIR          = 3,
    VARIATION_UNDERWEAR     = 4,
    NUM_COMPONENT_VARIATIONS
};

enum GEOCOMPONENTLINKS {
    ATTACH_SHIELD           = 0,
    ATTACH_HANDR            = 1,
    ATTACH_HANDL            = 2,
    // TODO
    ATTACH_SHEATH_MAINHAND  = 26,
    ATTACH_SHEATH_OFFHAND   = 27,
    ATTACH_SHEATH_SHIELD    = 28,
    // TODO
    ATTACH_LARGEWEAPONLEFT  = 30,
    ATTACH_LARGEWEAPONRIGHT = 31,
    // TODO
    ATTACH_HIPWEAPONLEFT    = 32,
    ATTACH_HIPWEAPONRIGHT   = 33,
    // TODO
    ATTACH_NONE             = 0xFFFFFFFF,
};

enum ITEM_SLOT {
    ITEMSLOT_0              = 0,
    ITEMSLOT_1              = 1,
    ITEMSLOT_2              = 2,
    ITEMSLOT_3              = 3,
    ITEMSLOT_4              = 4,
    ITEMSLOT_5              = 5,
    ITEMSLOT_6              = 6,
    ITEMSLOT_7              = 7,
    ITEMSLOT_8              = 8,
    ITEMSLOT_9              = 9,
    ITEMSLOT_10             = 10,
    ITEMSLOT_11             = 11,
    NUM_ITEM_SLOT
};

struct CharacterPreferences {
    int32_t raceID;
    int32_t sexID;
    int32_t classID;
    int32_t hairColorID;
    int32_t skinColorID;
    int32_t faceID;
    int32_t facialHairStyleID;
    int32_t hairStyleID;
};

#endif
