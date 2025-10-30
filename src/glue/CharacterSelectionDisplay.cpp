#include "glue/CharacterSelectionDisplay.hpp"
#include "db/Db.hpp"
#include "glue/CCharacterSelection.hpp"
#include "component/CCharacterComponent.hpp"
#include "object/client/Player_C.hpp"
#include "ui/CSimpleModelFFX.hpp"

void CharacterSelectionDisplay::CreateModelData() {
    auto modelData = Player_C_GetModelName(this->m_info.raceID, this->m_info.sexID);

    if (!modelData || !modelData->m_modelName) {
        return;
    }

    this->m_component = CCharacterComponent::AllocComponent();

    ComponentData componentData = {};
    componentData.raceID = this->m_info.raceID;
    componentData.sexID = this->m_info.sexID;
    componentData.skinColorID = this->m_info.skinColorID;
    componentData.faceID = this->m_info.faceID;
    componentData.hairStyleID = this->m_info.hairStyleID;
    componentData.hairColorID = this->m_info.hairColorID;
    componentData.facialHairStyleID = this->m_info.facialHairStyleID;

    auto scene = CCharacterSelection::s_modelFrame->GetScene();
    auto model = scene->CreateModel(modelData->m_modelName, 0);

    componentData.flags |= 0x2;
    componentData.model = model;

    this->m_component->Init(&componentData, nullptr);

    // TODO lighting callback/arg

    this->m_component->m_data.model->SetBoneSequence(
        0xFFFFFFFF,
        0,
        0xFFFFFFFF,
        0,
        1.0f,
        1,
        1
    );

    // TODO load pet model

    CCharacterSelection::s_characterCount++;
}
