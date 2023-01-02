#ifndef UI_C_FRAME_STRATA_HPP
#define UI_C_FRAME_STRATA_HPP

#include "ui/CRenderBatch.hpp"
#include "ui/CSimpleFrame.hpp"
#include <cstdint>
#include <storm/Array.hpp>
#include <storm/List.hpp>

class CFrameStrataNode {
    public:
        // Member variables
        STORM_EXPLICIT_LIST(CSimpleFrame, m_strataLink) pendingFrames;
        STORM_EXPLICIT_LIST(CSimpleFrame, m_strataLink) frames;
        CSimpleFrame* pendingFrame = nullptr;
        CRenderBatch batches[5];
        int32_t batchDirty = 0;
        STORM_EXPLICIT_LIST(CRenderBatch, renderLink) renderList;

        // Member functions
        int32_t BuildBatches(void);
        void OnLayerUpdate(float);
        bool RemoveFrame(CSimpleFrame*);
};

class CFrameStrata {
    public:
        // Member variables
        int32_t batchDirty = 0;
        int32_t levelsDirty = 0;
        uint32_t topLevel = 0;
        TSFixedArray<CFrameStrataNode*> levels;

        // Member functions
        void AddFrame(CSimpleFrame*);
        int32_t BuildBatches(int32_t);
        void CheckOcclusion(void);
        int32_t FrameOccluded(CSimpleFrame*);
        void OnLayerUpdate(float);
        void RemoveFrame(CSimpleFrame*);
        void RenderBatches(void);
};

#endif
