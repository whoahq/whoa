#ifndef UI_C_SIMPLE_SCROLL_FRAME_HPP
#define UI_C_SIMPLE_SCROLL_FRAME_HPP

#include "ui/CSimpleFrame.hpp"
#include <tempest/Vector.hpp>

class CRect;
class CRenderBatch;

class CSimpleScrollFrame : public CSimpleFrame {
    public:
        // Static variables
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable(void);
        static int32_t GetObjectType(void);
        static void RegisterScriptMethods(lua_State*);
        static void RenderScrollChild(void* param);

        // Member variables
        int32_t m_updateScrollChild = 0;
        CSimpleFrame* m_scrollChild = nullptr;
        C2Vector m_scrollRange;
        C2Vector m_scrollOffset;
        ScriptIx m_onHorizontalScroll;
        ScriptIx m_onVerticalScroll;
        ScriptIx m_onScrollRangeChanged;

        // Virtual member functions
        virtual ScriptIx* GetScriptByName(const char*, ScriptData&);
        virtual bool IsA(int32_t);
        virtual int32_t GetScriptMetaTable(void);
        virtual void LoadXML(XMLNode*, CStatus*);
        virtual int32_t GetBoundsRect(CRect& bounds);
        virtual void OnLayerUpdate(float elapsedSec);
        virtual void OnFrameRender(CRenderBatch* batch, uint32_t layer);
        virtual void OnFrameSizeChanged(float w, float h);

        // Member functions
        CSimpleScrollFrame(CSimpleFrame*);
        void RunOnScrollRangeChangedScript();
        void RunOnVerticalScrollScript();
        void SetScrollChild(CSimpleFrame*);
        void SetVerticalScroll(float offset);
        void UpdateScrollChild();
        void UpdateScrollChildRect(float w, float h);
};

#endif
