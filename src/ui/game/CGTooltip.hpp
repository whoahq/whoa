#ifndef UI_GAME_C_G_TOOLTIP_HPP
#define UI_GAME_C_G_TOOLTIP_HPP

#include "ui/simple/CSimpleFrame.hpp"

class CGTooltip : public CSimpleFrame {
    public:
        // Static functions
        static CSimpleFrame* Create(CSimpleFrame* parent);

        // Member variables
        // TODO

        // Member functions
        CGTooltip(CSimpleFrame* parent);
};

#endif
