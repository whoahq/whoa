#ifndef UI_C_SIMPLE_TOP_HPP
#define UI_C_SIMPLE_TOP_HPP

#include "event/Event.hpp"
#include "gx/Screen.hpp"
#include "ui/CFrameStrata.hpp"
#include "ui/CLayoutFrame.hpp"
#include "ui/CSimpleFrame.hpp"
#include "ui/FrameScript.hpp"
#include <common/Array.hpp>

enum MOVERESIZE_REASON {
    MOVERESIZE_REASON0 = 0,
    MOVERESIZE_REASON1 = 1,
    MOVERESIZE_REASON2 = 2
};

class CSimpleTop : public CLayoutFrame {
    public:
        // Types
        struct frame_layout {
            int32_t enabled = 0;
            CSimpleFrame* frame = nullptr;
            FRAMEPOINT anchor = FRAMEPOINT_TOPLEFT;
            C2Vector last;
            float float14 = 0.0f;
        };

        // Static variables
        static CSimpleTop* s_instance;
        static uint32_t m_eventTime;

        // Static functions
        static int32_t OnChar(const EVENT_DATA_CHAR*, void*);
        static int32_t OnDisplaySizeChanged(const EVENT_DATA_SIZE*, void*);
        static int32_t OnFocusChanged(const void*, void*);
        static int32_t OnIme(const void*, void*);
        static int32_t OnKeyDown(const EVENT_DATA_KEY*, void*);
        static int32_t OnKeyDownRepeat(const void*, void*);
        static int32_t OnKeyUp(const EVENT_DATA_KEY*, void*);
        static int32_t OnMouseDown(const EVENT_DATA_MOUSE*, void*);
        static int32_t OnMouseMove(const EVENT_DATA_MOUSE*, void*);
        static int32_t OnMouseMoveRelative(const EVENT_DATA_MOUSE*, void*);
        static int32_t OnMouseUp(const EVENT_DATA_MOUSE*, void*);
        static int32_t OnMouseWheel(const EVENT_DATA_MOUSE*, void*);
        static float RoundToPixelHeight(float);

        // Member variables
        HLAYER m_screenLayer;
        CSimpleFrame* m_mouseFocus = nullptr;
        CSimpleFrame* m_mouseCapture = nullptr;
        CSimpleFrame* m_keydownCapture[787] = {};
        STORM_EXPLICIT_LIST(CSimpleFrame, m_framesLink) m_frames;
        STORM_EXPLICIT_LIST(CSimpleFrame, m_destroyedLink) m_destroyed;
        CFrameStrata* m_strata[NUM_FRAME_STRATA];
        frame_layout m_layout;
        CSimpleSortedArray<FRAMEPRIORITY*> m_eventqueue[NUM_FRAME_STRATA][NUM_SIMPLE_EVENTS];
        int32_t m_checkFocus = 1;
        EVENT_DATA_MOUSE m_mousePosition;
        int32_t (*m_mouseButtonCallback)(CMouseEvent*) = nullptr;
        int32_t (*m_mousePositionCallback)(CMouseEvent*) = nullptr;
        int32_t (*m_displaySizeCallback)(const CSizeEvent&) = nullptr;

        // Member functions
        CSimpleTop();
        void CompressStrata(int32_t);
        void EnableEvents(void);
        void HideFrame(CSimpleFrame*, int32_t);
        void MoveOrResizeFrame(const CMouseEvent&);
        void NotifyFrameLayerChanged(CSimpleFrame*, uint32_t);
        void NotifyFrameMovedOrResized(CSimpleFrame*);
        void OnLayerUpdate(float);
        void OnLayerRender(void);
        int32_t RaiseFrame(CSimpleFrame*, int32_t);
        void RegisterForEvent(CSimpleFrame*, CSimpleEventType, int32_t, uint32_t);
        void RegisterFrame(CSimpleFrame*);
        void ShowFrame(CSimpleFrame*, int32_t);
        int32_t StartMoveOrResizeFrame(const CMouseEvent&, MOVERESIZE_REASON, int32_t);
        int32_t StartMoveOrResizeFrame(CSimpleFrame*, MOVERESIZE_REASON, float, float, int32_t);
        int32_t StartMoveOrResizeFrame(CSimpleFrame*, MOVERESIZE_REASON, float, float, FRAMEPOINT);
        void UnregisterForEvent(CSimpleFrame*, CSimpleEventType, int32_t);
};

#endif
