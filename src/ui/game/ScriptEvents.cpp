#include "ui/game/ScriptEvents.hpp"
#include "db/Db.hpp"
#include "object/Client.hpp"
#include "ui/FrameScript.hpp"
#include "ui/ScriptFunctionsSystem.hpp"
#include "ui/game/CGGameUI.hpp"
#include "ui/game/ScriptUtil.hpp"
#include "ui/game/Types.hpp"
#include "util/GUID.hpp"
#include "util/Lua.hpp"
#include "util/StringTo.hpp"
#include "util/Unimplemented.hpp"

#define NUM_SCRIPT_EVENTS 722

const char* g_scriptEvents[NUM_SCRIPT_EVENTS];

namespace {

int32_t Script_UnitExists(lua_State* L) {
    auto token = lua_tostring(L, 1);
    WOWGUID guid = 0;
    Script_GetGUIDFromToken(token, guid, false);

    auto object = ClntObjMgrObjectPtr(guid, TYPE_OBJECT, __FILE__, __LINE__);

    if ((object && object->CanBeTargetted()) || CGGameUI::IsRaidMemberOrPet(guid)) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

int32_t Script_UnitIsVisible(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsUnit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t Script_UnitIsPlayer(lua_State* L) {
    auto token = lua_tostring(L, 1);
    WOWGUID guid = 0;
    Script_GetGUIDFromToken(token, guid, false);

    auto object = ClntObjMgrObjectPtr(guid, TYPE_PLAYER, __FILE__, __LINE__);

    if (object || CGGameUI::IsRaidMember(guid)) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    return 1;
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
    if (!lua_isstring(L, 1)) {
        luaL_error(L, "Usage: UnitXP(\"unit\")");
        return 0;
    }

    auto name = lua_tostring(L, 1);
    auto unit = Script_GetUnitFromName(name);

    float xp = 0.0f;

    if (unit && unit->IsA(TYPE_PLAYER)) {
        xp = static_cast<CGPlayer_C*>(unit)->GetXP();
    }

    lua_pushnumber(L, xp);

    return 1;
}

int32_t Script_UnitXPMax(lua_State* L) {
    if (!lua_isstring(L, 1)) {
        luaL_error(L, "Usage: UnitXPMax(\"unit\")");
        return 0;
    }

    auto name = lua_tostring(L, 1);
    auto unit = Script_GetUnitFromName(name);

    float xpMax = 0.0f;

    if (unit && unit->IsA(TYPE_PLAYER)) {
        xpMax = static_cast<CGPlayer_C*>(unit)->GetNextLevelXP();
    }

    lua_pushnumber(L, xpMax);

    return 1;
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
    if (lua_type(L, 1) != LUA_TTABLE) {
        luaL_error(L, "Usage: FillLocalizedClassList(classTable[, isFemale])");
        return 0;
    }

    auto isFemale = StringToBOOL(L, 2, 0);
    auto sex = isFemale ? UNITSEX_FEMALE : UNITSEX_MALE;

    lua_settop(L, 1);

    for (int32_t i = 0; i < g_chrClassesDB.GetNumRecords(); ++i) {
        auto classRec = g_chrClassesDB.GetRecordByIndex(i);
        if (classRec) {
            auto displayName = CGUnit_C::GetDisplayClassNameFromRecord(classRec, sex, 0);

            lua_pushstring(L, classRec->m_filename);
            lua_pushstring(L, displayName);

            lua_settable(L, -3);
        }
    }

    return 1;
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

void ScriptEventsInitialize() {
    g_scriptEvents[0] = "UNIT_PET";
    g_scriptEvents[2] = "UNIT_PET";
    g_scriptEvents[12] = "UNIT_TARGET";
    g_scriptEvents[17] = "UNIT_DISPLAYPOWER";
    g_scriptEvents[18] = "UNIT_HEALTH";
    g_scriptEvents[19] = "UNIT_MANA";
    g_scriptEvents[20] = "UNIT_RAGE";
    g_scriptEvents[21] = "UNIT_FOCUS";
    g_scriptEvents[22] = "UNIT_ENERGY";
    g_scriptEvents[23] = "UNIT_HAPPINESS";
    g_scriptEvents[25] = "UNIT_RUNIC_POWER";
    g_scriptEvents[26] = "UNIT_MAXHEALTH";
    g_scriptEvents[27] = "UNIT_MAXMANA";
    g_scriptEvents[28] = "UNIT_MAXRAGE";
    g_scriptEvents[29] = "UNIT_MAXFOCUS";
    g_scriptEvents[30] = "UNIT_MAXENERGY";
    g_scriptEvents[31] = "UNIT_MAXHAPPINESS";
    g_scriptEvents[33] = "UNIT_MAXRUNIC_POWER";
    g_scriptEvents[48] = "UNIT_LEVEL";
    g_scriptEvents[49] = "UNIT_FACTION";
    g_scriptEvents[53] = "UNIT_FLAGS";
    g_scriptEvents[54] = "UNIT_FLAGS";
    g_scriptEvents[56] = "UNIT_ATTACK_SPEED";
    g_scriptEvents[57] = "UNIT_ATTACK_SPEED";
    g_scriptEvents[58] = "UNIT_RANGEDDAMAGE";
    g_scriptEvents[64] = "UNIT_DAMAGE";
    g_scriptEvents[65] = "UNIT_DAMAGE";
    g_scriptEvents[66] = "UNIT_DAMAGE";
    g_scriptEvents[67] = "UNIT_DAMAGE";
    g_scriptEvents[71] = "UNIT_PET_EXPERIENCE";
    g_scriptEvents[72] = "UNIT_PET_EXPERIENCE";
    g_scriptEvents[73] = "UNIT_DYNAMIC_FLAGS";
    g_scriptEvents[78] = "UNIT_STATS";
    g_scriptEvents[79] = "UNIT_STATS";
    g_scriptEvents[80] = "UNIT_STATS";
    g_scriptEvents[81] = "UNIT_STATS";
    g_scriptEvents[82] = "UNIT_STATS";
    g_scriptEvents[93] = "UNIT_RESISTANCES";
    g_scriptEvents[94] = "UNIT_RESISTANCES";
    g_scriptEvents[95] = "UNIT_RESISTANCES";
    g_scriptEvents[96] = "UNIT_RESISTANCES";
    g_scriptEvents[97] = "UNIT_RESISTANCES";
    g_scriptEvents[98] = "UNIT_RESISTANCES";
    g_scriptEvents[99] = "UNIT_RESISTANCES";
    g_scriptEvents[100] = "UNIT_RESISTANCES";
    g_scriptEvents[101] = "UNIT_RESISTANCES";
    g_scriptEvents[102] = "UNIT_RESISTANCES";
    g_scriptEvents[103] = "UNIT_RESISTANCES";
    g_scriptEvents[104] = "UNIT_RESISTANCES";
    g_scriptEvents[105] = "UNIT_RESISTANCES";
    g_scriptEvents[106] = "UNIT_RESISTANCES";
    g_scriptEvents[107] = "UNIT_RESISTANCES";
    g_scriptEvents[108] = "UNIT_RESISTANCES";
    g_scriptEvents[109] = "UNIT_RESISTANCES";
    g_scriptEvents[110] = "UNIT_RESISTANCES";
    g_scriptEvents[111] = "UNIT_RESISTANCES";
    g_scriptEvents[112] = "UNIT_RESISTANCES";
    g_scriptEvents[113] = "UNIT_RESISTANCES";
    g_scriptEvents[117] = "UNIT_ATTACK_POWER";
    g_scriptEvents[118] = "UNIT_ATTACK_POWER";
    g_scriptEvents[119] = "UNIT_ATTACK_POWER";
    g_scriptEvents[120] = "UNIT_RANGED_ATTACK_POWER";
    g_scriptEvents[121] = "UNIT_RANGED_ATTACK_POWER";
    g_scriptEvents[122] = "UNIT_RANGED_ATTACK_POWER";
    g_scriptEvents[123] = "UNIT_RANGEDDAMAGE";
    g_scriptEvents[124] = "UNIT_RANGEDDAMAGE";
    g_scriptEvents[125] = "UNIT_MANA";
    g_scriptEvents[132] = "UNIT_MANA";
    g_scriptEvents[139] = "UNIT_STATS";
    g_scriptEvents[142] = "UNIT_AURA";
    g_scriptEvents[143] = "UNIT_COMBAT";
    g_scriptEvents[144] = "UNIT_NAME_UPDATE";
    g_scriptEvents[145] = "UNIT_PORTRAIT_UPDATE";
    g_scriptEvents[146] = "UNIT_MODEL_CHANGED";
    g_scriptEvents[147] = "UNIT_INVENTORY_CHANGED";
    g_scriptEvents[148] = "UNIT_CLASSIFICATION_CHANGED";
    g_scriptEvents[149] = "UNIT_COMBO_POINTS";
    g_scriptEvents[150] = "ITEM_LOCK_CHANGED";
    g_scriptEvents[151] = "PLAYER_XP_UPDATE";
    g_scriptEvents[152] = "PLAYER_REGEN_DISABLED";
    g_scriptEvents[153] = "PLAYER_REGEN_ENABLED";
    g_scriptEvents[154] = "PLAYER_AURAS_CHANGED";
    g_scriptEvents[155] = "PLAYER_ENTER_COMBAT";
    g_scriptEvents[156] = "PLAYER_LEAVE_COMBAT";
    g_scriptEvents[157] = "PLAYER_TARGET_CHANGED";
    g_scriptEvents[158] = "PLAYER_FOCUS_CHANGED";
    g_scriptEvents[159] = "PLAYER_CONTROL_LOST";
    g_scriptEvents[160] = "PLAYER_CONTROL_GAINED";
    g_scriptEvents[161] = "PLAYER_FARSIGHT_FOCUS_CHANGED";
    g_scriptEvents[162] = "PLAYER_LEVEL_UP";
    g_scriptEvents[163] = "PLAYER_MONEY";
    g_scriptEvents[164] = "PLAYER_DAMAGE_DONE_MODS";
    g_scriptEvents[165] = "PLAYER_TOTEM_UPDATE";
    g_scriptEvents[166] = "ZONE_CHANGED";
    g_scriptEvents[167] = "ZONE_CHANGED_INDOORS";
    g_scriptEvents[168] = "ZONE_CHANGED_NEW_AREA";
    g_scriptEvents[169] = "MINIMAP_UPDATE_ZOOM";
    g_scriptEvents[170] = "MINIMAP_UPDATE_TRACKING";
    g_scriptEvents[171] = "SCREENSHOT_SUCCEEDED";
    g_scriptEvents[172] = "SCREENSHOT_FAILED";
    g_scriptEvents[173] = "ACTIONBAR_SHOWGRID";
    g_scriptEvents[174] = "ACTIONBAR_HIDEGRID";
    g_scriptEvents[175] = "ACTIONBAR_PAGE_CHANGED";
    g_scriptEvents[SCRIPT_ACTIONBAR_SLOT_CHANGED] = "ACTIONBAR_SLOT_CHANGED";
    g_scriptEvents[177] = "ACTIONBAR_UPDATE_STATE";
    g_scriptEvents[178] = "ACTIONBAR_UPDATE_USABLE";
    g_scriptEvents[179] = "ACTIONBAR_UPDATE_COOLDOWN";
    g_scriptEvents[180] = "UPDATE_BONUS_ACTIONBAR";
    g_scriptEvents[181] = "PARTY_MEMBERS_CHANGED";
    g_scriptEvents[182] = "PARTY_LEADER_CHANGED";
    g_scriptEvents[183] = "PARTY_MEMBER_ENABLE";
    g_scriptEvents[184] = "PARTY_MEMBER_DISABLE";
    g_scriptEvents[185] = "PARTY_LOOT_METHOD_CHANGED";
    g_scriptEvents[186] = "SYSMSG";
    g_scriptEvents[187] = "UI_ERROR_MESSAGE";
    g_scriptEvents[188] = "UI_INFO_MESSAGE";
    g_scriptEvents[189] = "UPDATE_CHAT_COLOR";
    g_scriptEvents[190] = "CHAT_MSG_ADDON";
    g_scriptEvents[191] = "CHAT_MSG_SYSTEM";
    g_scriptEvents[192] = "CHAT_MSG_SAY";
    g_scriptEvents[193] = "CHAT_MSG_PARTY";
    g_scriptEvents[194] = "CHAT_MSG_RAID";
    g_scriptEvents[195] = "CHAT_MSG_GUILD";
    g_scriptEvents[196] = "CHAT_MSG_OFFICER";
    g_scriptEvents[197] = "CHAT_MSG_YELL";
    g_scriptEvents[198] = "CHAT_MSG_WHISPER";
    g_scriptEvents[199] = "CHAT_MSG_WHISPER_INFORM";
    g_scriptEvents[200] = "CHAT_MSG_EMOTE";
    g_scriptEvents[201] = "CHAT_MSG_TEXT_EMOTE";
    g_scriptEvents[202] = "CHAT_MSG_MONSTER_SAY";
    g_scriptEvents[203] = "CHAT_MSG_MONSTER_PARTY";
    g_scriptEvents[204] = "CHAT_MSG_MONSTER_YELL";
    g_scriptEvents[205] = "CHAT_MSG_MONSTER_WHISPER";
    g_scriptEvents[206] = "CHAT_MSG_MONSTER_EMOTE";
    g_scriptEvents[207] = "CHAT_MSG_CHANNEL";
    g_scriptEvents[208] = "CHAT_MSG_CHANNEL_JOIN";
    g_scriptEvents[209] = "CHAT_MSG_CHANNEL_LEAVE";
    g_scriptEvents[210] = "CHAT_MSG_CHANNEL_LIST";
    g_scriptEvents[211] = "CHAT_MSG_CHANNEL_NOTICE";
    g_scriptEvents[212] = "CHAT_MSG_CHANNEL_NOTICE_USER";
    g_scriptEvents[213] = "CHAT_MSG_AFK";
    g_scriptEvents[214] = "CHAT_MSG_DND";
    g_scriptEvents[215] = "CHAT_MSG_IGNORED";
    g_scriptEvents[216] = "CHAT_MSG_SKILL";
    g_scriptEvents[217] = "CHAT_MSG_LOOT";
    g_scriptEvents[218] = "CHAT_MSG_MONEY";
    g_scriptEvents[219] = "CHAT_MSG_OPENING";
    g_scriptEvents[220] = "CHAT_MSG_TRADESKILLS";
    g_scriptEvents[221] = "CHAT_MSG_PET_INFO";
    g_scriptEvents[222] = "CHAT_MSG_COMBAT_MISC_INFO";
    g_scriptEvents[223] = "CHAT_MSG_COMBAT_XP_GAIN";
    g_scriptEvents[224] = "CHAT_MSG_COMBAT_HONOR_GAIN";
    g_scriptEvents[225] = "CHAT_MSG_COMBAT_FACTION_CHANGE";
    g_scriptEvents[226] = "CHAT_MSG_BG_SYSTEM_NEUTRAL";
    g_scriptEvents[227] = "CHAT_MSG_BG_SYSTEM_ALLIANCE";
    g_scriptEvents[228] = "CHAT_MSG_BG_SYSTEM_HORDE";
    g_scriptEvents[229] = "CHAT_MSG_RAID_LEADER";
    g_scriptEvents[230] = "CHAT_MSG_RAID_WARNING";
    g_scriptEvents[231] = "CHAT_MSG_RAID_BOSS_WHISPER";
    g_scriptEvents[232] = "CHAT_MSG_RAID_BOSS_EMOTE";
    g_scriptEvents[233] = "CHAT_MSG_FILTERED";
    g_scriptEvents[234] = "CHAT_MSG_BATTLEGROUND";
    g_scriptEvents[235] = "CHAT_MSG_BATTLEGROUND_LEADER";
    g_scriptEvents[236] = "CHAT_MSG_RESTRICTED";
    g_scriptEvents[237] = "";
    g_scriptEvents[238] = "CHAT_MSG_ACHIEVEMENT";
    g_scriptEvents[239] = "CHAT_MSG_GUILD_ACHIEVEMENT";
    g_scriptEvents[240] = "LANGUAGE_LIST_CHANGED";
    g_scriptEvents[241] = "TIME_PLAYED_MSG";
    g_scriptEvents[242] = "SPELLS_CHANGED";
    g_scriptEvents[243] = "CURRENT_SPELL_CAST_CHANGED";
    g_scriptEvents[244] = "SPELL_UPDATE_COOLDOWN";
    g_scriptEvents[245] = "SPELL_UPDATE_USABLE";
    g_scriptEvents[246] = "CHARACTER_POINTS_CHANGED";
    g_scriptEvents[247] = "SKILL_LINES_CHANGED";
    g_scriptEvents[248] = "ITEM_PUSH";
    g_scriptEvents[249] = "LOOT_OPENED";
    g_scriptEvents[250] = "LOOT_SLOT_CLEARED";
    g_scriptEvents[251] = "LOOT_SLOT_CHANGED";
    g_scriptEvents[252] = "LOOT_CLOSED";
    g_scriptEvents[SCRIPT_PLAYER_LOGIN] = "PLAYER_LOGIN";
    g_scriptEvents[SCRIPT_PLAYER_LOGOUT] = "PLAYER_LOGOUT";
    g_scriptEvents[SCRIPT_PLAYER_ENTERING_WORLD] = "PLAYER_ENTERING_WORLD";
    g_scriptEvents[256] = "PLAYER_LEAVING_WORLD";
    g_scriptEvents[257] = "PLAYER_ALIVE";
    g_scriptEvents[258] = "PLAYER_DEAD";
    g_scriptEvents[259] = "PLAYER_CAMPING";
    g_scriptEvents[260] = "PLAYER_QUITING";
    g_scriptEvents[261] = "LOGOUT_CANCEL";
    g_scriptEvents[262] = "RESURRECT_REQUEST";
    g_scriptEvents[263] = "PARTY_INVITE_REQUEST";
    g_scriptEvents[264] = "PARTY_INVITE_CANCEL";
    g_scriptEvents[265] = "GUILD_INVITE_REQUEST";
    g_scriptEvents[266] = "GUILD_INVITE_CANCEL";
    g_scriptEvents[267] = "GUILD_MOTD";
    g_scriptEvents[268] = "TRADE_REQUEST";
    g_scriptEvents[269] = "TRADE_REQUEST_CANCEL";
    g_scriptEvents[270] = "LOOT_BIND_CONFIRM";
    g_scriptEvents[271] = "EQUIP_BIND_CONFIRM";
    g_scriptEvents[272] = "AUTOEQUIP_BIND_CONFIRM";
    g_scriptEvents[273] = "USE_BIND_CONFIRM";
    g_scriptEvents[274] = "DELETE_ITEM_CONFIRM";
    g_scriptEvents[275] = "CURSOR_UPDATE";
    g_scriptEvents[276] = "ITEM_TEXT_BEGIN";
    g_scriptEvents[277] = "ITEM_TEXT_TRANSLATION";
    g_scriptEvents[278] = "ITEM_TEXT_READY";
    g_scriptEvents[279] = "ITEM_TEXT_CLOSED";
    g_scriptEvents[280] = "GOSSIP_SHOW";
    g_scriptEvents[281] = "GOSSIP_CONFIRM";
    g_scriptEvents[282] = "GOSSIP_CONFIRM_CANCEL";
    g_scriptEvents[283] = "GOSSIP_ENTER_CODE";
    g_scriptEvents[284] = "GOSSIP_CLOSED";
    g_scriptEvents[285] = "QUEST_GREETING";
    g_scriptEvents[286] = "QUEST_DETAIL";
    g_scriptEvents[287] = "QUEST_PROGRESS";
    g_scriptEvents[288] = "QUEST_COMPLETE";
    g_scriptEvents[289] = "QUEST_FINISHED";
    g_scriptEvents[290] = "QUEST_ITEM_UPDATE";
    g_scriptEvents[291] = "TAXIMAP_OPENED";
    g_scriptEvents[292] = "TAXIMAP_CLOSED";
    g_scriptEvents[293] = "QUEST_LOG_UPDATE";
    g_scriptEvents[294] = "TRAINER_SHOW";
    g_scriptEvents[295] = "TRAINER_UPDATE";
    g_scriptEvents[296] = "TRAINER_DESCRIPTION_UPDATE";
    g_scriptEvents[297] = "TRAINER_CLOSED";
    g_scriptEvents[298] = "CVAR_UPDATE";
    g_scriptEvents[299] = "TRADE_SKILL_SHOW";
    g_scriptEvents[300] = "TRADE_SKILL_UPDATE";
    g_scriptEvents[301] = "TRADE_SKILL_CLOSE";
    g_scriptEvents[302] = "MERCHANT_SHOW";
    g_scriptEvents[303] = "MERCHANT_UPDATE";
    g_scriptEvents[304] = "MERCHANT_CLOSED";
    g_scriptEvents[305] = "TRADE_SHOW";
    g_scriptEvents[306] = "TRADE_CLOSED";
    g_scriptEvents[307] = "TRADE_UPDATE";
    g_scriptEvents[308] = "TRADE_ACCEPT_UPDATE";
    g_scriptEvents[309] = "TRADE_TARGET_ITEM_CHANGED";
    g_scriptEvents[310] = "TRADE_PLAYER_ITEM_CHANGED";
    g_scriptEvents[311] = "TRADE_MONEY_CHANGED";
    g_scriptEvents[312] = "PLAYER_TRADE_MONEY";
    g_scriptEvents[313] = "BAG_OPEN";
    g_scriptEvents[314] = "BAG_UPDATE";
    g_scriptEvents[315] = "BAG_CLOSED";
    g_scriptEvents[316] = "BAG_UPDATE_COOLDOWN";
    g_scriptEvents[317] = "LOCALPLAYER_PET_RENAMED";
    g_scriptEvents[318] = "UNIT_ATTACK";
    g_scriptEvents[319] = "UNIT_DEFENSE";
    g_scriptEvents[320] = "PET_ATTACK_START";
    g_scriptEvents[321] = "PET_ATTACK_STOP";
    g_scriptEvents[322] = "UPDATE_MOUSEOVER_UNIT";
    g_scriptEvents[323] = "UNIT_SPELLCAST_SENT";
    g_scriptEvents[324] = "UNIT_SPELLCAST_START";
    g_scriptEvents[325] = "UNIT_SPELLCAST_STOP";
    g_scriptEvents[326] = "UNIT_SPELLCAST_FAILED";
    g_scriptEvents[327] = "UNIT_SPELLCAST_FAILED_QUIET";
    g_scriptEvents[328] = "UNIT_SPELLCAST_INTERRUPTED";
    g_scriptEvents[329] = "UNIT_SPELLCAST_DELAYED";
    g_scriptEvents[330] = "UNIT_SPELLCAST_SUCCEEDED";
    g_scriptEvents[331] = "UNIT_SPELLCAST_CHANNEL_START";
    g_scriptEvents[332] = "UNIT_SPELLCAST_CHANNEL_UPDATE";
    g_scriptEvents[333] = "UNIT_SPELLCAST_CHANNEL_STOP";
    g_scriptEvents[334] = "UNIT_SPELLCAST_INTERRUPTIBLE";
    g_scriptEvents[335] = "UNIT_SPELLCAST_NOT_INTERRUPTIBLE";
    g_scriptEvents[336] = "PLAYER_GUILD_UPDATE";
    g_scriptEvents[337] = "QUEST_ACCEPT_CONFIRM";
    g_scriptEvents[338] = "PLAYERBANKSLOTS_CHANGED";
    g_scriptEvents[339] = "BANKFRAME_OPENED";
    g_scriptEvents[340] = "BANKFRAME_CLOSED";
    g_scriptEvents[341] = "PLAYERBANKBAGSLOTS_CHANGED";
    g_scriptEvents[342] = "FRIENDLIST_UPDATE";
    g_scriptEvents[343] = "IGNORELIST_UPDATE";
    g_scriptEvents[344] = "MUTELIST_UPDATE";
    g_scriptEvents[345] = "PET_BAR_UPDATE";
    g_scriptEvents[346] = "PET_BAR_UPDATE_COOLDOWN";
    g_scriptEvents[347] = "PET_BAR_SHOWGRID";
    g_scriptEvents[348] = "PET_BAR_HIDEGRID";
    g_scriptEvents[349] = "PET_BAR_HIDE";
    g_scriptEvents[350] = "PET_BAR_UPDATE_USABLE";
    g_scriptEvents[351] = "MINIMAP_PING";
    g_scriptEvents[352] = "MIRROR_TIMER_START";
    g_scriptEvents[353] = "MIRROR_TIMER_PAUSE";
    g_scriptEvents[354] = "MIRROR_TIMER_STOP";
    g_scriptEvents[355] = "WORLD_MAP_UPDATE";
    g_scriptEvents[356] = "WORLD_MAP_NAME_UPDATE";
    g_scriptEvents[357] = "AUTOFOLLOW_BEGIN";
    g_scriptEvents[358] = "AUTOFOLLOW_END";
    g_scriptEvents[360] = "CINEMATIC_START";
    g_scriptEvents[361] = "CINEMATIC_STOP";
    g_scriptEvents[362] = "UPDATE_FACTION";
    g_scriptEvents[363] = "CLOSE_WORLD_MAP";
    g_scriptEvents[364] = "OPEN_TABARD_FRAME";
    g_scriptEvents[365] = "CLOSE_TABARD_FRAME";
    g_scriptEvents[366] = "TABARD_CANSAVE_CHANGED";
    g_scriptEvents[367] = "GUILD_REGISTRAR_SHOW";
    g_scriptEvents[368] = "GUILD_REGISTRAR_CLOSED";
    g_scriptEvents[369] = "DUEL_REQUESTED";
    g_scriptEvents[370] = "DUEL_OUTOFBOUNDS";
    g_scriptEvents[371] = "DUEL_INBOUNDS";
    g_scriptEvents[372] = "DUEL_FINISHED";
    g_scriptEvents[373] = "TUTORIAL_TRIGGER";
    g_scriptEvents[374] = "PET_DISMISS_START";
    g_scriptEvents[375] = "UPDATE_BINDINGS";
    g_scriptEvents[376] = "UPDATE_SHAPESHIFT_FORMS";
    g_scriptEvents[377] = "UPDATE_SHAPESHIFT_FORM";
    g_scriptEvents[378] = "UPDATE_SHAPESHIFT_USABLE";
    g_scriptEvents[379] = "UPDATE_SHAPESHIFT_COOLDOWN";
    g_scriptEvents[380] = "WHO_LIST_UPDATE";
    g_scriptEvents[381] = "PETITION_SHOW";
    g_scriptEvents[382] = "PETITION_CLOSED";
    g_scriptEvents[383] = "EXECUTE_CHAT_LINE";
    g_scriptEvents[384] = "UPDATE_MACROS";
    g_scriptEvents[385] = "UPDATE_TICKET";
    g_scriptEvents[386] = "UPDATE_CHAT_WINDOWS";
    g_scriptEvents[387] = "CONFIRM_XP_LOSS";
    g_scriptEvents[388] = "CORPSE_IN_RANGE";
    g_scriptEvents[389] = "CORPSE_IN_INSTANCE";
    g_scriptEvents[390] = "CORPSE_OUT_OF_RANGE";
    g_scriptEvents[391] = "UPDATE_GM_STATUS";
    g_scriptEvents[392] = "PLAYER_UNGHOST";
    g_scriptEvents[393] = "BIND_ENCHANT";
    g_scriptEvents[394] = "REPLACE_ENCHANT";
    g_scriptEvents[395] = "TRADE_REPLACE_ENCHANT";
    g_scriptEvents[396] = "TRADE_POTENTIAL_BIND_ENCHANT";
    g_scriptEvents[397] = "PLAYER_UPDATE_RESTING";
    g_scriptEvents[398] = "UPDATE_EXHAUSTION";
    g_scriptEvents[399] = "PLAYER_FLAGS_CHANGED";
    g_scriptEvents[400] = "GUILD_ROSTER_UPDATE";
    g_scriptEvents[401] = "GM_PLAYER_INFO";
    g_scriptEvents[402] = "MAIL_SHOW";
    g_scriptEvents[403] = "MAIL_CLOSED";
    g_scriptEvents[404] = "SEND_MAIL_MONEY_CHANGED";
    g_scriptEvents[405] = "SEND_MAIL_COD_CHANGED";
    g_scriptEvents[406] = "MAIL_SEND_INFO_UPDATE";
    g_scriptEvents[407] = "MAIL_SEND_SUCCESS";
    g_scriptEvents[408] = "MAIL_INBOX_UPDATE";
    g_scriptEvents[409] = "MAIL_LOCK_SEND_ITEMS";
    g_scriptEvents[410] = "MAIL_UNLOCK_SEND_ITEMS";
    g_scriptEvents[411] = "BATTLEFIELDS_SHOW";
    g_scriptEvents[412] = "BATTLEFIELDS_CLOSED";
    g_scriptEvents[413] = "UPDATE_BATTLEFIELD_STATUS";
    g_scriptEvents[414] = "UPDATE_BATTLEFIELD_SCORE";
    g_scriptEvents[415] = "AUCTION_HOUSE_SHOW";
    g_scriptEvents[416] = "AUCTION_HOUSE_CLOSED";
    g_scriptEvents[417] = "NEW_AUCTION_UPDATE";
    g_scriptEvents[418] = "AUCTION_ITEM_LIST_UPDATE";
    g_scriptEvents[419] = "AUCTION_OWNED_LIST_UPDATE";
    g_scriptEvents[420] = "AUCTION_BIDDER_LIST_UPDATE";
    g_scriptEvents[421] = "PET_UI_UPDATE";
    g_scriptEvents[422] = "PET_UI_CLOSE";
    g_scriptEvents[423] = "ADDON_LOADED";
    g_scriptEvents[424] = "VARIABLES_LOADED";
    g_scriptEvents[425] = "MACRO_ACTION_FORBIDDEN";
    g_scriptEvents[426] = "ADDON_ACTION_FORBIDDEN";
    g_scriptEvents[427] = "MACRO_ACTION_BLOCKED";
    g_scriptEvents[428] = "ADDON_ACTION_BLOCKED";
    g_scriptEvents[429] = "START_AUTOREPEAT_SPELL";
    g_scriptEvents[430] = "STOP_AUTOREPEAT_SPELL";
    g_scriptEvents[431] = "PET_STABLE_SHOW";
    g_scriptEvents[432] = "PET_STABLE_UPDATE";
    g_scriptEvents[433] = "PET_STABLE_UPDATE_PAPERDOLL";
    g_scriptEvents[434] = "PET_STABLE_CLOSED";
    g_scriptEvents[435] = "RAID_ROSTER_UPDATE";
    g_scriptEvents[436] = "UPDATE_PENDING_MAIL";
    g_scriptEvents[437] = "UPDATE_INVENTORY_ALERTS";
    g_scriptEvents[438] = "UPDATE_INVENTORY_DURABILITY";
    g_scriptEvents[439] = "UPDATE_TRADESKILL_RECAST";
    g_scriptEvents[440] = "OPEN_MASTER_LOOT_LIST";
    g_scriptEvents[441] = "UPDATE_MASTER_LOOT_LIST";
    g_scriptEvents[442] = "START_LOOT_ROLL";
    g_scriptEvents[443] = "CANCEL_LOOT_ROLL";
    g_scriptEvents[444] = "CONFIRM_LOOT_ROLL";
    g_scriptEvents[445] = "CONFIRM_DISENCHANT_ROLL";
    g_scriptEvents[446] = "INSTANCE_BOOT_START";
    g_scriptEvents[447] = "INSTANCE_BOOT_STOP";
    g_scriptEvents[448] = "LEARNED_SPELL_IN_TAB";
    g_scriptEvents[449] = "DISPLAY_SIZE_CHANGED";
    g_scriptEvents[450] = "CONFIRM_TALENT_WIPE";
    g_scriptEvents[451] = "CONFIRM_BINDER";
    g_scriptEvents[452] = "MAIL_FAILED";
    g_scriptEvents[453] = "CLOSE_INBOX_ITEM";
    g_scriptEvents[454] = "CONFIRM_SUMMON";
    g_scriptEvents[455] = "CANCEL_SUMMON";
    g_scriptEvents[456] = "BILLING_NAG_DIALOG";
    g_scriptEvents[457] = "IGR_BILLING_NAG_DIALOG";
    g_scriptEvents[458] = "PLAYER_SKINNED";
    g_scriptEvents[459] = "TABARD_SAVE_PENDING";
    g_scriptEvents[460] = "UNIT_QUEST_LOG_CHANGED";
    g_scriptEvents[461] = "PLAYER_PVP_KILLS_CHANGED";
    g_scriptEvents[462] = "PLAYER_PVP_RANK_CHANGED";
    g_scriptEvents[463] = "INSPECT_HONOR_UPDATE";
    g_scriptEvents[464] = "UPDATE_WORLD_STATES";
    g_scriptEvents[465] = "AREA_SPIRIT_HEALER_IN_RANGE";
    g_scriptEvents[466] = "AREA_SPIRIT_HEALER_OUT_OF_RANGE";
    g_scriptEvents[467] = "PLAYTIME_CHANGED";
    g_scriptEvents[468] = "UPDATE_LFG_TYPES";
    g_scriptEvents[469] = "UPDATE_LFG_LIST";
    g_scriptEvents[470] = "UPDATE_LFG_LIST_INCREMENTAL";
    g_scriptEvents[471] = "START_MINIGAME";
    g_scriptEvents[472] = "MINIGAME_UPDATE";
    g_scriptEvents[473] = "READY_CHECK";
    g_scriptEvents[474] = "READY_CHECK_CONFIRM";
    g_scriptEvents[475] = "READY_CHECK_FINISHED";
    g_scriptEvents[476] = "RAID_TARGET_UPDATE";
    g_scriptEvents[477] = "GMSURVEY_DISPLAY";
    g_scriptEvents[478] = "UPDATE_INSTANCE_INFO";
    g_scriptEvents[479] = "SOCKET_INFO_UPDATE";
    g_scriptEvents[480] = "SOCKET_INFO_CLOSE";
    g_scriptEvents[481] = "PETITION_VENDOR_SHOW";
    g_scriptEvents[482] = "PETITION_VENDOR_CLOSED";
    g_scriptEvents[483] = "PETITION_VENDOR_UPDATE";
    g_scriptEvents[484] = "COMBAT_TEXT_UPDATE";
    g_scriptEvents[485] = "QUEST_WATCH_UPDATE";
    g_scriptEvents[486] = "KNOWLEDGE_BASE_SETUP_LOAD_SUCCESS";
    g_scriptEvents[487] = "KNOWLEDGE_BASE_SETUP_LOAD_FAILURE";
    g_scriptEvents[488] = "KNOWLEDGE_BASE_QUERY_LOAD_SUCCESS";
    g_scriptEvents[489] = "KNOWLEDGE_BASE_QUERY_LOAD_FAILURE";
    g_scriptEvents[490] = "KNOWLEDGE_BASE_ARTICLE_LOAD_SUCCESS";
    g_scriptEvents[491] = "KNOWLEDGE_BASE_ARTICLE_LOAD_FAILURE";
    g_scriptEvents[492] = "KNOWLEDGE_BASE_SYSTEM_MOTD_UPDATED";
    g_scriptEvents[493] = "KNOWLEDGE_BASE_SERVER_MESSAGE";
    g_scriptEvents[494] = "ARENA_TEAM_UPDATE";
    g_scriptEvents[495] = "ARENA_TEAM_ROSTER_UPDATE";
    g_scriptEvents[496] = "ARENA_TEAM_INVITE_REQUEST";
    g_scriptEvents[497] = "HONOR_CURRENCY_UPDATE";
    g_scriptEvents[498] = "KNOWN_TITLES_UPDATE";
    g_scriptEvents[499] = "NEW_TITLE_EARNED";
    g_scriptEvents[500] = "OLD_TITLE_LOST";
    g_scriptEvents[501] = "LFG_UPDATE";
    g_scriptEvents[502] = "LFG_PROPOSAL_UPDATE";
    g_scriptEvents[503] = "LFG_PROPOSAL_SHOW";
    g_scriptEvents[504] = "LFG_PROPOSAL_FAILED";
    g_scriptEvents[505] = "LFG_PROPOSAL_SUCCEEDED";
    g_scriptEvents[506] = "LFG_ROLE_UPDATE";
    g_scriptEvents[507] = "LFG_ROLE_CHECK_UPDATE";
    g_scriptEvents[508] = "LFG_ROLE_CHECK_SHOW";
    g_scriptEvents[509] = "LFG_ROLE_CHECK_HIDE";
    g_scriptEvents[510] = "LFG_ROLE_CHECK_ROLE_CHOSEN";
    g_scriptEvents[511] = "LFG_QUEUE_STATUS_UPDATE";
    g_scriptEvents[512] = "LFG_BOOT_PROPOSAL_UPDATE";
    g_scriptEvents[513] = "LFG_LOCK_INFO_RECEIVED";
    g_scriptEvents[514] = "LFG_UPDATE_RANDOM_INFO";
    g_scriptEvents[515] = "LFG_OFFER_CONTINUE";
    g_scriptEvents[516] = "LFG_OPEN_FROM_GOSSIP";
    g_scriptEvents[517] = "LFG_COMPLETION_REWARD";
    g_scriptEvents[518] = "PARTY_LFG_RESTRICTED";
    g_scriptEvents[519] = "PLAYER_ROLES_ASSIGNED";
    g_scriptEvents[520] = "COMBAT_RATING_UPDATE";
    g_scriptEvents[521] = "MODIFIER_STATE_CHANGED";
    g_scriptEvents[522] = "UPDATE_STEALTH";
    g_scriptEvents[523] = "ENABLE_TAXI_BENCHMARK";
    g_scriptEvents[524] = "DISABLE_TAXI_BENCHMARK";
    g_scriptEvents[525] = "VOICE_START";
    g_scriptEvents[526] = "VOICE_STOP";
    g_scriptEvents[527] = "VOICE_STATUS_UPDATE";
    g_scriptEvents[528] = "VOICE_CHANNEL_STATUS_UPDATE";
    g_scriptEvents[529] = "UPDATE_FLOATING_CHAT_WINDOWS";
    g_scriptEvents[530] = "RAID_INSTANCE_WELCOME";
    g_scriptEvents[531] = "MOVIE_RECORDING_PROGRESS";
    g_scriptEvents[532] = "MOVIE_COMPRESSING_PROGRESS";
    g_scriptEvents[533] = "MOVIE_UNCOMPRESSED_MOVIE";
    g_scriptEvents[534] = "VOICE_PUSH_TO_TALK_START";
    g_scriptEvents[535] = "VOICE_PUSH_TO_TALK_STOP";
    g_scriptEvents[536] = "GUILDBANKFRAME_OPENED";
    g_scriptEvents[537] = "GUILDBANKFRAME_CLOSED";
    g_scriptEvents[538] = "GUILDBANKBAGSLOTS_CHANGED";
    g_scriptEvents[539] = "GUILDBANK_ITEM_LOCK_CHANGED";
    g_scriptEvents[540] = "GUILDBANK_UPDATE_TABS";
    g_scriptEvents[541] = "GUILDBANK_UPDATE_MONEY";
    g_scriptEvents[542] = "GUILDBANKLOG_UPDATE";
    g_scriptEvents[543] = "GUILDBANK_UPDATE_WITHDRAWMONEY";
    g_scriptEvents[544] = "GUILDBANK_UPDATE_TEXT";
    g_scriptEvents[545] = "GUILDBANK_TEXT_CHANGED";
    g_scriptEvents[546] = "CHANNEL_UI_UPDATE";
    g_scriptEvents[547] = "CHANNEL_COUNT_UPDATE";
    g_scriptEvents[548] = "CHANNEL_ROSTER_UPDATE";
    g_scriptEvents[549] = "CHANNEL_VOICE_UPDATE";
    g_scriptEvents[550] = "CHANNEL_INVITE_REQUEST";
    g_scriptEvents[551] = "CHANNEL_PASSWORD_REQUEST";
    g_scriptEvents[552] = "CHANNEL_FLAGS_UPDATED";
    g_scriptEvents[553] = "VOICE_SESSIONS_UPDATE";
    g_scriptEvents[554] = "VOICE_CHAT_ENABLED_UPDATE";
    g_scriptEvents[555] = "VOICE_LEFT_SESSION";
    g_scriptEvents[556] = "INSPECT_TALENT_READY";
    g_scriptEvents[557] = "VOICE_SELF_MUTE";
    g_scriptEvents[558] = "VOICE_PLATE_START";
    g_scriptEvents[559] = "VOICE_PLATE_STOP";
    g_scriptEvents[560] = "ARENA_SEASON_WORLD_STATE";
    g_scriptEvents[561] = "GUILD_EVENT_LOG_UPDATE";
    g_scriptEvents[562] = "GUILDTABARD_UPDATE";
    g_scriptEvents[563] = "SOUND_DEVICE_UPDATE";
    g_scriptEvents[564] = "COMMENTATOR_MAP_UPDATE";
    g_scriptEvents[565] = "COMMENTATOR_ENTER_WORLD";
    g_scriptEvents[566] = "COMBAT_LOG_EVENT";
    g_scriptEvents[567] = "COMBAT_LOG_EVENT_UNFILTERED";
    g_scriptEvents[568] = "COMMENTATOR_PLAYER_UPDATE";
    g_scriptEvents[569] = "PLAYER_ENTERING_BATTLEGROUND";
    g_scriptEvents[570] = "BARBER_SHOP_OPEN";
    g_scriptEvents[571] = "BARBER_SHOP_CLOSE";
    g_scriptEvents[572] = "BARBER_SHOP_SUCCESS";
    g_scriptEvents[573] = "BARBER_SHOP_APPEARANCE_APPLIED";
    g_scriptEvents[574] = "CALENDAR_UPDATE_INVITE_LIST";
    g_scriptEvents[575] = "CALENDAR_UPDATE_EVENT_LIST";
    g_scriptEvents[576] = "CALENDAR_NEW_EVENT";
    g_scriptEvents[577] = "CALENDAR_OPEN_EVENT";
    g_scriptEvents[578] = "CALENDAR_CLOSE_EVENT";
    g_scriptEvents[579] = "CALENDAR_UPDATE_EVENT";
    g_scriptEvents[580] = "CALENDAR_UPDATE_PENDING_INVITES";
    g_scriptEvents[581] = "CALENDAR_EVENT_ALARM";
    g_scriptEvents[582] = "CALENDAR_UPDATE_ERROR";
    g_scriptEvents[583] = "CALENDAR_ACTION_PENDING";
    g_scriptEvents[584] = "VEHICLE_ANGLE_SHOW";
    g_scriptEvents[585] = "VEHICLE_ANGLE_UPDATE";
    g_scriptEvents[586] = "VEHICLE_POWER_SHOW";
    g_scriptEvents[587] = "UNIT_ENTERING_VEHICLE";
    g_scriptEvents[588] = "UNIT_ENTERED_VEHICLE";
    g_scriptEvents[589] = "UNIT_EXITING_VEHICLE";
    g_scriptEvents[590] = "UNIT_EXITED_VEHICLE";
    g_scriptEvents[591] = "VEHICLE_PASSENGERS_CHANGED";
    g_scriptEvents[592] = "PLAYER_GAINS_VEHICLE_DATA";
    g_scriptEvents[593] = "PLAYER_LOSES_VEHICLE_DATA";
    g_scriptEvents[594] = "PET_FORCE_NAME_DECLENSION";
    g_scriptEvents[595] = "LEVEL_GRANT_PROPOSED";
    g_scriptEvents[596] = "SYNCHRONIZE_SETTINGS";
    g_scriptEvents[597] = "PLAY_MOVIE";
    g_scriptEvents[598] = "RUNE_POWER_UPDATE";
    g_scriptEvents[599] = "RUNE_TYPE_UPDATE";
    g_scriptEvents[600] = "ACHIEVEMENT_EARNED";
    g_scriptEvents[601] = "CRITERIA_UPDATE";
    g_scriptEvents[602] = "RECEIVED_ACHIEVEMENT_LIST";
    g_scriptEvents[603] = "PET_RENAMEABLE";
    g_scriptEvents[604] = "KNOWN_CURRENCY_TYPES_UPDATE";
    g_scriptEvents[605] = "CURRENCY_DISPLAY_UPDATE";
    g_scriptEvents[606] = "COMPANION_LEARNED";
    g_scriptEvents[607] = "COMPANION_UNLEARNED";
    g_scriptEvents[608] = "COMPANION_UPDATE";
    g_scriptEvents[609] = "UNIT_THREAT_LIST_UPDATE";
    g_scriptEvents[610] = "UNIT_THREAT_SITUATION_UPDATE";
    g_scriptEvents[611] = "GLYPH_ADDED";
    g_scriptEvents[612] = "GLYPH_REMOVED";
    g_scriptEvents[613] = "GLYPH_UPDATED";
    g_scriptEvents[614] = "GLYPH_ENABLED";
    g_scriptEvents[615] = "GLYPH_DISABLED";
    g_scriptEvents[616] = "USE_GLYPH";
    g_scriptEvents[617] = "TRACKED_ACHIEVEMENT_UPDATE";
    g_scriptEvents[618] = "ARENA_OPPONENT_UPDATE";
    g_scriptEvents[619] = "INSPECT_ACHIEVEMENT_READY";
    g_scriptEvents[620] = "RAISED_AS_GHOUL";
    g_scriptEvents[621] = "PARTY_CONVERTED_TO_RAID";
    g_scriptEvents[622] = "PVPQUEUE_ANYWHERE_SHOW";
    g_scriptEvents[623] = "PVPQUEUE_ANYWHERE_UPDATE_AVAILABLE";
    g_scriptEvents[624] = "QUEST_ACCEPTED";
    g_scriptEvents[625] = "PLAYER_TALENT_UPDATE";
    g_scriptEvents[626] = "ACTIVE_TALENT_GROUP_CHANGED";
    g_scriptEvents[627] = "PET_TALENT_UPDATE";
    g_scriptEvents[628] = "PREVIEW_TALENT_POINTS_CHANGED";
    g_scriptEvents[629] = "PREVIEW_PET_TALENT_POINTS_CHANGED";
    g_scriptEvents[630] = "WEAR_EQUIPMENT_SET";
    g_scriptEvents[631] = "EQUIPMENT_SETS_CHANGED";
    g_scriptEvents[632] = "INSTANCE_LOCK_START";
    g_scriptEvents[633] = "INSTANCE_LOCK_STOP";
    g_scriptEvents[634] = "PLAYER_EQUIPMENT_CHANGED";
    g_scriptEvents[635] = "ITEM_LOCKED";
    g_scriptEvents[636] = "ITEM_UNLOCKED";
    g_scriptEvents[637] = "TRADE_SKILL_FILTER_UPDATE";
    g_scriptEvents[638] = "EQUIPMENT_SWAP_PENDING";
    g_scriptEvents[639] = "EQUIPMENT_SWAP_FINISHED";
    g_scriptEvents[640] = "NPC_PVPQUEUE_ANYWHERE";
    g_scriptEvents[641] = "UPDATE_MULTI_CAST_ACTIONBAR";
    g_scriptEvents[642] = "ENABLE_XP_GAIN";
    g_scriptEvents[643] = "DISABLE_XP_GAIN";
    g_scriptEvents[644] = "BATTLEFIELD_MGR_ENTRY_INVITE";
    g_scriptEvents[645] = "BATTLEFIELD_MGR_ENTERED";
    g_scriptEvents[646] = "BATTLEFIELD_MGR_QUEUE_REQUEST_RESPONSE";
    g_scriptEvents[647] = "BATTLEFIELD_MGR_EJECT_PENDING";
    g_scriptEvents[648] = "BATTLEFIELD_MGR_EJECTED";
    g_scriptEvents[649] = "BATTLEFIELD_MGR_QUEUE_INVITE";
    g_scriptEvents[650] = "BATTLEFIELD_MGR_STATE_CHANGE";
    g_scriptEvents[651] = "WORLD_STATE_UI_TIMER_UPDATE";
    g_scriptEvents[652] = "END_REFUND";
    g_scriptEvents[653] = "END_BOUND_TRADEABLE";
    g_scriptEvents[654] = "UPDATE_CHAT_COLOR_NAME_BY_CLASS";
    g_scriptEvents[655] = "GMRESPONSE_RECEIVED";
    g_scriptEvents[656] = "VEHICLE_UPDATE";
    g_scriptEvents[657] = "WOW_MOUSE_NOT_FOUND";
    g_scriptEvents[659] = "MAIL_SUCCESS";
    g_scriptEvents[660] = "TALENTS_INVOLUNTARILY_RESET";
    g_scriptEvents[661] = "INSTANCE_ENCOUNTER_ENGAGE_UNIT";
    g_scriptEvents[662] = "QUEST_QUERY_COMPLETE";
    g_scriptEvents[663] = "QUEST_POI_UPDATE";
    g_scriptEvents[664] = "PLAYER_DIFFICULTY_CHANGED";
    g_scriptEvents[665] = "CHAT_MSG_PARTY_LEADER";
    g_scriptEvents[666] = "VOTE_KICK_REASON_NEEDED";
    g_scriptEvents[667] = "ENABLE_LOW_LEVEL_RAID";
    g_scriptEvents[668] = "DISABLE_LOW_LEVEL_RAID";
    g_scriptEvents[669] = "CHAT_MSG_TARGETICONS";
    g_scriptEvents[670] = "AUCTION_HOUSE_DISABLED";
    g_scriptEvents[671] = "AUCTION_MULTISELL_START";
    g_scriptEvents[672] = "AUCTION_MULTISELL_UPDATE";
    g_scriptEvents[673] = "AUCTION_MULTISELL_FAILURE";
    g_scriptEvents[674] = "PET_SPELL_POWER_UPDATE";
    g_scriptEvents[675] = "BN_CONNECTED";
    g_scriptEvents[676] = "BN_DISCONNECTED";
    g_scriptEvents[677] = "BN_SELF_ONLINE";
    g_scriptEvents[678] = "BN_SELF_OFFLINE";
    g_scriptEvents[679] = "BN_FRIEND_LIST_SIZE_CHANGED";
    g_scriptEvents[680] = "BN_FRIEND_INVITE_LIST_INITIALIZED";
    g_scriptEvents[681] = "BN_FRIEND_INVITE_SEND_RESULT";
    g_scriptEvents[682] = "BN_FRIEND_INVITE_ADDED";
    g_scriptEvents[683] = "BN_FRIEND_INVITE_REMOVED";
    g_scriptEvents[684] = "BN_FRIEND_INFO_CHANGED";
    g_scriptEvents[685] = "BN_CUSTOM_MESSAGE_CHANGED";
    g_scriptEvents[686] = "BN_CUSTOM_MESSAGE_LOADED";
    g_scriptEvents[687] = "CHAT_MSG_BN_WHISPER";
    g_scriptEvents[688] = "CHAT_MSG_BN_WHISPER_INFORM";
    g_scriptEvents[689] = "BN_CHAT_WHISPER_UNDELIVERABLE";
    g_scriptEvents[690] = "BN_CHAT_CHANNEL_JOINED";
    g_scriptEvents[691] = "BN_CHAT_CHANNEL_LEFT";
    g_scriptEvents[692] = "BN_CHAT_CHANNEL_CLOSED";
    g_scriptEvents[693] = "CHAT_MSG_BN_CONVERSATION";
    g_scriptEvents[694] = "CHAT_MSG_BN_CONVERSATION_NOTICE";
    g_scriptEvents[695] = "CHAT_MSG_BN_CONVERSATION_LIST";
    g_scriptEvents[696] = "BN_CHAT_CHANNEL_MESSAGE_UNDELIVERABLE";
    g_scriptEvents[697] = "BN_CHAT_CHANNEL_MESSAGE_BLOCKED";
    g_scriptEvents[698] = "BN_CHAT_CHANNEL_MEMBER_JOINED";
    g_scriptEvents[699] = "BN_CHAT_CHANNEL_MEMBER_LEFT";
    g_scriptEvents[700] = "BN_CHAT_CHANNEL_MEMBER_UPDATED";
    g_scriptEvents[701] = "BN_CHAT_CHANNEL_CREATE_SUCCEEDED";
    g_scriptEvents[702] = "BN_CHAT_CHANNEL_CREATE_FAILED";
    g_scriptEvents[703] = "BN_CHAT_CHANNEL_INVITE_SUCCEEDED";
    g_scriptEvents[704] = "BN_CHAT_CHANNEL_INVITE_FAILED";
    g_scriptEvents[705] = "BN_BLOCK_LIST_UPDATED";
    g_scriptEvents[706] = "BN_SYSTEM_MESSAGE";
    g_scriptEvents[707] = "BN_REQUEST_FOF_SUCCEEDED";
    g_scriptEvents[708] = "BN_REQUEST_FOF_FAILED";
    g_scriptEvents[709] = "BN_NEW_PRESENCE";
    g_scriptEvents[710] = "BN_TOON_NAME_UPDATED";
    g_scriptEvents[711] = "BN_FRIEND_ACCOUNT_ONLINE";
    g_scriptEvents[712] = "BN_FRIEND_ACCOUNT_OFFLINE";
    g_scriptEvents[713] = "BN_FRIEND_TOON_ONLINE";
    g_scriptEvents[714] = "BN_FRIEND_TOON_OFFLINE";
    g_scriptEvents[715] = "BN_MATURE_LANGUAGE_FILTER";
    g_scriptEvents[716] = "COMMENTATOR_SKIRMISH_QUEUE_REQUEST";
    g_scriptEvents[717] = "COMMENTATOR_SKIRMISH_MODE_REQUEST";
    g_scriptEvents[718] = "CHAT_MSG_BN_INLINE_TOAST_ALERT";
    g_scriptEvents[719] = "CHAT_MSG_BN_INLINE_TOAST_BROADCAST";
    g_scriptEvents[720] = "CHAT_MSG_BN_INLINE_TOAST_BROADCAST_INFORM";
    g_scriptEvents[721] = "CHAT_MSG_BN_INLINE_TOAST_CONVERSATION";
}

void ScriptEventsRegisterEvents() {
    FrameScript_CreateEvents(g_scriptEvents, NUM_SCRIPT_EVENTS);
}
