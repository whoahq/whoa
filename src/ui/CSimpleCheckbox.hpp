#ifndef UI_C_SIMPLE_CHECKBOX_HPP
#define UI_C_SIMPLE_CHECKBOX_HPP

#include "ui/CSimpleButton.hpp"

class CSimpleCheckbox : public CSimpleButton {
    public:
        // Static variables
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable(void);
        static int32_t GetObjectType(void);
        static void RegisterScriptMethods(lua_State*);

        // Member variables

        // Virtual member functions
        virtual bool IsA(int32_t);
        virtual int32_t GetScriptMetaTable(void);

        // Member functions
        CSimpleCheckbox(CSimpleFrame*);
};

#endif
