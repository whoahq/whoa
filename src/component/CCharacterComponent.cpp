#include "component/CCharacterComponent.hpp"
#include "model/CM2Model.hpp"
#include <storm/Memory.hpp>

int32_t s_bInRenderPrep = 0;

CCharacterComponent* CCharacterComponent::AllocComponent() {
    // TODO ObjectAlloc
    return STORM_NEW(CCharacterComponent);
}

void CCharacterComponent::Initialize() {
    // TODO

    // TODO proper implementation
    CCharacterComponent::Initialize(GxTex_Rgb565, 9, 0, 0);
}

void CCharacterComponent::Initialize(EGxTexFormat textureFormat, uint32_t textureLevel, int32_t thread, int32_t compress) {
    // TODO
}

void CCharacterComponent::CreateBaseTexture() {
    // TODO
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

int32_t CCharacterComponent::ItemsLoaded(int32_t a2) {
    // TODO

    return 1;
}

void CCharacterComponent::PrepSections() {
    // TODO
}

int32_t CCharacterComponent::RenderPrep(int32_t a2) {
    if (this->m_data.flags & 0x1) {
        if (this->m_flags & 0x4) {
            this->GeosRenderPrep();
        }

        return 1;
    }

    // TODO

    if (a2) {
        // TODO

        this->VariationsLoaded(1);
        this->ItemsLoaded(1);

        this->m_flags |= 8u;

        this->RenderPrepSections();
        // TODO this->Sub79F820();

        return 1;
    }

    // TODO
}

void CCharacterComponent::RenderPrepSections() {
    s_bInRenderPrep = 1;

    if (this->m_flags & 0x4) {
        this->GeosRenderPrep();
    }

    if (!this->m_baseTexture) {
        this->CreateBaseTexture();
    }

    this->PrepSections();

    this->m_flags &= ~0x1;

    // TODO

    s_bInRenderPrep = 0;
}

int32_t CCharacterComponent::VariationsLoaded(int32_t a2) {
    // TODO

    return 1;
}
