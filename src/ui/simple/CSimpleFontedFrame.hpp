#ifndef UI_SIMPLE_C_SIMPLE_FONTED_FRAME_HPP
#define UI_SIMPLE_C_SIMPLE_FONTED_FRAME_HPP

#include <cstdint>

class CSimpleFontedFrameFont;

class CSimpleFontedFrame {
    public:
        // Virtual member functions
        virtual void FontUpdated(CSimpleFontedFrameFont*, int32_t) = 0;
};

#endif
