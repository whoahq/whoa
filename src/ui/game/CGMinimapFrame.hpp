#ifndef UI_GAME_C_G_MINIMAP_FRAME_HPP
#define UI_GAME_C_G_MINIMAP_FRAME_HPP

#include "ui/simple/CSimpleFrame.hpp"

class CGMinimapFrame : public CSimpleFrame {
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
        // TODO

        // Virtual member functions
        virtual int32_t GetScriptMetaTable();

        // Member functions
        CGMinimapFrame(CSimpleFrame* parent);
};

#endif
