#ifndef UI_C_SIMPLE_FRAME_HPP
#define UI_C_SIMPLE_FRAME_HPP

#include "event/CEvent.hpp"
#include "ui/CRenderBatch.hpp"
#include "ui/CScriptRegion.hpp"
#include "ui/CSimpleRegion.hpp"
#include "ui/Types.hpp"
#include <cstdint>
#include <storm/List.hpp>

class CBackdropGenerator;
class CCharEvent;
class CKeyEvent;
class CMouseEvent;
class CSimpleTitleRegion;
class CSimpleTop;
struct lua_State;

class CSimpleFrame : public CScriptRegion {
    public:
        // Static members
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable();
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State* L);

        // Member variables
        CSimpleTop* m_top = nullptr;
        CSimpleTitleRegion* m_titleRegion = nullptr;
        int32_t m_intAC = 1;
        int32_t m_id = 0;
        uint32_t m_flags = 0;
        float m_frameScale = 1.0f;
        uint8_t m_alpha = 255;
        uint8_t alphaBD = 255;
        float m_depth = 0.0;
        FRAME_STRATA m_strata = FRAME_STRATA_MEDIUM;
        int32_t m_level = 0;
        uint32_t m_eventmask = 0;
        int32_t m_shown = 0;
        int32_t m_visible = 0;
        CRect m_hitRect = {};
        CRect m_hitOffset = {};
        int32_t m_highlightLocked = 0;
        uint32_t m_lookForDrag = 0;
        int32_t m_mouseDown = 0;
        int32_t m_dragging = 0;
        int32_t m_dragButton;
        C2Vector m_clickPoint;
        int32_t m_loading = 0;
        ScriptIx m_onLoad;
        ScriptIx m_onSizeChanged;
        ScriptIx m_onUpdate;
        ScriptIx m_onShow;
        ScriptIx m_onHide;
        ScriptIx m_onEnter;
        ScriptIx m_onLeave;
        ScriptIx m_onMouseDown;
        ScriptIx m_onMouseUp;
        ScriptIx m_onMouseWheel;
        ScriptIx m_onDragStart;
        ScriptIx m_onDragStop;
        ScriptIx m_onReceiveDrag;
        ScriptIx m_onChar;
        ScriptIx m_onKeyDown;
        ScriptIx m_onKeyUp;
        ScriptIx m_onAttributeChange;
        ScriptIx m_onEnable;
        ScriptIx m_onDisable;
        int32_t m_drawenabled[NUM_SIMPLEFRAME_DRAWLAYERS];
        CBackdropGenerator* m_backdrop = nullptr;
        STORM_EXPLICIT_LIST(CSimpleRegion, m_regionLink) m_regions;
        STORM_EXPLICIT_LIST(CSimpleRegion, m_layerLink) m_drawlayers[NUM_SIMPLEFRAME_DRAWLAYERS];
        uint32_t m_batchDirty = 0;
        CRenderBatch* m_batch[NUM_SIMPLEFRAME_DRAWLAYERS] = {};
        STORM_EXPLICIT_LIST(CRenderBatch, renderLink) m_renderList;
        TSList<SIMPLEFRAMENODE, TSGetLink<SIMPLEFRAMENODE>> m_children;
        TSLink<CSimpleFrame> m_framesLink;
        TSLink<CSimpleFrame> m_destroyedLink;
        TSLink<CSimpleFrame> m_strataLink;

        // Virtual member functions
        virtual ~CSimpleFrame();
        virtual ScriptIx* GetScriptByName(const char* name, ScriptData& data);
        virtual bool IsA(int32_t type);
        virtual int32_t GetScriptMetaTable();
        virtual void LoadXML(XMLNode* node, CStatus* status);
        virtual void PreOnAnimUpdate();
        virtual void OnLayerShow();
        virtual void OnLayerHide();
        virtual void OnLayerUpdate(float elapsedSec);
        virtual int32_t OnLayerTrackUpdate(const CMouseEvent& evt);
        virtual void OnFrameRender();
        virtual void OnFrameRender(CRenderBatch* batch, uint32_t layer);
        virtual void OnScreenSizeChanged();
        virtual void OnFrameSizeChanged(float width, float height);
        virtual void OnLayerCursorEnter(int32_t a2);
        virtual void OnLayerCursorExit(int32_t a2, int32_t a3);
        virtual int32_t OnLayerKeyDownRepeat(const CKeyEvent& evt);
        virtual int32_t OnLayerChar(const CCharEvent& evt);
        virtual int32_t OnLayerKeyDown(const CKeyEvent& evt);
        virtual int32_t OnLayerKeyUp(const CKeyEvent& evt);
        virtual int32_t OnLayerMouseDown(const CMouseEvent& evt, const char* btn);
        virtual int32_t OnLayerMouseUp(const CMouseEvent& evt, const char* btn);
        virtual void PostLoadXML(XMLNode* node, CStatus* status);
        virtual void UnregisterRegion(CSimpleRegion* region);
        virtual int32_t GetBoundsRect(CRect& bounds);
        virtual void PreLoadXML(XMLNode* node, CStatus* status);
        virtual void LockHighlight(int32_t lock);
        virtual int32_t HideThis();
        virtual int32_t ShowThis();
        virtual bool UpdateScale(bool a2);
        virtual void UpdateAlpha();
        virtual void UpdateDepth(bool a2);
        virtual void ParentFrame(CSimpleFrame* frame);
        virtual void OnFrameSizeChanged(const CRect& rect);

        // Member functions
        CSimpleFrame(CSimpleFrame* parent);
        void AddFrameRegion(CSimpleRegion* region, uint32_t drawlayer);
        void DisableDrawLayer(uint32_t drawlayer);
        void EnableDrawLayer(uint32_t drawlayer);
        void EnableEvent(CSimpleEventType eventType, int32_t priority);
        int32_t GetHitRect(CRect& rect);
        void Hide();
        void LoadXML_Attributes(XMLNode* node, CStatus* status);
        void LoadXML_Backdrop(XMLNode* node, CStatus* status);
        void LoadXML_Layers(XMLNode* node, CStatus* status);
        void LoadXML_Scripts(XMLNode* node, CStatus* status);
        void NotifyDrawLayerChanged(uint32_t drawlayer);
        void NotifyScrollParent();
        void PostLoadXML_Frames(XMLNode* node, CStatus* status);
        void Raise();
        void RegisterForEvents(int32_t a2);
        void RegisterRegion(CSimpleRegion* region);
        void RemoveFrameRegion(CSimpleRegion* region, uint32_t drawlayer);
        void RunOnCharScript(const char* chr);
        void RunOnEnterScript(int32_t a2);
        void RunOnHideScript();
        void RunOnKeyDownScript(const char* key);
        void RunOnKeyUpScript(const char* key);
        void RunOnLeaveScript(int32_t a2);
        void RunOnLoadScript();
        void RunOnMouseDownScript(const char* btn);
        void RunOnMouseUpScript(const char* btn);
        void RunOnShowScript();
        void RunOnSizeChangedScript(float width, float height);
        void RunOnUpdateScript(float elapsedSec);
        void SetBackdrop(CBackdropGenerator* backdrop);
        void SetBeingScrolled(int32_t a2, int32_t a3);
        void SetFrameAlpha(uint8_t alpha);
        void SetFrameFlag(int32_t flag, int32_t on);
        void SetFrameLevel(int32_t level, int32_t shiftChildren);
        bool SetFrameScale(float scale, bool a3);
        void SetFrameStrata(FRAME_STRATA strata);
        void SetHitRect();
        void SetParent(CSimpleFrame* parent);
        void Show();
        int32_t TestHitRect(const C2Vector& pt);
        void UnregisterForEvents(int32_t a2);
};

#endif
