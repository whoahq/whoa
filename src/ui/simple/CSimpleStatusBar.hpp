#ifndef UI_SIMPLE_C_SIMPLE_STATUS_BAR_HPP
#define UI_SIMPLE_C_SIMPLE_STATUS_BAR_HPP

#include "ui/simple/CSimpleFrame.hpp"

class CSimpleStatusBar : public CSimpleFrame {
    public:
        // Public static variables
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Public static functions
        static void CreateScriptMetaTable();
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State* L);

        // Public virtual member functions
        virtual int32_t GetScriptMetaTable();
        virtual ScriptIx* GetScriptByName(const char* name, ScriptData& data);
        virtual bool IsA(int32_t type);
        // TODO
        virtual void SetValue(float value);
        virtual void LoadXML(const XMLNode* node, CStatus* status);

        // Public member functions
        CSimpleStatusBar(CSimpleFrame* parent);
        float GetMaxValue() const;
        float GetMinValue() const;
        float GetValue() const;
        void RunOnMinMaxChangedScript();
        void RunOnValueChangedScript();
        void SetBarTexture(CSimpleTexture* texture, int32_t drawlayer);
        void SetMinMaxValues(float min, float max);
        void SetOrientation(ORIENTATION orientation);
        void SetRotatesTexture(int32_t enabled);
        void SetStatusBarColor(const CImVector& color);

    protected:
        // Protected member variables
        uint32_t m_changed  : 1;
        uint32_t m_rangeSet : 1;
        uint32_t m_valueSet : 1;
        float m_minValue = 0.0f;
        float m_maxValue = 0.0f;
        float m_value = 0.0f;
        CSimpleTexture* m_barTexture = nullptr;
        ORIENTATION m_orientation = ORIENTATION_HORIZONTAL;
        ScriptIx m_onValueChanged;
        ScriptIx m_onMinMaxChanged;
};

#endif
