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
    INVSLOT_BAG0        = 19,
    INVSLOT_BAGFIRST    = INVSLOT_BAG0,
    INVSLOT_BAG1        = 20,
    INVSLOT_BAG2        = 21,
    INVSLOT_BAG3        = 22,
    INVSLOT_BAGLAST     = INVSLOT_BAG3,
    INVSLOT_LAST        = INVSLOT_BAG3,

    // TODO

    NUM_INVENTORY_SLOTS = INVSLOT_LAST - INVSLOT_FIRST + 1,
    NUM_BAG_SLOTS       = INVSLOT_BAGLAST - INVSLOT_BAGFIRST + 1,
};

enum OBJECT_TYPE {
    TYPE_OBJECT         = 0x1,
    TYPE_ITEM           = 0x2,
    TYPE_CONTAINER      = 0x4,
    TYPE_UNIT           = 0x8,
    TYPE_PLAYER         = 0x10,
    TYPE_GAMEOBJECT     = 0x20,
    TYPE_DYNAMICOBJECT  = 0x40,
    TYPE_CORPSE         = 0x80,
    // TODO
};

enum OBJECT_TYPE_ID {
    ID_OBJECT           = 0,
    ID_ITEM             = 1,
    ID_CONTAINER        = 2,
    ID_UNIT             = 3,
    ID_PLAYER           = 4,
    ID_GAMEOBJECT       = 5,
    ID_DYNAMICOBJECT    = 6,
    ID_CORPSE           = 7,
    NUM_CLIENT_OBJECT_TYPES,
    // TODO
};

enum SHEATHE_TYPE {
    SHEATHE_0           = 0,
    SHEATHE_1           = 1,
    SHEATHE_2           = 2,
    SHEATHE_3           = 3,
    SHEATHE_4           = 4,
    NUM_SHEATHE_TYPES
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
