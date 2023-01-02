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
        static void ResizePending(void);

        // Member variables
        TSLink<CLayoutFrame> resizeLink;
        CFramePoint* m_points[FRAMEPOINT_NUMPOINTS] = {};
        TSList<FRAMENODE, TSGetLink<FRAMENODE>> m_resizeList;
        struct {
            int32_t left : 1;
            int32_t top : 1;
            int32_t right : 1;
            int32_t bottom : 1;
            int32_t centerX : 1;
            int32_t centerY : 1;
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
        virtual CLayoutFrame* GetLayoutParent(void);
        virtual bool SetLayoutScale(float, bool);
        virtual void SetWidth(float);
        virtual void SetHeight(float);
        virtual float GetWidth(void);
        virtual float GetHeight(void);
        virtual void GetClampRectInsets(float&, float&, float&, float&);
        virtual int32_t IsAttachmentOrigin(void);
        virtual CLayoutFrame* GetLayoutFrameByName(const char*);
        virtual int32_t IsObjectLoaded(void);
        virtual void OnFrameSizeChanged(const CRect&);

        // Member functions
        CLayoutFrame();
        void AddToResizeList(void);
        float Bottom(void);
        int32_t CalculateRect(CRect*);
        bool CanBeAnchorFor(CLayoutFrame*);
        float CenterX(void);
        float CenterY(void);
        void ClearAllPoints(void);
        void DestroyLayout();
        void FreePoints(void);
        void GetFirstPointX(const FRAMEPOINT* const, int32_t, float&);
        void GetFirstPointY(const FRAMEPOINT* const, int32_t, float&);
        int32_t GetRect(CRect*);
        int32_t IsResizeDependency(CLayoutFrame*);
        uint32_t IsResizePending(void);
        float Left(void);
        void LoadXML(XMLNode*, CStatus*);
        int32_t OnFrameResize(void);
        void OnProtectedAttach(CLayoutFrame*);
        int32_t PtInFrameRect(const C2Vector&);
        void RegisterResize(CLayoutFrame*, uint32_t);
        void Resize(int32_t);
        float Right(void);
        void SetAllPoints(CLayoutFrame*, int32_t);
        void SetDeferredResize(int32_t);
        void SetPoint(FRAMEPOINT, CLayoutFrame*, FRAMEPOINT, float, float, int32_t);
        void SetProtectFlag(uint32_t);
        int32_t Sub488DB0(const FRAMEPOINT* const, int32_t, float&);
        int32_t Sub488E40(const FRAMEPOINT* const, int32_t, float&);
        float Top(void);
        void UnflattenFrame(CLayoutFrame*);
        void UnregisterResize(CLayoutFrame*, uint32_t);
};

namespace LayoutFrame {
    // TODO put in better location
    extern STORM_EXPLICIT_LIST(CLayoutFrame, resizeLink) s_resizePendingList;
}

#endif
