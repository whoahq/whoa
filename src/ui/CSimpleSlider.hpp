#ifndef UI_C_SIMPLE_SLIDER_HPP
#define UI_C_SIMPLE_SLIDER_HPP

#include "ui/CSimpleFrame.hpp"

class CSimpleSlider : public CSimpleFrame {
    public:
        // Static variables
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable();
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State* L);

        // Member variables
        int32_t m_changed : 1;
        int32_t m_rangeSet : 1;
        int32_t m_valueSet : 1;
        int32_t m_buttonDown : 1;
        float m_baseValue = 0.0f;
        float m_range = 0.0f;
        float m_value = 0.0f;
        float m_valueStep = 0.0f;
        CSimpleTexture* m_thumbTexture = nullptr;
        SLIDER_ORIENTATION m_orientation = SLIDER_VERTICAL;
        ScriptIx m_onValueChanged;
        ScriptIx m_onMinMaxChanged;

        // Virtual member functions
        virtual ScriptIx* GetScriptByName(const char* name, ScriptData& data);
        virtual bool IsA(int32_t type);
        virtual int32_t GetScriptMetaTable();
        virtual void LoadXML(XMLNode* node, CStatus* status);
        virtual void OnLayerUpdate(float elapsedSec);

        // Member functions
        CSimpleSlider(CSimpleFrame* parent)
            : CSimpleFrame(parent)
            , m_changed(0)
            , m_rangeSet(0)
            , m_valueSet(0)
            , m_buttonDown(0)
            {
                this->EnableEvent(SIMPLE_EVENT_MOUSE, -1);
            };
        void RunOnMinMaxChangedScript();
        void RunOnValueChangedScript();
        void SetMinMaxValues(float min, float max);
        void SetThumbTexture(CSimpleTexture* texture, int32_t drawLayer);
        void SetValue(float value);
        float Sub96BC10(float value);
};

#endif
