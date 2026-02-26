#include "ui/game/CGTooltip.hpp"
#include "ui/game/CGTooltipScript.hpp"

int32_t CGTooltip::s_metatable;
int32_t CGTooltip::s_objectType;

CSimpleFrame* CGTooltip::Create(CSimpleFrame* parent) {
    // TODO use CDataAllocator

    return STORM_NEW(CGTooltip)(parent);
}

void CGTooltip::CreateScriptMetaTable() {
    auto L = FrameScript_GetContext();
    CGTooltip::s_metatable = FrameScript_Object::CreateScriptMetaTable(L, &CGTooltip::RegisterScriptMethods);
}

int32_t CGTooltip::GetObjectType() {
    if (!CGTooltip::s_objectType) {
        CGTooltip::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CGTooltip::s_objectType;
}

void CGTooltip::RegisterScriptMethods(lua_State* L) {
    CSimpleFrame::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, CGTooltipMethods, NUM_CG_TOOLTIP_SCRIPT_METHODS);
}

CGTooltip::CGTooltip(CSimpleFrame* parent) : CSimpleFrame(parent) {
    // TODO
}

int32_t CGTooltip::GetScriptMetaTable() {
    return CGTooltip::s_metatable;
}

bool CGTooltip::IsA(int32_t type) {
    return type == CGTooltip::s_objectType
        || type == CSimpleFrame::s_objectType
        || type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}
