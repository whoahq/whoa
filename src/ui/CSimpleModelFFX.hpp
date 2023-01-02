#ifndef UI_C_SIMPLE_MODEL_FFX_HPP
#define UI_C_SIMPLE_MODEL_FFX_HPP

#include "ui/CSimpleModel.hpp"

class CSimpleFrame;

class CSimpleModelFFX : public CSimpleModel {
    public:
        // Static variables
        static int32_t s_metatable;

        // Static functions
        static CSimpleFrame* Create(CSimpleFrame*);
        static void CreateScriptMetaTable(void);
        static void RegisterScriptMethods(lua_State*);
        static void Render(void*);

        // Virtual member functions
        virtual int32_t GetScriptMetaTable(void);
        virtual void OnFrameRender(CRenderBatch*, uint32_t);

        // Member functions
        CSimpleModelFFX(CSimpleFrame*);
};

#endif
