#include "glue/CCharacterCreation.hpp"
#include "component/CCharacterComponent.hpp"
#include "component/Types.hpp"
#include "db/Db.hpp"
#include "glue/CGlueLoading.hpp"
#include "object/client/Player_C.hpp"
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
    auto modelData = Player_C_GetModelName(data->raceID, data->sexID);

    if (!modelData || !modelData->m_modelName) {
        return;
    }

    auto existingComponent = CCharacterCreation::s_character;

    if (existingComponent) {
        auto model = existingComponent->m_data.model;

        if (model->m_attachParent) {
            model->DetachFromParent();
        }

        // TODO CCharacterComponent::FreeComponent(existingComponent);
    }

    CCharacterCreation::s_character = CCharacterComponent::AllocComponent();

    auto scene = CCharacterCreation::s_charCustomizeFrame->GetScene();
    auto model = scene->CreateModel(modelData->m_modelName, 0);
    data->model = model;

    if (!model) {
        return;
    }

    // TODO lighting

    data->model->SetBoneSequence(-1, 0, 0, 0, 1.0f, 1, 1);

    data->flags |= 0x2;

    CCharacterCreation::s_character->Init(data, nullptr);

    if (randomize) {
        // TODO select random variations
    }

    // TODO track previous variations

    // TODO set facing

    CCharacterCreation::Dress();

    CCharacterCreation::s_character->RenderPrep(0);

    if (CCharacterCreation::s_charCustomizeFrame->m_model) {
        model->AttachToParent(CCharacterCreation::s_charCustomizeFrame->m_model, 0, nullptr, 0);
    }
}

void CCharacterCreation::Dress() {
    // TODO
}

int32_t CCharacterCreation::GetRandomClassID() {
    // TODO
    return 1;
}

void CCharacterCreation::GetRandomRaceAndSex(ComponentData* data) {
    // TODO
    data->raceID = 1;
    data->sexID = UNITSEX_MALE;
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
