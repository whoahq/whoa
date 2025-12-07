#ifndef UI_SIMPLE_C_SIMPLE_MODEL_FFX_HPP
#define UI_SIMPLE_C_SIMPLE_MODEL_FFX_HPP

#include "ui/simple/CSimpleModel.hpp"

class CSimpleFrame;

class CSimpleModelFFX : public CSimpleModel {
    public:
        // Static variables
        static int32_t s_metatable;

        // Static functions
        static CSimpleFrame* Create(CSimpleFrame* parent);
        static void CreateScriptMetaTable();
        static void RegisterScriptMethods(lua_State* L);
        static void Render(void* arg);

        // Virtual member functions
        virtual int32_t GetScriptMetaTable();
        virtual void OnFrameRender(CRenderBatch* batch, uint32_t layer);

        // Member functions
        CSimpleModelFFX(CSimpleFrame* parent);
};

#endif
