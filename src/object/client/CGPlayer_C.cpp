#include "object/client/CGPlayer_C.hpp"
#include "db/Db.hpp"
#include "object/client/ObjMgr.hpp"
#include "object/Types.hpp"
#include <storm/Error.hpp>

CGPlayer_C::CGPlayer_C(uint32_t time, CClientObjCreate& objCreate) : CGUnit_C(time, objCreate) {
    // TODO
}

CGPlayer_C::~CGPlayer_C() {
    // TODO
}

uint32_t CGPlayer_C::GetActiveNextLevelXP() const {
    if (this->GetGUID() != ClntObjMgrGetActivePlayer()) {
        return 0;
    }

    return this->GetNextLevelXP();
}

uint32_t CGPlayer_C::GetActiveXP() const {
    if (this->GetGUID() != ClntObjMgrGetActivePlayer()) {
        return 0;
    }

    return this->GetXP();
}

void CGPlayer_C::PostInit(uint32_t time, const CClientObjCreate& init, bool a4) {
    // TODO

    this->CGUnit_C::PostInit(time, init, a4);

    // TODO
}

void CGPlayer_C::SetStorage(uint32_t* storage, uint32_t* saved) {
    this->CGUnit_C::SetStorage(storage, saved);

    this->m_player = reinterpret_cast<CGPlayerData*>(&storage[CGPlayer::GetBaseOffset()]);
    this->m_playerSaved = &saved[CGPlayer::GetBaseOffsetSaved()];
}

uint32_t Player_C_GetDisplayId(uint32_t race, uint32_t sex) {
    STORM_ASSERT(sex < UNITSEX_LAST);

    auto raceRec = g_chrRacesDB.GetRecord(race);

    if (!raceRec) {
        return 0;
    }

    if (sex == UNITSEX_MALE) {
        return raceRec->m_maleDisplayID;
    }

    if (sex == UNITSEX_FEMALE) {
        return raceRec->m_femaleDisplayID;
    }

    return 0;
}

const CreatureModelDataRec* Player_C_GetModelName(uint32_t race, uint32_t sex) {
    STORM_ASSERT(sex < UNITSEX_LAST);

    auto displayID = Player_C_GetDisplayId(race, sex);

    if (!displayID) {
        return nullptr;
    }

    auto displayInfo = g_creatureDisplayInfoDB.GetRecord(displayID);

    if (!displayInfo) {
        // TODO this becomes nullsub in retail build -- might be variation of macro
        STORM_APP_FATAL("Error, unknown displayInfo %d specified for player race %d sex %d!", displayID, race, sex);
    }

    auto modelData = g_creatureModelDataDB.GetRecord(displayInfo->m_modelID);

    if (!modelData) {
        // TODO this becomes nullsub in retail build -- might be variation of macro
        STORM_APP_FATAL("Error, unknown model record %d specified for player race %d sex %d!", displayInfo->m_modelID, race, sex);
    }

    return modelData;
}
