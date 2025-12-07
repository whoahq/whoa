#include "ui/CFrameStrata.hpp"
#include "ui/simple/CSimpleFrame.hpp"
#include "ui/simple/CSimpleRender.hpp"

int32_t CFrameStrataNode::BuildBatches() {
    if (!this->batchDirty) {
        return 0;
    }

    int32_t batchDirty = this->batchDirty;
    this->batchDirty = 0;

    for (int32_t layer = 0; layer < NUM_SIMPLEFRAME_DRAWLAYERS; layer++) {
        CRenderBatch* batch = &this->batches[layer];

        this->renderList.UnlinkNode(batch);

        if ((1 << layer) & batchDirty) {
            batch->Clear();

            for (auto frame = this->frames.Head(); frame; frame = this->frames.Link(frame)->Next()) {
                if (!(frame->m_flags & 0x2000)) {
                    frame->OnFrameRender(batch, layer);
                }
            }

            batch->Finish();
        }

        if (batch->m_count) {
            this->renderList.LinkToTail(batch);
        }
    }

    if (this->batchDirty & 0x20) {
        this->batchDirty |= 0x1F;
    }

    return 0;
}

void CFrameStrataNode::OnLayerUpdate(float elapsedSec) {
    auto frame = this->frames.Head();

    while (frame) {
        auto next = this->frames.Link(frame)->Next();
        this->pendingFrame = next;
        frame->OnLayerUpdate(elapsedSec);
        frame = this->pendingFrame;
    }

    // TODO
    // this->frames.Combine(this->pendingFrames, 2, nullptr);
}

bool CFrameStrataNode::RemoveFrame(CSimpleFrame* frame) {
    if (!this->frames.Link(frame)->IsLinked()) {
        return 0;
    }

    if (frame == this->pendingFrame) {
        this->pendingFrame = this->frames.Link(frame)->Next();
    }

    this->frames.UnlinkNode(frame);

    if (!(frame->m_flags & 0x2000)) {
        this->batchDirty = -1;
    }

    return this->batchDirty != 0;
}

void CFrameStrata::AddFrame(CSimpleFrame* frame) {
    // TODO
    // - potentially an inlined TSFixedArray function?
    if (frame->m_level >= this->levels.Count()) {
        uint32_t count = this->levels.Count();

        this->levels.SetCount(frame->m_level + 1);

        for (int32_t i = count; i < frame->m_level + 1; i++) {
            auto m = SMemAlloc(sizeof(CFrameStrataNode), __FILE__, __LINE__, 0x0);
            auto node = new (m) CFrameStrataNode();
            this->levels[i] = node;
        }
    }

    if (frame->m_level >= this->topLevel) {
        this->topLevel = frame->m_level + 1;
    }

    auto level = this->levels[frame->m_level];

    if (!frame->m_strataLink.IsLinked()) {
        auto frames = level->pendingFrame ? &level->pendingFrames : &level->frames;

        frames->LinkToTail(frame);

        if (!(frame->m_flags & 0x2000)) {
            level->batchDirty = -1;
        }
    }

    this->levelsDirty = 1;
    this->batchDirty |= (level->batchDirty != 0);
}

void CFrameStrata::CheckOcclusion() {
    // TODO
}

int32_t CFrameStrata::BuildBatches(int32_t a2) {
    if (this->levelsDirty) {
        this->CheckOcclusion();
        this->levelsDirty = 0;
    }

    if (!this->batchDirty) {
        return this->batchDirty;
    }

    this->batchDirty = 0;

    if (this->topLevel == 0) {
        return 0;
    }

    for (int32_t i = 0; i < this->topLevel; i++) {
        if (this->levels[i]->BuildBatches()) {
            this->batchDirty = 1;
        }
    }

    return this->batchDirty;
}

int32_t CFrameStrata::FrameOccluded(CSimpleFrame* frame) {
    // TODO
    return 0;
}

void CFrameStrata::OnLayerUpdate(float elapsedSec) {
    if (this->topLevel) {
        for (int32_t l = 0; l < this->topLevel; l++) {
            this->levels[l]->OnLayerUpdate(elapsedSec);
        }
    }
}

void CFrameStrata::RemoveFrame(CSimpleFrame* frame) {
    if (frame->m_level < this->levels.Count()) {
        CFrameStrataNode* level = this->levels[frame->m_level];
        int32_t batchDirty = level->RemoveFrame(frame);
        this->batchDirty |= batchDirty;
        this->levelsDirty = 1;
    }
}

void CFrameStrata::RenderBatches() {
    for (int32_t i = 0; i < this->topLevel; i++) {
        auto renderList = &this->levels[i]->renderList;

        for (auto batch = renderList->Head(); batch; batch = renderList->Link(batch)->Next()) {
            CSimpleRender::DrawBatch(batch);
        }
    }
}
