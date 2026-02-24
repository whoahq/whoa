#ifndef UI_GAME_C_G_WORLD_FRAME_HPP
#define UI_GAME_C_G_WORLD_FRAME_HPP

#include "ui/simple/CSimpleFrame.hpp"
#include <cstdint>

class CGCamera;

class CGWorldFrame : public CSimpleFrame {
    public:
        // Static variables
        CGWorldFrame* s_currentWorldFrame;

        // Static functions
        static CSimpleFrame* Create(CSimpleFrame* parent);
        static void RenderWorld(void* param);

        // Virtual member functions
        virtual void OnFrameRender(CRenderBatch* batch, uint32_t layer);
        // TODO
        virtual void OnFrameSizeChanged(const CRect& rect);

        // Member functions
        CGWorldFrame(CSimpleFrame* parent);
        void OnWorldRender();
        void OnWorldUpdate();

    private:
        // Private member variables
        // TODO
        CRect m_screenRect;
        CRect m_viewport;
        // TODO
        CGCamera* m_camera;
        // TODO
};

#endif
