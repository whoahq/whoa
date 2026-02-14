#include "ui/game/CGWorldFrame.hpp"
#include "gx/Shader.hpp"
#include "gx/Transform.hpp"
#include "ui/game/PlayerName.hpp"
#include <storm/Memory.hpp>
#include <tempest/Matrix.hpp>

CSimpleFrame* CGWorldFrame::Create(CSimpleFrame* parent) {
    // TODO use CDataAllocator

    return STORM_NEW(CGWorldFrame)(parent);
}

void CGWorldFrame::RenderWorld(void* param) {
    auto frame = reinterpret_cast<CGWorldFrame*>(param);

    C44Matrix savedProj;
    GxXformProjection(savedProj);

    C44Matrix savedView;
    GxXformView(savedView);

    frame->OnWorldUpdate();
    PlayerNameUpdateWorldText();

    frame->OnWorldRender();
    PlayerNameRenderWorldText();

    GxXformSetProjection(savedProj);
    GxXformSetView(savedView);

    CShaderEffect::UpdateProjMatrix();
}

CGWorldFrame::CGWorldFrame(CSimpleFrame* parent) : CSimpleFrame(parent) {
    // TODO

    CGWorldFrame::s_currentWorldFrame = this;

    // TODO

    this->SetFrameStrata(FRAME_STRATA_WORLD);

    this->EnableEvent(SIMPLE_EVENT_KEY, -1);
    this->EnableEvent(SIMPLE_EVENT_MOUSE, -1);
    this->EnableEvent(SIMPLE_EVENT_MOUSEWHEEL, -1);

    // TODO
}

void CGWorldFrame::OnFrameRender(CRenderBatch* batch, uint32_t layer) {
    this->CSimpleFrame::OnFrameRender(batch, layer);

    if (layer == DRAWLAYER_BACKGROUND) {
        batch->QueueCallback(&CGWorldFrame::RenderWorld, this);
    }
}

void CGWorldFrame::OnWorldRender() {
    // TODO
}

void CGWorldFrame::OnWorldUpdate() {
    // TODO
}
