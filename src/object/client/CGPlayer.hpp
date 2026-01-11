#ifndef OBJECT_CLIENT_CG_PLAYER_HPP
#define OBJECT_CLIENT_CG_PLAYER_HPP

#include "object/Types.hpp"
#include <cstdint>

struct CQuestLogData {
    int32_t questID;
    uint32_t field2;
    uint32_t field3;
    uint32_t field4;
    uint32_t field5;
};

// TODO is this VisibleItem_C?
struct CVisibleItemData {
    int32_t entryID;
    uint32_t enchantment;
};

struct CGPlayerData {
    uint64_t duelArbiter;
    uint32_t flags;
    uint32_t guildID;
    uint32_t guildRank;
    int32_t pad1;
    int32_t pad2;
    int32_t pad3;
    uint32_t duelTeam;
    int32_t guildTimestamp;
    CQuestLogData questLog[25];
    CVisibleItemData visibleItems[19];
    int32_t chosenTitle;
    int32_t fakeInebriation;
    int32_t pad4;
    uint64_t invSlots[NUM_INVENTORY_SLOTS];
    uint64_t packSlots[16];
    uint64_t bankSlots[28];
    uint64_t bankBagSlots[7];
    uint64_t vendorBuybackSlots[12];
    uint64_t keyringSlots[32];
    uint64_t currencyTokenSlots[32];
    uint64_t farsightObject;
    uint64_t knownTitles;
    uint64_t knownTitles2;
    uint64_t knownTitles3;
    uint64_t knownCurrencies;
    uint32_t xp;
    uint32_t nextLevelXP;
    // TODO
};

class CGPlayer {
    public:
        // Public static functions
        static uint32_t GetBaseOffset();
        static uint32_t GetBaseOffsetSaved();
        static uint32_t GetDataSize();
        static uint32_t GetDataSizeSaved();
        static uint32_t GetRemoteDataSize();
        static uint32_t GetRemoteDataSizeSaved();
        static uint32_t TotalFields();
        static uint32_t TotalRemoteFields();
        static uint32_t TotalFieldsSaved();
        static uint32_t TotalRemoteFieldsSaved();

        // Public member variables
        CGPlayerData* m_player;
        uint32_t* m_playerSaved;
};

#endif
