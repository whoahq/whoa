#ifndef UI_SIMPLE_C_SIMPLE_STATUS_BAR_HPP
#define UI_SIMPLE_C_SIMPLE_STATUS_BAR_HPP

#include "ui/simple/CSimpleFrame.hpp"

class CSimpleStatusBar : public CSimpleFrame {
    public:
        // Static variables
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable();
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State* L);

        // Member variables
        // TODO

        // Virtual member functions
        virtual int32_t GetScriptMetaTable();

        // Member functions
        CSimpleStatusBar(CSimpleFrame* parent);
};

#endif
