#ifndef UI_GAME_C_G_TOOLTIP_HPP
#define UI_GAME_C_G_TOOLTIP_HPP

#include "ui/simple/CSimpleFrame.hpp"

class CGTooltip : public CSimpleFrame {
    public:
        // Static variables
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static CSimpleFrame* Create(CSimpleFrame* parent);
        static void CreateScriptMetaTable();
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State* L);

        // Member variables
        CSimpleFrame* m_owner = nullptr;
        TOOLTIP_ANCHORPOINT m_anchorPoint;
        // TODO
        C2Vector m_offset;
        // TODO

        // Virtual member functions
        virtual bool IsA(int32_t type);
        virtual int32_t GetScriptMetaTable();

        // Member functions
        CGTooltip(CSimpleFrame* parent);
};

#endif
