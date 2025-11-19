#include "ui/CSimpleCheckbox.hpp"
#include "ui/CSimpleCheckboxScript.hpp"

int32_t CSimpleCheckbox::s_metatable;
int32_t CSimpleCheckbox::s_objectType;

void CSimpleCheckbox::CreateScriptMetaTable() {
    lua_State* L = FrameScript_GetContext();
    int32_t ref = FrameScript_Object::CreateScriptMetaTable(L, &CSimpleCheckbox::RegisterScriptMethods);
    CSimpleCheckbox::s_metatable = ref;
}

int32_t CSimpleCheckbox::GetObjectType() {
    if (!CSimpleCheckbox::s_objectType) {
        CSimpleCheckbox::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CSimpleCheckbox::s_objectType;
}

void CSimpleCheckbox::RegisterScriptMethods(lua_State* L) {
    CSimpleButton::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, SimpleCheckboxMethods, NUM_SIMPLE_CHECKBOX_SCRIPT_METHODS);
}

int32_t CSimpleCheckbox::GetScriptMetaTable() {
    return CSimpleCheckbox::s_metatable;
}

bool CSimpleCheckbox::IsA(int32_t type) {
    return type == CSimpleCheckbox::s_objectType
        || type == CSimpleButton::s_objectType
        || type == CSimpleFrame::s_objectType
        || type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}
