#include "component/CCharacterComponent.hpp"
#include "model/CM2Model.hpp"
#include <storm/Memory.hpp>

CCharacterComponent* CCharacterComponent::AllocComponent() {
    // TODO ObjectAlloc
    return STORM_NEW(CCharacterComponent);
}

void CCharacterComponent::GeosRenderPrep() {
    // TODO

    // Default all sections (0 - 2000) to hidden
    this->m_data.model->SetGeometryVisible(0, 2000, 0);

    // Show base "skin" section (0)
    this->m_data.model->SetGeometryVisible(0, 0, 1);

    // Show selected geosets
    for (int32_t i = 0; i < NUM_GEOSET; i++) {
        // TODO handle 1703

        this->m_data.model->SetGeometryVisible(this->m_data.geosets[i], this->m_data.geosets[i], 1);
    }

    // TODO

    this->m_flags &= ~0x4;
}

void CCharacterComponent::Init(ComponentData* data, const char* a3) {
    if (data->model) {
        data->model->Release();
    }

    this->m_data = *data;

    // TODO
}

void CCharacterComponent::RenderPrep(int32_t a2) {
    if (this->m_data.flags & 0x1) {
        if (this->m_flags & 0x4) {
            this->GeosRenderPrep();
        }

        return;
    }

    // TODO
}
