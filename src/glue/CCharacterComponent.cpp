#include "glue/CCharacterComponent.hpp"
#include "model/CM2Model.hpp"
#include <storm/Memory.hpp>

CCharacterComponent* CCharacterComponent::AllocComponent() {
    // TODO ObjectAlloc
    return STORM_NEW(CCharacterComponent);
}

void CCharacterComponent::Init(ComponentData* data, const char* a3) {
    if (data->model) {
        data->model->Release();
    }

    this->m_data = *data;

    // TODO
}
