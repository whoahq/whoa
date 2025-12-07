#ifndef UI_SIMPLE_C_SIMPLE_SCROLL_FRAME_HPP
#define UI_SIMPLE_C_SIMPLE_SCROLL_FRAME_HPP

#include "ui/simple/CSimpleFrame.hpp"
#include <tempest/Vector.hpp>

class CRect;
class CRenderBatch;

class CSimpleScrollFrame : public CSimpleFrame {
    public:
        // Static variables
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable();
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State* L);
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
        virtual ScriptIx* GetScriptByName(const char* name, ScriptData& data);
        virtual bool IsA(int32_t type);
        virtual int32_t GetScriptMetaTable();
        virtual void LoadXML(XMLNode* node, CStatus* status);
        virtual int32_t GetBoundsRect(CRect& bounds);
        virtual void OnLayerUpdate(float elapsedSec);
        virtual void OnFrameRender(CRenderBatch* batch, uint32_t layer);
        virtual void OnFrameSizeChanged(float w, float h);

        // Member functions
        CSimpleScrollFrame(CSimpleFrame* parent);
        void RunOnScrollRangeChangedScript();
        void RunOnVerticalScrollScript();
        void SetScrollChild(CSimpleFrame* frame);
        void SetVerticalScroll(float offset);
        void UpdateScrollChild();
        void UpdateScrollChildRect(float w, float h);
};

#endif
