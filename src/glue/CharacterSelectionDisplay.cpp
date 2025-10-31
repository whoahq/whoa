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

    for (int32_t invSlot = INVSLOT_FIRST; invSlot < NUM_INVENTORY_SLOTS; invSlot++) {
        auto displayID = this->m_info.inventoryItemDisplayID[invSlot];
        auto visualID = this->m_info.inventoryItemVisualID[invSlot];

        // Only honor ranged slot for hunters
        if (invSlot == INVSLOT_RANGED && displayID && this->m_info.classID != 3) {
            continue;
        }

        if (invSlot == INVSLOT_HEAD && (this->m_info.flags & 0x400)) {
            continue;
        }

        if (invSlot == INVSLOT_BACK && (this->m_info.flags & 0x800)) {
            continue;
        }

        auto displayRec = g_itemDisplayInfoDB.GetRecord(displayID);

        if (!displayRec) {
            continue;
        }

        // Item visuals in database override item visuals in character info
        if (g_itemVisualsDB.GetRecord(displayRec->m_itemVisual)) {
            visualID = 0;
        }

        // Bags

        if (invSlot >= INVSLOT_BAGFIRST) {
            if (*displayRec->m_modelName[0]) {
                this->m_component->AddItem(ITEMSLOT_11, displayRec, 0);
            }

            continue;
        }

        // Main hand and off hand

        if (invSlot == INVSLOT_MAINHAND || invSlot == INVSLOT_OFFHAND) {
            CCharacterComponent::AddHandItem(
                this->m_component->m_data.model,
                displayRec,
                static_cast<INVENTORY_SLOTS>(invSlot),
                SHEATHE_0,
                false,
                this->m_info.inventoryItemType[invSlot] == 14,
                false,
                visualID
            );

            continue;
        }

        // Ranged

        if (invSlot == INVSLOT_RANGED) {
            CCharacterComponent::AddHandItem(
                this->m_component->m_data.model,
                displayRec,
                static_cast<INVENTORY_SLOTS>(invSlot),
                SHEATHE_0,
                false,
                this->m_info.inventoryItemType[invSlot] == 14,
                false,
                visualID
            );

            // TODO

            continue;
        }

        // Tabard

        if (invSlot == INVSLOT_TABARD) {
            // TODO

            continue;
        }

        // All other slots

        this->m_component->AddItemBySlot(static_cast<INVENTORY_SLOTS>(invSlot), displayID, visualID);
    }

    // TODO load pet model

    CCharacterSelection::s_characterCount++;
}
