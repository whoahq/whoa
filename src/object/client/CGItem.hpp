#ifndef OBJECT_CLIENT_CG_ITEM_HPP
#define OBJECT_CLIENT_CG_ITEM_HPP

#include <cstdint>

struct ItemEnchantment {
    int32_t id;
    int32_t expiration;
    int32_t chargesRemaining;
};

struct CGItemData {
    uint64_t owner;
    uint64_t containedIn;
    uint64_t creator;
    uint64_t giftCreator;
    uint32_t stackCount;
    int32_t expiration;
    int32_t spellCharges[5];
    uint32_t flags;
    ItemEnchantment enchantments[12];
    int32_t propertySeed;
    int32_t randomPropertiesID;
    int32_t durability;
    int32_t maxDurability;
    int32_t createPlayedTime;
    int32_t pad;
};

class CGItem {
    public:
        // Public static functions
        static uint32_t GetBaseOffset();
        static uint32_t GetBaseOffsetSaved();
        static uint32_t GetDataSize();
        static uint32_t GetDataSizeSaved();
        static uint32_t TotalFields();
        static uint32_t TotalFieldsSaved();

        // Public member variables
        CGItemData* m_item;
        uint32_t* m_itemSaved;
};

#endif
