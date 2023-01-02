#ifndef UI_C_SIMPLE_FRAME_HPP
#define UI_C_SIMPLE_FRAME_HPP

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
        static void CreateScriptMetaTable(void);
        static int32_t GetObjectType(void);
        static void RegisterScriptMethods(lua_State*);

        // Member variables
        CSimpleTop* m_top = NULL;
        CSimpleTitleRegion* m_titleRegion = NULL;
        int32_t m_intAC = 1;
        int32_t m_id = 0;
        uint32_t m_flags = 0;
        float m_frameScale = 1.0f;
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
        virtual ScriptIx* GetScriptByName(const char*, ScriptData&);
        virtual bool IsA(int32_t);
        virtual int32_t GetScriptMetaTable(void);
        virtual void LoadXML(XMLNode*, CStatus*);
        virtual void PreOnAnimUpdate(void);
        virtual void OnLayerShow(void);
        virtual void OnLayerHide(void);
        virtual void OnLayerUpdate(float);
        virtual int32_t OnLayerTrackUpdate(const CMouseEvent&);
        virtual void OnFrameRender(void);
        virtual void OnFrameRender(CRenderBatch*, uint32_t);
        virtual void OnScreenSizeChanged();
        virtual void OnFrameSizeChanged(float, float);
        virtual void OnLayerCursorEnter(int32_t);
        virtual void OnLayerCursorExit(int32_t, int32_t);
        virtual int32_t OnLayerChar(const CCharEvent&);
        virtual int32_t OnLayerKeyDown(const CKeyEvent&);
        virtual int32_t OnLayerKeyUp(const CKeyEvent&);
        virtual int32_t OnLayerMouseDown(const CMouseEvent&, const char*);
        virtual int32_t OnLayerMouseUp(const CMouseEvent&, const char*);
        virtual void PostLoadXML(XMLNode*, CStatus*);
        virtual void UnregisterRegion(CSimpleRegion*);
        virtual int32_t GetBoundsRect(CRect& bounds);
        virtual void PreLoadXML(XMLNode*, CStatus*);
        virtual void LockHighlight(int32_t);
        virtual int32_t HideThis(void);
        virtual int32_t ShowThis(void);
        virtual bool UpdateScale(bool);
        virtual void UpdateDepth(bool);
        virtual void ParentFrame(CSimpleFrame*);
        virtual void OnFrameSizeChanged(const CRect&);

        // Member functions
        CSimpleFrame(CSimpleFrame*);
        void AddFrameRegion(CSimpleRegion*, uint32_t);
        void DisableDrawLayer(uint32_t);
        void EnableDrawLayer(uint32_t);
        void EnableEvent(CSimpleEventType, int32_t);
        int32_t GetHitRect(CRect& rect);
        void Hide(void);
        void LoadXML_Attributes(XMLNode*, CStatus*);
        void LoadXML_Backdrop(XMLNode*, CStatus*);
        void LoadXML_Layers(XMLNode*, CStatus*);
        void LoadXML_Scripts(XMLNode*, CStatus*);
        void NotifyDrawLayerChanged(uint32_t);
        void NotifyScrollParent(void);
        void PostLoadXML_Frames(XMLNode*, CStatus*);
        void Raise(void);
        void RegisterForEvents(int32_t);
        void RegisterRegion(CSimpleRegion*);
        void RemoveFrameRegion(CSimpleRegion*, uint32_t);
        void RunOnCharScript(const char*);
        void RunOnEnterScript(int32_t);
        void RunOnHideScript(void);
        void RunOnKeyDownScript(const char*);
        void RunOnKeyUpScript(const char*);
        void RunOnLeaveScript(int32_t);
        void RunOnLoadScript(void);
        void RunOnMouseDownScript(const char*);
        void RunOnMouseUpScript(const char*);
        void RunOnShowScript(void);
        void RunOnSizeChangedScript(float, float);
        void RunOnUpdateScript(float);
        void SetBackdrop(CBackdropGenerator*);
        void SetBeingScrolled(int32_t, int32_t);
        void SetFrameAlpha(uint8_t alpha);
        void SetFrameFlag(int32_t, int32_t);
        void SetFrameLevel(int32_t, int32_t);
        bool SetFrameScale(float scale, bool a3);
        void SetFrameStrata(FRAME_STRATA);
        void SetHitRect(void);
        void SetParent(CSimpleFrame*);
        void Show(void);
        int32_t TestHitRect(const C2Vector&);
        void UnregisterForEvents(int32_t);
};

#endif
