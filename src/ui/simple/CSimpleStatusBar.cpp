#include "ui/simple/CSimpleStatusBar.hpp"
#include "ui/simple/CSimpleStatusBarScript.hpp"

int32_t CSimpleStatusBar::s_metatable;
int32_t CSimpleStatusBar::s_objectType;

void CSimpleStatusBar::CreateScriptMetaTable() {
    auto L = FrameScript_GetContext();
    CSimpleStatusBar::s_metatable = FrameScript_Object::CreateScriptMetaTable(L, &CSimpleStatusBar::RegisterScriptMethods);
}

int32_t CSimpleStatusBar::GetObjectType() {
    if (!CSimpleStatusBar::s_objectType) {
        CSimpleStatusBar::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CSimpleStatusBar::s_objectType;
}

void CSimpleStatusBar::RegisterScriptMethods(lua_State* L) {
    CSimpleFrame::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, SimpleStatusBarMethods, NUM_SIMPLE_STATUS_BAR_SCRIPT_METHODS);
}

CSimpleStatusBar::CSimpleStatusBar(CSimpleFrame* parent) : CSimpleFrame(parent) {
    // TODO
}

int32_t CSimpleStatusBar::GetScriptMetaTable() {
    return CSimpleStatusBar::s_metatable;
}
