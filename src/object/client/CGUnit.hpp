#ifndef OBJECT_CLIENT_CG_UNIT_HPP
#define OBJECT_CLIENT_CG_UNIT_HPP

#include "util/GUID.hpp"
#include <cstdint>

struct CGUnitData {
    WOWGUID charm;
    WOWGUID summon;
    WOWGUID critter;
    WOWGUID charmedBy;
    WOWGUID summonedBy;
    WOWGUID createdBy;
    WOWGUID target;
    WOWGUID channelObject;
    int32_t channelSpell;
    int32_t pad1;
    int32_t health;
    int32_t power[7];
    int32_t maxHealth;
    int32_t maxPower[7];
    float powerRegenFlatModifier[7];
    int32_t powerRegenInterruptedFlatModifier[7];
    int32_t level;
    int32_t factionTemplate;
    int32_t virtualItemSlotID[3];
    uint32_t flags;
    uint32_t flags2;
    uint32_t auraState;
    uint32_t attackRoundBaseTime[2];
    uint32_t rangedAttackTime;
    float boundingRadius;
    float combatReach;
    int32_t displayID;
    int32_t nativeDisplayID;
    int32_t mountDisplayID;
    float minDamage;
    float maxDamage;
    uint32_t minOffhandDamage;
    uint32_t maxOffhandDamage;
    int32_t pad2;
    uint32_t petNumber;
    uint32_t petNameTimestamp;
    uint32_t petExperience;
    uint32_t petNextLevelExperience;
    uint32_t dynamicFlags;
    float modCastingSpeed;
    int32_t createdBySpell;
    uint32_t npcFlags;
    uint32_t emoteState;
    int32_t stats[5];
    int32_t posStats[5];
    int32_t negStats[5];
    int32_t resistance[7];
    int32_t resistanceBuffModsPositive[7];
    int32_t resistanceBuffModsNegative[7];
    int32_t baseMana;
    int32_t baseHealth;
    int32_t pad3;
    int32_t attackPower;
    int32_t attackPowerMods;
    int32_t attackPowerMultiplier;
    int32_t rangedAttackPower;
    int32_t rangedAttackPowerMods;
    int32_t rangedAttackPowerMultiplier;
    float minRangedDamage;
    float maxRangedDamage;
    int32_t powerCostModifier[7];
    int32_t powerCostMultiplier[7];
    int32_t maxHealthModifier;
    float hoverHeight;
    int32_t pad4;
};

class CGUnit {
    public:
        // Public static functions
        static uint32_t GetBaseOffset();
        static uint32_t GetBaseOffsetSaved();
        static uint32_t GetDataSize();
        static uint32_t GetDataSizeSaved();
        static uint32_t TotalFields();
        static uint32_t TotalFieldsSaved();

    protected:
        // Protected member variables
        CGUnitData* m_unit;
        uint32_t* m_unitSaved;

        // Protected member functions
        CGUnitData* Unit() const;
};

#endif
