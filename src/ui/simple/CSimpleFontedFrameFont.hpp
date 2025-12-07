#ifndef UI_SIMPLE_C_SIMPLE_FONTED_FRAME_FONT_HPP
#define UI_SIMPLE_C_SIMPLE_FONTED_FRAME_FONT_HPP

#include "ui/simple/CSimpleFont.hpp"

class CSimpleFontedFrame;

class CSimpleFontedFrameFont : public CSimpleFont {
    public:
        // Member variables
        CSimpleFontedFrame* m_frame;

        // Member functions
        CSimpleFontedFrameFont(CSimpleFontedFrame* frame)
            : m_frame(frame)
            {};
};

#endif
