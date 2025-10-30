#ifndef OBJECT_TYPES_HPP
#define OBJECT_TYPES_HPP

enum INVENTORY_SLOTS {
    INVSLOT_HEAD        = 0,
    INVSLOT_FIRST       = INVSLOT_HEAD,
    EQUIPPED_FIRST      = INVSLOT_HEAD,
    INVSLOT_NECK        = 1,
    INVSLOT_SHOULDER    = 2,
    INVSLOT_BODY        = 3,
    INVSLOT_CHEST       = 4,
    INVSLOT_WAIST       = 5,
    INVSLOT_LEGS        = 6,
    INVSLOT_FEET        = 7,
    INVSLOT_WRIST       = 8,
    INVSLOT_HAND        = 9,
    INVSLOT_FINGER1     = 10,
    INVSLOT_FINGER2     = 11,
    INVSLOT_TRINKET1    = 12,
    INVSLOT_TRINKET2    = 13,
    INVSLOT_BACK        = 14,
    INVSLOT_MAINHAND    = 15,
    INVSLOT_OFFHAND     = 16,
    INVSLOT_RANGED      = 17,
    INVSLOT_TABARD      = 18,
    EQUIPPED_LAST       = INVSLOT_TABARD,
    // TODO
};

enum UNIT_SEX {
    UNITSEX_MALE        = 0,
    UNITSEX_FEMALE      = 1,
    UNITSEX_NUM_SEXES   = 2,
    UNITSEX_NONE        = 2,
    UNITSEX_LAST,
    UNITSEX_BOTH        = 3,
};

#endif
