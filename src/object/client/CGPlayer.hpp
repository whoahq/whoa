#ifndef OBJECT_CLIENT_CG_PLAYER_HPP
#define OBJECT_CLIENT_CG_PLAYER_HPP

#include "object/Types.hpp"
#include "util/GUID.hpp"
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

struct CSkillInfo {
    uint16_t skillLineID;
    uint16_t skillStep;
    uint16_t skillRank;
    uint16_t skillMaxRank;
    int16_t skillTempModifier;
    int16_t skillPermModifier;
};

struct CArenaTeamInfo {
    uint32_t field1;
    uint32_t field2;
    uint32_t field3;
    uint32_t field4;
    uint32_t field5;
    uint32_t field6;
    uint32_t field7;
};

struct CGPlayerData {
    WOWGUID duelArbiter;
    uint32_t flags;
    uint32_t guildID;
    uint32_t guildRank;
    uint8_t skinID;
    uint8_t faceID;
    uint8_t hairStyleID;
    uint8_t hairColorID;
    uint8_t facialHairStyleID;
    uint8_t bytes_2_2; // TODO
    uint8_t bytes_2_3; // TODO
    uint8_t restState;
    uint8_t bytes_3_1; // TODO
    uint8_t bytes_3_2; // TODO
    uint8_t bytes_3_3; // TODO
    uint8_t bytes_3_4; // TODO
    uint32_t duelTeam;
    int32_t guildTimestamp;
    CQuestLogData questLog[25];
    CVisibleItemData visibleItems[19];
    int32_t chosenTitle;
    int32_t fakeInebriation;
    int32_t pad1;
    WOWGUID invSlots[NUM_INVENTORY_SLOTS];
    WOWGUID packSlots[16];
    WOWGUID bankSlots[28];
    WOWGUID bankBagSlots[7];
    WOWGUID vendorBuybackSlots[12];
    WOWGUID keyringSlots[32];
    WOWGUID currencyTokenSlots[32];
    WOWGUID farsightObject;
    WOWGUID knownTitles;
    WOWGUID knownTitles2;
    WOWGUID knownTitles3;
    WOWGUID knownCurrencies;
    uint32_t xp;
    uint32_t nextLevelXP;
    CSkillInfo skillInfo[128];
    int32_t characterPoints[2];
    uint32_t trackCreatureMask;
    uint32_t trackResourceMask;
    float blockPercentage;
    float dodgePercentage;
    float parryPercentage;
    int32_t expertise;
    int32_t offhandExpertise;
    float critPercentage;
    float rangedCritPercentage;
    float offhandCritPercentage;
    float spellCritPercentage[7];
    int32_t shieldBlock;
    float shieldBlockCritPercentage;
    uint8_t exploredZones[512];
    uint32_t restStateXP;
    uint32_t coinage;
    int32_t modDamageDonePos[7];
    int32_t modDamageDoneNeg[7];
    float modDamageDonePct[7];
    uint32_t modHealingDonePos;
    float modHealingPct;
    float modHealingDonePct;
    int32_t modTargetResistance;
    int32_t modTargetPhysicalResistance;
    uint8_t field_bytes_1; // TODO
    uint8_t field_bytes_2; // TODO
    uint8_t field_bytes_3; // TODO
    uint8_t field_bytes_4; // TODO
    int32_t ammoID;
    int32_t selfResSpell;
    uint32_t pvpMedals;
    uint32_t buybackPrice[12];
    uint32_t buybackTimestamp[12];
    uint16_t kills[2];
    uint32_t todayContribution;
    uint32_t yesterdayContribution;
    uint32_t lifetimeHonorableKills;
    uint8_t field_bytes_2_1; // TODO
    uint8_t field_bytes_2_2; // TODO
    uint8_t field_bytes_2_3; // TODO
    uint8_t field_bytes_2_4; // TODO
    int32_t watchedFactionIndex;
    uint32_t combatRating[25];
    CArenaTeamInfo arenaTeamInfo[3];
    uint32_t honorCurrency;
    uint32_t arenaCurrency;
    uint32_t maxLevel;
    uint32_t dailyQuests[25];
    float runeRegen[4];
    uint32_t noReagentCost[3];
    uint32_t glyphSlots[6];
    uint32_t glyphs[6];
    int32_t glyphsEnabled;
    uint32_t petSpellPower;
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

        // Public member functions
        uint32_t GetMoney() const;
        uint32_t GetNextLevelXP() const;
        uint32_t GetXP() const;

    protected:
        // Protected member variables
        CGPlayerData* m_player;
        uint32_t* m_playerSaved;

        // Protected member functions
        CGPlayerData* Player() const;
};

#endif
