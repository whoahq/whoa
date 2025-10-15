#include "glue/CGlueLoading.hpp"
#include "component/CCharacterComponent.hpp"
#include "model/Model2.hpp"
#include "util/SFile.hpp"

int32_t CGlueLoading::s_loadState;

void CGlueLoading::StartLoad(CCharacterComponent* component, int32_t a2) {
    if (SFile::IsStreamingTrial()) {
        // TODO

        return;
    }

    if (component) {
        component->RenderPrep(1);
        component->m_data.model->IsDrawable(1, 1);
    }

    CGlueLoading::s_loadState = 3;
}
