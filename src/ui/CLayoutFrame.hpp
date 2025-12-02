#ifndef UI_C_LAYOUT_FRAME_HPP
#define UI_C_LAYOUT_FRAME_HPP

#include "ui/Types.hpp"
#include "ui/Util.hpp"
#include <cstdint>
#include <storm/List.hpp>
#include <tempest/Rect.hpp>

class CFramePoint;
class CStatus;
class XMLNode;

class CLayoutFrame {
    public:
        // Types
        struct FRAMENODE : public TSLinkedNode<FRAMENODE> {
            CLayoutFrame* frame;
            uint32_t dep;
        };

        // Static functions
        static void ResizePending();

        // Member variables
        TSLink<CLayoutFrame> resizeLink;
        CFramePoint* m_points[FRAMEPOINT_NUMPOINTS] = {};
        TSList<FRAMENODE, TSGetLink<FRAMENODE>> m_resizeList;
        struct {
            uint32_t left       : 1;
            uint32_t top        : 1;
            uint32_t right      : 1;
            uint32_t bottom     : 1;
            uint32_t centerX    : 1;
            uint32_t centerY    : 1;
        } m_guard;
        uint32_t m_resizeCounter : 8;
        uint32_t m_flags : 16;
        CRect m_rect;
        float m_width;
        float m_height;
        float m_layoutScale;
        float m_layoutDepth;

        // Virtual member functions
        virtual ~CLayoutFrame();
        virtual void LoadXML(XMLNode* node, CStatus* status);
        virtual CLayoutFrame* GetLayoutParent();
        virtual bool SetLayoutScale(float scale, bool force);
        virtual void SetWidth(float width);
        virtual void SetHeight(float height);
        virtual float GetWidth();
        virtual float GetHeight();
        virtual void GetClampRectInsets(float& a1, float& a2, float& a3, float& a4);
        virtual int32_t IsAttachmentOrigin();
        virtual CLayoutFrame* GetLayoutFrameByName(const char* name);
        virtual int32_t IsObjectLoaded();
        virtual void OnFrameSizeChanged(const CRect& rect);

        // Member functions
        CLayoutFrame();
        void AddToResizeList();
        float Bottom();
        int32_t CalculateRect(CRect* rect);
        bool CanBeAnchorFor(CLayoutFrame* frame);
        float CenterX();
        float CenterY();
        void ClearAllPoints();
        void DestroyLayout();
        void FreePoints();
        void GetFirstPointX(const FRAMEPOINT* const pointarray, int32_t elements, float& x);
        void GetFirstPointY(const FRAMEPOINT* const pointarray, int32_t elements, float& y);
        int32_t GetRect(CRect* rect);
        int32_t IsResizeDependency(CLayoutFrame* dependentFrame);
        uint32_t IsResizePending();
        float Left();
        int32_t OnFrameResize();
        void OnProtectedAttach(CLayoutFrame* frame);
        int32_t PtInFrameRect(const C2Vector& pt);
        void RegisterResize(CLayoutFrame* frame, uint32_t dep);
        void Resize(int32_t force);
        float Right();
        void SetAllPoints(CLayoutFrame* relative, int32_t doResize);
        void SetDeferredResize(int32_t enable);
        void SetPoint(FRAMEPOINT point, CLayoutFrame* relative, FRAMEPOINT relativePoint, float offsetX, float offsetY, int32_t doResize);
        void SetProtectFlag(uint32_t flag);
        int32_t Sub488DB0(const FRAMEPOINT* const pointarray, int32_t elements, float& x);
        int32_t Sub488E40(const FRAMEPOINT* const pointarray, int32_t elements, float& y);
        float Top();
        void UnflattenFrame(CLayoutFrame* frame);
        void UnregisterResize(CLayoutFrame* frame, uint32_t dep);
};

namespace LayoutFrame {
    // TODO put in better location
    extern STORM_EXPLICIT_LIST(CLayoutFrame, resizeLink) s_resizePendingList;
}

#endif
