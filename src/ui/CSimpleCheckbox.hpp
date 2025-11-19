#ifndef UI_C_SIMPLE_CHECKBOX_HPP
#define UI_C_SIMPLE_CHECKBOX_HPP

#include "ui/CSimpleButton.hpp"

class CSimpleCheckbox : public CSimpleButton {
    public:
        // Static variables
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable();
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State* L);

        // Member variables
        int32_t m_checked = 0;
        CSimpleTexture* m_checkedTexture = nullptr;
        CSimpleTexture* m_disabledTexture = nullptr;

        // Virtual member functions
        virtual bool IsA(int32_t type);
        virtual int32_t GetScriptMetaTable();
        virtual void LoadXML(XMLNode* node, CStatus* status);

        // Member functions
        CSimpleCheckbox(CSimpleFrame* parent)
            : CSimpleButton(parent) {};
        int32_t GetChecked();
        void SetChecked(int32_t checked, int32_t force);
        void SetCheckedTexture(CSimpleTexture* texture);
        void SetDisabledCheckedTexture(CSimpleTexture* texture);
};

#endif
