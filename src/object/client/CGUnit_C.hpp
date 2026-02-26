#ifndef OBJECT_CLIENT_CG_UNIT_C_HPP
#define OBJECT_CLIENT_CG_UNIT_C_HPP

#include "object/client/CClientObjCreate.hpp"
#include "object/client/CGObject_C.hpp"
#include "object/client/CGUnit.hpp"
#include "object/client/CMovementData_C.hpp"
#include "object/Types.hpp"
#include "util/GUID.hpp"

class ChrClassesRec;
class ChrRacesRec;
class CreatureDisplayInfoRec;
class CreatureDisplayInfoExtraRec;
class CreatureModelDataRec;
class CreatureSoundDataRec;
class UnitBloodLevelsRec;

class CGUnit_C : public CGObject_C, public CGUnit {
    public:
        // Public static variables
        static WOWGUID s_activeMover;

        // Public static functions
        static const char* GetDisplayClassNameFromRecord(const ChrClassesRec* classRec, UNIT_SEX sex, UNIT_SEX* displaySex);
        static const char* GetDisplayRaceNameFromRecord(const ChrRacesRec* raceRec, UNIT_SEX sex, UNIT_SEX* displaySex);

        // Virtual public member functions
        virtual ~CGUnit_C();
        // TODO
        virtual int32_t GetModelFileName(const char*& name) const;
        // TODO
        virtual int32_t CanHighlight();
        virtual int32_t CanBeTargetted();
        // TODO

        // Public member functions
        CGUnit_C(uint32_t time, CClientObjCreate& objCreate);
        int32_t GetDisplayID() const;
        CreatureModelDataRec* GetModelData() const;
        void PostInit(uint32_t time, const CClientObjCreate& init, bool a4);
        void PostMovementUpdate(const CClientMoveUpdate& move, int32_t activeMover);
        void SetStorage(uint32_t* storage, uint32_t* saved);

    protected:
        // Protected member functions
        int32_t GetLocalDisplayID() const;
        void RefreshDataPointers();

    private:
        // Private member variables
        CMovementData_C* m_movement;
        // TODO
        CMovementData_C m_localMovement;
        // TODO
        CreatureDisplayInfoRec* m_displayInfo;
        CreatureDisplayInfoExtraRec* m_displayInfoExtra;
        CreatureModelDataRec* m_modelData;
        CreatureSoundDataRec* m_soundData;
        // TODO
        UnitBloodLevelsRec* m_bloodRec;
        // TODO
        int32_t m_localDisplayID = 0;
        // TODO
};

#endif
