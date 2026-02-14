#ifndef UI_GAME_C_G_WORLD_FRAME_HPP
#define UI_GAME_C_G_WORLD_FRAME_HPP

#include "ui/simple/CSimpleFrame.hpp"
#include <cstdint>

class CGWorldFrame : public CSimpleFrame {
    public:
        // Static variables
        CGWorldFrame* s_currentWorldFrame;

        // Static functions
        static CSimpleFrame* Create(CSimpleFrame* parent);
        static void RenderWorld(void* param);

        // Virtual member functions
        virtual void OnFrameRender(CRenderBatch* batch, uint32_t layer);

        // Member functions
        CGWorldFrame(CSimpleFrame* parent);
        void OnWorldRender();
        void OnWorldUpdate();
};

#endif
