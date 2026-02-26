#include "object/client/CGUnit_C.hpp"
#include "component/CCharacterComponent.hpp"
#include "model/Model2.hpp"
#include "object/client/ObjMgr.hpp"
#include "db/Db.hpp"
#include "ui/Game.hpp"
#include <storm/Error.hpp>

WOWGUID CGUnit_C::s_activeMover;

const char* CGUnit_C::GetDisplayClassNameFromRecord(const ChrClassesRec* classRec, UNIT_SEX sex, UNIT_SEX* displaySex) {
    if (displaySex) {
        *displaySex = sex;
    }

    if (!classRec) {
        return nullptr;
    }

    if (sex == UNITSEX_MALE) {
        if (*classRec->m_nameMale) {
            return classRec->m_nameMale;
        }

        if (*classRec->m_nameFemale) {
            if (displaySex) {
                *displaySex = UNITSEX_FEMALE;
            }

            return classRec->m_nameFemale;
        }

        return classRec->m_name;
    }

    if (sex == UNITSEX_FEMALE) {
        if (*classRec->m_nameFemale) {
            return classRec->m_nameFemale;
        }

        if (*classRec->m_nameMale) {
            if (displaySex) {
                *displaySex = UNITSEX_MALE;
            }

            return classRec->m_nameMale;
        }

        return classRec->m_name;
    }

    return classRec->m_name;
}

const char* CGUnit_C::GetDisplayRaceNameFromRecord(const ChrRacesRec* raceRec, UNIT_SEX sex, UNIT_SEX* displaySex) {
    if (displaySex) {
        *displaySex = sex;
    }

    if (!raceRec) {
        return nullptr;
    }

    if (sex == UNITSEX_MALE) {
        if (*raceRec->m_nameMale) {
            return raceRec->m_nameMale;
        }

        if (*raceRec->m_nameFemale) {
            if (displaySex) {
                *displaySex = UNITSEX_FEMALE;
            }

            return raceRec->m_nameFemale;
        }

        return raceRec->m_name;
    }

    if (sex == UNITSEX_FEMALE) {
        if (*raceRec->m_nameFemale) {
            return raceRec->m_nameFemale;
        }

        if (*raceRec->m_nameMale) {
            if (displaySex) {
                *displaySex = UNITSEX_MALE;
            }

            return raceRec->m_nameMale;
        }

        return raceRec->m_name;
    }

    return raceRec->m_name;
}

CGUnit_C::CGUnit_C(uint32_t time, CClientObjCreate& objCreate)
    : CGObject_C(time, objCreate)
    , m_localMovement(objCreate.move.status.position28, objCreate.move.status.facing34, this->GetGUID(), this)
{
    // TODO

    this->RefreshDataPointers();

    // TODO
}

CGUnit_C::~CGUnit_C() {
    // TODO
}

int32_t CGUnit_C::CanHighlight() {
    if (this->m_unit->flags & 0x2000000) {
        if (this->m_unit->createdBy != ClntObjMgrGetActivePlayer() || this->GetGUID() != CGPetInfo::GetPet(0)) {
            return false;
        }
    }

    return true;
}

int32_t CGUnit_C::CanBeTargetted() {
    return this->CanHighlight();
}

int32_t CGUnit_C::GetDisplayID() const {
    // Prefer local display ID if set and unit's display ID hasn't been overridden from unit's
    // native display ID.
    if (this->GetLocalDisplayID() && this->GetDisplayID() == this->GetNativeDisplayID()) {
        return this->GetLocalDisplayID();
    }

    return this->CGUnit::GetDisplayID();
}

int32_t CGUnit_C::GetLocalDisplayID() const {
    return this->m_localDisplayID;
}

CreatureModelDataRec* CGUnit_C::GetModelData() const {
    auto displayID = this->GetDisplayID();

    auto creatureDisplayInfoRec = g_creatureDisplayInfoDB.GetRecord(displayID);

    if (!creatureDisplayInfoRec) {
        // TODO SysMsgPrintf(1, 2, "NOCREATUREDISPLAYIDFOUND|%d", displayID);
        return nullptr;
    }

    auto creatureModelDataRec = g_creatureModelDataDB.GetRecord(creatureDisplayInfoRec->m_modelID);

    if (!creatureModelDataRec) {
        // TODO SysMsgPrintf(1, 16, "INVALIDDISPLAYMODELRECORD|%d|%d", creatureDisplayInfoRec->m_modelID, creatureDisplayInfoRec->m_ID);
        return nullptr;
    }

    return creatureModelDataRec;
}

int32_t CGUnit_C::GetModelFileName(const char*& name) const {
    auto modelDataRec = this->GetModelData();

    // Model data not found
    if (!modelDataRec) {
        name = "Spells\\ErrorCube.mdx";

        return true;
    }

    name = modelDataRec->m_modelName;

    return modelDataRec->m_modelName ? true : false;
}

void CGUnit_C::PostInit(uint32_t time, const CClientObjCreate& init, bool a4) {
    // TODO

    this->CGObject_C::PostInit(time, init, a4);

    // TODO

    if (this->m_displayInfo) {
        CCharacterComponent::ApplyMonsterGeosets(this->m_model, this->m_displayInfo);
        CCharacterComponent::ReplaceMonsterSkin(this->m_model, this->m_displayInfo, this->m_modelData);

        if (this->m_modelData) {
            this->m_model->m_flag4 = (this->m_modelData->m_flags & 0x200) ? true : false;
        }
    }

    // TODO
}

void CGUnit_C::PostMovementUpdate(const CClientMoveUpdate& move, int32_t activeMover) {
    // TODO
}

void CGUnit_C::RefreshDataPointers() {
    auto displayID = this->GetDisplayID();

    // Display info

    this->m_displayInfo = g_creatureDisplayInfoDB.GetRecord(displayID);

    if (!this->m_displayInfo) {
        // TODO auto name = this->GetUnitName(0, 1);
        // TODO SysMsgPrintf(2, 2, "NOUNITDISPLAYID|%d|%s", displayID, name);

        this->m_displayInfo = g_creatureDisplayInfoDB.GetRecordByIndex(0);

        if (!this->m_displayInfo) {
            STORM_APP_FATAL("Error, NO creature display records found");
        }
    }

    // Display info extra

    this->m_displayInfoExtra = g_creatureDisplayInfoExtraDB.GetRecord(this->m_displayInfo->m_extendedDisplayInfoID);

    // Model data

    this->m_modelData = g_creatureModelDataDB.GetRecord(this->m_displayInfo->m_modelID);

    // Sound data

    this->m_soundData = g_creatureSoundDataDB.GetRecord(this->m_displayInfo->m_soundID);

    if (!this->m_soundData) {
        this->m_soundData = g_creatureSoundDataDB.GetRecord(this->m_modelData->m_soundID);
    }

    // Blood levels

    this->m_bloodRec = g_unitBloodLevelsDB.GetRecord(this->m_displayInfo->m_bloodID);

    if (!this->m_bloodRec) {
        this->m_bloodRec = g_unitBloodLevelsDB.GetRecord(this->m_modelData->m_bloodID);

        if (!this->m_bloodRec) {
            this->m_bloodRec = g_unitBloodLevelsDB.GetRecordByIndex(0);
        }
    }

    // Creature stats

    if (this->GetType() == HIER_TYPE_UNIT) {
        // TODO load creature stats
    }

    // Flags

    // TODO set flags
}

void CGUnit_C::SetStorage(uint32_t* storage, uint32_t* saved) {
    this->CGObject_C::SetStorage(storage, saved);

    this->m_unit = reinterpret_cast<CGUnitData*>(&storage[CGUnit::GetBaseOffset()]);
    this->m_unitSaved = &saved[CGUnit::GetBaseOffsetSaved()];
}
