#ifndef UI_C_FRAME_POINT_HPP
#define UI_C_FRAME_POINT_HPP

#include "ui/Types.hpp"
#include <cstdint>
#include <tempest/Vector.hpp>

class CLayoutFrame;
class CRect;

class CFramePoint {
    public:
        // Static variables
        static float const UNDEFINED;

        // Member variables
        C2Vector m_offset;
        CLayoutFrame* m_relative;
        uint32_t m_framePoint : 8;
        uint32_t m_flags : 24;

        // Member functions
        CFramePoint(CLayoutFrame*, FRAMEPOINT, float, float);
        CLayoutFrame* GetRelative();
        int32_t GetRelativeRect(CRect&);
        void MarkUnused(void);
        void SetRelative(CLayoutFrame*, FRAMEPOINT, float, float);
        float X(float);
        float Y(float);
};

#endif
