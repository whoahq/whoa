#ifndef UI_SIMPLE_C_SIMPLE_TOP_HPP
#define UI_SIMPLE_C_SIMPLE_TOP_HPP

#include "event/Event.hpp"
#include "gx/Screen.hpp"
#include "ui/CFrameStrata.hpp"
#include "ui/CLayoutFrame.hpp"
#include "ui/FrameScript.hpp"
#include <common/Array.hpp>

class CSimpleFrame;

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
        static int32_t OnChar(const EVENT_DATA_CHAR* pCharData, void* param);
        static int32_t OnDisplaySizeChanged(const EVENT_DATA_SIZE* pSizeData, void* param);
        static int32_t OnFocusChanged(const void* a1, void* a2);
        static int32_t OnIme(const void* a1, void* a2);
        static int32_t OnKeyDown(const EVENT_DATA_KEY* pKeyData, void* param);
        static int32_t OnKeyDownRepeat(const EVENT_DATA_KEY* pKeyData, void* param);
        static int32_t OnKeyUp(const EVENT_DATA_KEY* pKeyData, void* param);
        static int32_t OnMouseDown(const EVENT_DATA_MOUSE* pMouseData, void* param);
        static int32_t OnMouseMove(const EVENT_DATA_MOUSE* pMouseData, void* param);
        static int32_t OnMouseMoveRelative(const EVENT_DATA_MOUSE* pMouseData, void* param);
        static int32_t OnMouseUp(const EVENT_DATA_MOUSE* pMouseData, void* param);
        static int32_t OnMouseWheel(const EVENT_DATA_MOUSE* pMouseData, void* param);
        static float RoundToPixelHeight(float ddcHeight);

        // Member variables
        HLAYER m_screenLayer;
        CSimpleFrame* m_mouseFocus = nullptr;
        CSimpleFrame* m_mouseCapture = nullptr;
        CSimpleFrame* m_keydownCapture[787] = {};
        STORM_EXPLICIT_LIST(CSimpleFrame, m_framesLink) m_frames;
        STORM_EXPLICIT_LIST(CSimpleFrame, m_destroyedLink) m_destroyed;
        CFrameStrata* m_strata[NUM_FRAME_STRATA];
        frame_layout m_layout;
        CSimpleSortedArray<FRAMEPRIORITY*> m_eventqueue[NUM_SIMPLE_EVENTS][NUM_FRAME_STRATA];
        int32_t m_checkFocus = 1;
        EVENT_DATA_MOUSE m_mousePosition;
        int32_t (*m_mouseButtonCallback)(CMouseEvent*) = nullptr;
        int32_t (*m_mousePositionCallback)(CMouseEvent*) = nullptr;
        int32_t (*m_displaySizeCallback)(const CSizeEvent&) = nullptr;

        // Virtual member functions
        virtual ~CSimpleTop();

        // Member functions
        CSimpleTop();
        void CompressStrata(int32_t strata);
        void DisableEvents();
        void EnableEvents();
        void HideFrame(CSimpleFrame* frame, int32_t a4);
        void MoveOrResizeFrame(const CMouseEvent& evt);
        void NotifyFrameLayerChanged(CSimpleFrame* frame, uint32_t layer);
        void NotifyFrameMovedOrResized(CSimpleFrame* frame);
        void OnLayerUpdate(float elapsedSec);
        void OnLayerRender();
        int32_t RaiseFrame(CSimpleFrame* frame, int32_t checkOcclusion);
        void RegisterForEvent(CSimpleFrame* frame, CSimpleEventType event, int32_t a4, uint32_t priority);
        void RegisterFrame(CSimpleFrame* frame);
        void ShowFrame(CSimpleFrame* frame, int32_t a3);
        int32_t StartMoveOrResizeFrame(const CMouseEvent& start, MOVERESIZE_REASON reason, int32_t resize);
        int32_t StartMoveOrResizeFrame(CSimpleFrame* frame, MOVERESIZE_REASON reason, float startx, float starty, int32_t a6);
        int32_t StartMoveOrResizeFrame(CSimpleFrame* frame, MOVERESIZE_REASON reason, float startx, float starty, FRAMEPOINT a6);
        void UnregisterForEvent(CSimpleFrame* frame, CSimpleEventType event, int32_t a4);
        void UnregisterFrame(CSimpleFrame* frame);
};

#endif
