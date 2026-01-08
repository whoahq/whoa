#include "object/client/CGPlayer_C.hpp"
#include "db/Db.hpp"
#include "object/Types.hpp"
#include <storm/Error.hpp>

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
