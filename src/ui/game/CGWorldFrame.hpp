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

        // Member functions
        CGWorldFrame(CSimpleFrame* parent);
};

#endif
