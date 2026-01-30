#include "ui/game/ScriptEvents.hpp"
#include "ui/FrameScript.hpp"
#include "ui/ScriptFunctionsSystem.hpp"
#include "util/Unimplemented.hpp"

namespace {

int32_t Script_UnitExists(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsVisible(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsUnit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsPlayer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsInMyGuild(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsCorpse(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsPartyLeader(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitGroupRolesAssigned(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsRaidOfficer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitInParty(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitPlayerOrPetInParty(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitInRaid(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitPlayerOrPetInRaid(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitPlayerControlled(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsAFK(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsDND(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsPVP(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsPVPSanctuary(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsPVPFreeForAll(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitFactionGroup(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitReaction(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsEnemy(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsFriend(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitCanCooperate(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitCanAssist(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitCanAttack(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsCharmed(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsPossessed(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PlayerCanTeleport(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitClassification(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitSelectionColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitGUID(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitPVPName(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitXP(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitXPMax(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitHealth(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitHealthMax(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitMana(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitManaMax(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitPower(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitPowerMax(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitPowerType(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitOnTaxi(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsFeignDeath(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsDead(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsGhost(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsDeadOrGhost(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsConnected(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitAffectingCombat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitSex(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitLevel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMoney(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetHonorCurrency(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetArenaCurrency(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitRace(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitClass(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitClassBase(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitResistance(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitStat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitAttackBothHands(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitDamage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitRangedDamage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitRangedAttack(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitAttackSpeed(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitAttackPower(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitRangedAttackPower(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitDefense(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitArmor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitCharacterPoints(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitBuff(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitDebuff(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitAura(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsTapped(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsTappedByPlayer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsTappedByAllThreatList(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsTrivial(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitHasRelicSlot(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_SetPortraitTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_HasFullControl(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetComboPoints(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsInGuild(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsGuildLeader(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsArenaTeamCaptain(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsInArenaTeam(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsResting(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCombatRating(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCombatRatingBonus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetMaxCombatRatingBonus(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetDodgeChance(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetBlockChance(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetShieldBlock(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetParryChance(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCritChanceFromAgility(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSpellCritChanceFromIntellect(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetCritChance(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRangedCritChance(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSpellCritChance(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSpellBonusDamage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSpellBonusHealing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPetSpellBonusDamage(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetSpellPenetration(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetArmorPenetration(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetAttackPowerForStat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitCreatureType(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitCreatureFamily(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetResSicknessDuration(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPVPSessionStats(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPVPYesterdayStats(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPVPLifetimeStats(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitPVPRank(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPVPRankInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPVPRankProgress(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitCastingInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitChannelInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsLoggedIn(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsFlyableArea(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsIndoors(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsOutdoors(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsOutOfBounds(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsFalling(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsSwimming(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsFlying(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsMounted(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsStealthed(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsSameServer(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetUnitHealthModifier(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetUnitMaxHealthModifier(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetUnitPowerModifier(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetUnitHealthRegenRateFromSpirit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetUnitManaRegenRateFromSpirit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetManaRegen(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPowerRegen(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRuneCooldown(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRuneCount(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetRuneType(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_ReportPlayerIsPVPAFK(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_PlayerIsPVPInactive(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetExpertise(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetExpertisePercent(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitInBattleground(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitInRange(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetUnitSpeed(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetUnitPitch(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitInVehicle(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitUsingVehicle(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitControllingVehicle(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitInVehicleControlSeat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitHasVehicleUI(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitTargetsVehicleInRaidUI(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitVehicleSkin(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitVehicleSeatCount(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitVehicleSeatInfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitSwitchToVehicleSeat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CanSwitchVehicleSeat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetVehicleUIIndicator(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetVehicleUIIndicatorSeat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitThreatSituation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitDetailedThreatSituation(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsControlling(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_EjectPassengerFromSeat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_CanEjectPassengerFromSeat(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_RespondInstanceLock(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPlayerFacing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetPlayerInfoByGUID(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetItemStats(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_GetItemStatDelta(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_IsXPUserDisabled(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_FillLocalizedClassList(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

}

static FrameScript_Method s_UnitFunctions[] = {
    { "UnitExists",                     &Script_UnitExists },
    { "UnitIsVisible",                  &Script_UnitIsVisible },
    { "UnitIsUnit",                     &Script_UnitIsUnit },
    { "UnitIsPlayer",                   &Script_UnitIsPlayer },
    { "UnitIsInMyGuild",                &Script_UnitIsInMyGuild },
    { "UnitIsCorpse",                   &Script_UnitIsCorpse },
    { "UnitIsPartyLeader",              &Script_UnitIsPartyLeader },
    { "UnitGroupRolesAssigned",         &Script_UnitGroupRolesAssigned },
    { "UnitIsRaidOfficer",              &Script_UnitIsRaidOfficer },
    { "UnitInParty",                    &Script_UnitInParty },
    { "UnitPlayerOrPetInParty",         &Script_UnitPlayerOrPetInParty },
    { "UnitInRaid",                     &Script_UnitInRaid },
    { "UnitPlayerOrPetInRaid",          &Script_UnitPlayerOrPetInRaid },
    { "UnitPlayerControlled",           &Script_UnitPlayerControlled },
    { "UnitIsAFK",                      &Script_UnitIsAFK },
    { "UnitIsDND",                      &Script_UnitIsDND },
    { "UnitIsPVP",                      &Script_UnitIsPVP },
    { "UnitIsPVPSanctuary",             &Script_UnitIsPVPSanctuary },
    { "UnitIsPVPFreeForAll",            &Script_UnitIsPVPFreeForAll },
    { "UnitFactionGroup",               &Script_UnitFactionGroup },
    { "UnitReaction",                   &Script_UnitReaction },
    { "UnitIsEnemy",                    &Script_UnitIsEnemy },
    { "UnitIsFriend",                   &Script_UnitIsFriend },
    { "UnitCanCooperate",               &Script_UnitCanCooperate },
    { "UnitCanAssist",                  &Script_UnitCanAssist },
    { "UnitCanAttack",                  &Script_UnitCanAttack },
    { "UnitIsCharmed",                  &Script_UnitIsCharmed },
    { "UnitIsPossessed",                &Script_UnitIsPossessed },
    { "PlayerCanTeleport",              &Script_PlayerCanTeleport },
    { "UnitClassification",             &Script_UnitClassification },
    { "UnitSelectionColor",             &Script_UnitSelectionColor },
    { "UnitGUID",                       &Script_UnitGUID },
    { "UnitName",                       &Script_UnitName },
    { "UnitPVPName",                    &Script_UnitPVPName },
    { "UnitXP",                         &Script_UnitXP },
    { "UnitXPMax",                      &Script_UnitXPMax },
    { "UnitHealth",                     &Script_UnitHealth },
    { "UnitHealthMax",                  &Script_UnitHealthMax },
    { "UnitMana",                       &Script_UnitMana },
    { "UnitManaMax",                    &Script_UnitManaMax },
    { "UnitPower",                      &Script_UnitPower },
    { "UnitPowerMax",                   &Script_UnitPowerMax },
    { "UnitPowerType",                  &Script_UnitPowerType },
    { "UnitOnTaxi",                     &Script_UnitOnTaxi },
    { "UnitIsFeignDeath",               &Script_UnitIsFeignDeath },
    { "UnitIsDead",                     &Script_UnitIsDead },
    { "UnitIsGhost",                    &Script_UnitIsGhost },
    { "UnitIsDeadOrGhost",              &Script_UnitIsDeadOrGhost },
    { "UnitIsConnected",                &Script_UnitIsConnected },
    { "UnitAffectingCombat",            &Script_UnitAffectingCombat },
    { "UnitSex",                        &Script_UnitSex },
    { "UnitLevel",                      &Script_UnitLevel },
    { "GetMoney",                       &Script_GetMoney },
    { "GetHonorCurrency",               &Script_GetHonorCurrency },
    { "GetArenaCurrency",               &Script_GetArenaCurrency },
    { "UnitRace",                       &Script_UnitRace },
    { "UnitClass",                      &Script_UnitClass },
    { "UnitClassBase",                  &Script_UnitClassBase },
    { "UnitResistance",                 &Script_UnitResistance },
    { "UnitStat",                       &Script_UnitStat },
    { "UnitAttackBothHands",            &Script_UnitAttackBothHands },
    { "UnitDamage",                     &Script_UnitDamage },
    { "UnitRangedDamage",               &Script_UnitRangedDamage },
    { "UnitRangedAttack",               &Script_UnitRangedAttack },
    { "UnitAttackSpeed",                &Script_UnitAttackSpeed },
    { "UnitAttackPower",                &Script_UnitAttackPower },
    { "UnitRangedAttackPower",          &Script_UnitRangedAttackPower },
    { "UnitDefense",                    &Script_UnitDefense },
    { "UnitArmor",                      &Script_UnitArmor },
    { "UnitCharacterPoints",            &Script_UnitCharacterPoints },
    { "UnitBuff",                       &Script_UnitBuff },
    { "UnitDebuff",                     &Script_UnitDebuff },
    { "UnitAura",                       &Script_UnitAura },
    { "UnitIsTapped",                   &Script_UnitIsTapped },
    { "UnitIsTappedByPlayer",           &Script_UnitIsTappedByPlayer },
    { "UnitIsTappedByAllThreatList",    &Script_UnitIsTappedByAllThreatList },
    { "UnitIsTrivial",                  &Script_UnitIsTrivial },
    { "UnitHasRelicSlot",               &Script_UnitHasRelicSlot },
    { "SetPortraitTexture",             &Script_SetPortraitTexture },
    { "HasFullControl",                 &Script_HasFullControl },
    { "GetComboPoints",                 &Script_GetComboPoints },
    { "IsInGuild",                      &Script_IsInGuild },
    { "IsGuildLeader",                  &Script_IsGuildLeader },
    { "IsArenaTeamCaptain",             &Script_IsArenaTeamCaptain },
    { "IsInArenaTeam",                  &Script_IsInArenaTeam },
    { "IsResting",                      &Script_IsResting },
    { "GetCombatRating",                &Script_GetCombatRating },
    { "GetCombatRatingBonus",           &Script_GetCombatRatingBonus },
    { "GetMaxCombatRatingBonus",        &Script_GetMaxCombatRatingBonus },
    { "GetDodgeChance",                 &Script_GetDodgeChance },
    { "GetBlockChance",                 &Script_GetBlockChance },
    { "GetShieldBlock",                 &Script_GetShieldBlock },
    { "GetParryChance",                 &Script_GetParryChance },
    { "GetCritChanceFromAgility",       &Script_GetCritChanceFromAgility },
    { "GetSpellCritChanceFromIntellect", &Script_GetSpellCritChanceFromIntellect },
    { "GetCritChance",                  &Script_GetCritChance },
    { "GetRangedCritChance",            &Script_GetRangedCritChance },
    { "GetSpellCritChance",             &Script_GetSpellCritChance },
    { "GetSpellBonusDamage",            &Script_GetSpellBonusDamage },
    { "GetSpellBonusHealing",           &Script_GetSpellBonusHealing },
    { "GetPetSpellBonusDamage",         &Script_GetPetSpellBonusDamage },
    { "GetSpellPenetration",            &Script_GetSpellPenetration },
    { "GetArmorPenetration",            &Script_GetArmorPenetration },
    { "GetAttackPowerForStat",          &Script_GetAttackPowerForStat },
    { "UnitCreatureType",               &Script_UnitCreatureType },
    { "UnitCreatureFamily",             &Script_UnitCreatureFamily },
    { "GetResSicknessDuration",         &Script_GetResSicknessDuration },
    { "GetPVPSessionStats",             &Script_GetPVPSessionStats },
    { "GetPVPYesterdayStats",           &Script_GetPVPYesterdayStats },
    { "GetPVPLifetimeStats",            &Script_GetPVPLifetimeStats },
    { "UnitPVPRank",                    &Script_UnitPVPRank },
    { "GetPVPRankInfo",                 &Script_GetPVPRankInfo },
    { "GetPVPRankProgress",             &Script_GetPVPRankProgress },
    { "UnitCastingInfo",                &Script_UnitCastingInfo },
    { "UnitChannelInfo",                &Script_UnitChannelInfo },
    { "IsLoggedIn",                     &Script_IsLoggedIn },
    { "IsFlyableArea",                  &Script_IsFlyableArea },
    { "IsIndoors",                      &Script_IsIndoors },
    { "IsOutdoors",                     &Script_IsOutdoors },
    { "IsOutOfBounds",                  &Script_IsOutOfBounds },
    { "IsFalling",                      &Script_IsFalling },
    { "IsSwimming",                     &Script_IsSwimming },
    { "IsFlying",                       &Script_IsFlying },
    { "IsMounted",                      &Script_IsMounted },
    { "IsStealthed",                    &Script_IsStealthed },
    { "UnitIsSameServer",               &Script_UnitIsSameServer },
    { "GetUnitHealthModifier",          &Script_GetUnitHealthModifier },
    { "GetUnitMaxHealthModifier",       &Script_GetUnitMaxHealthModifier },
    { "GetUnitPowerModifier",           &Script_GetUnitPowerModifier },
    { "GetUnitHealthRegenRateFromSpirit", &Script_GetUnitHealthRegenRateFromSpirit },
    { "GetUnitManaRegenRateFromSpirit", &Script_GetUnitManaRegenRateFromSpirit },
    { "GetManaRegen",                   &Script_GetManaRegen },
    { "GetPowerRegen",                  &Script_GetPowerRegen },
    { "GetRuneCooldown",                &Script_GetRuneCooldown },
    { "GetRuneCount",                   &Script_GetRuneCount },
    { "GetRuneType",                    &Script_GetRuneType },
    { "ReportPlayerIsPVPAFK",           &Script_ReportPlayerIsPVPAFK },
    { "PlayerIsPVPInactive",            &Script_PlayerIsPVPInactive },
    { "GetExpertise",                   &Script_GetExpertise },
    { "GetExpertisePercent",            &Script_GetExpertisePercent },
    { "UnitInBattleground",             &Script_UnitInBattleground },
    { "UnitInRange",                    &Script_UnitInRange },
    { "GetUnitSpeed",                   &Script_GetUnitSpeed },
    { "GetUnitPitch",                   &Script_GetUnitPitch },
    { "UnitInVehicle",                  &Script_UnitInVehicle },
    { "UnitUsingVehicle",               &Script_UnitUsingVehicle },
    { "UnitControllingVehicle",         &Script_UnitControllingVehicle },
    { "UnitInVehicleControlSeat",       &Script_UnitInVehicleControlSeat },
    { "UnitHasVehicleUI",               &Script_UnitHasVehicleUI },
    { "UnitTargetsVehicleInRaidUI",     &Script_UnitTargetsVehicleInRaidUI },
    { "UnitVehicleSkin",                &Script_UnitVehicleSkin },
    { "UnitVehicleSeatCount",           &Script_UnitVehicleSeatCount },
    { "UnitVehicleSeatInfo",            &Script_UnitVehicleSeatInfo },
    { "UnitSwitchToVehicleSeat",        &Script_UnitSwitchToVehicleSeat },
    { "CanSwitchVehicleSeat",           &Script_CanSwitchVehicleSeat },
    { "GetVehicleUIIndicator",          &Script_GetVehicleUIIndicator },
    { "GetVehicleUIIndicatorSeat",      &Script_GetVehicleUIIndicatorSeat },
    { "UnitThreatSituation",            &Script_UnitThreatSituation },
    { "UnitDetailedThreatSituation",    &Script_UnitDetailedThreatSituation },
    { "UnitIsControlling",              &Script_UnitIsControlling },
    { "EjectPassengerFromSeat",         &Script_EjectPassengerFromSeat },
    { "CanEjectPassengerFromSeat",      &Script_CanEjectPassengerFromSeat },
    { "RespondInstanceLock",            &Script_RespondInstanceLock },
    { "GetPlayerFacing",                &Script_GetPlayerFacing },
    { "GetPlayerInfoByGUID",            &Script_GetPlayerInfoByGUID },
    { "GetItemStats",                   &Script_GetItemStats },
    { "GetItemStatDelta",               &Script_GetItemStatDelta },
    { "IsXPUserDisabled",               &Script_IsXPUserDisabled },
    { "FillLocalizedClassList",         &Script_FillLocalizedClassList },
};

void ScriptEventsRegisterFunctions() {
    SystemRegisterFunctions();

    for (auto& func : s_UnitFunctions) {
        FrameScript_RegisterFunction(func.name, func.method);
    }
}
