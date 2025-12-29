#include "glue/CCharacterCreation.hpp"
#include "component/Types.hpp"
#include "glue/CGlueLoading.hpp"
#include "ui/simple/CSimpleModelFFX.hpp"

CCharacterComponent* CCharacterCreation::s_character;
CSimpleModelFFX* CCharacterCreation::s_charCustomizeFrame;
int32_t CCharacterCreation::s_existingCharacterIndex;
int32_t CCharacterCreation::s_raceIndex;
int32_t CCharacterCreation::s_selectedClassID;

void CCharacterCreation::CalcClasses(int32_t raceID) {
    // TODO
}

void CCharacterCreation::CreateComponent(ComponentData* data, bool randomize) {
    // TODO
}

int32_t CCharacterCreation::GetRandomClassID() {
    // TODO
    return 1;
}

void CCharacterCreation::GetRandomRaceAndSex(ComponentData* data) {
    // TODO
}

void CCharacterCreation::ResetCharCustomizeInfo() {
    if (!CCharacterCreation::s_charCustomizeFrame) {
        return;
    }

    CCharacterCreation::s_existingCharacterIndex = -1;

    auto model = CCharacterCreation::s_charCustomizeFrame->m_model;

    if (model) {
        model->DetachAllChildrenById(0);
    }

    ComponentData data;
    CCharacterCreation::GetRandomRaceAndSex(&data);
    CCharacterCreation::CalcClasses(data.raceID);

    CCharacterCreation::CreateComponent(&data, true);

    CCharacterCreation::SetSelectedClass(CCharacterCreation::GetRandomClassID());
    data.classID = CCharacterCreation::s_selectedClassID;

    CCharacterCreation::s_raceIndex = -1;

    // TODO race and name gen stuff

    CGlueLoading::StartLoad(CCharacterCreation::s_character, true);
}

void CCharacterCreation::SetSelectedClass(int32_t classID) {
    // TODO
}
