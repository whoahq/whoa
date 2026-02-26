#include "ui/game/CGCooldown.hpp"
#include "ui/game/CGCooldownScript.hpp"

int32_t CGCooldown::s_metatable;
int32_t CGCooldown::s_objectType;

CSimpleFrame* CGCooldown::Create(CSimpleFrame* parent) {
    // TODO use CDataAllocator

    return STORM_NEW(CGCooldown)(parent);
}

void CGCooldown::CreateScriptMetaTable() {
    auto L = FrameScript_GetContext();
    CGCooldown::s_metatable = FrameScript_Object::CreateScriptMetaTable(L, &CGCooldown::RegisterScriptMethods);
}

int32_t CGCooldown::GetObjectType() {
    if (!CGCooldown::s_objectType) {
        CGCooldown::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CGCooldown::s_objectType;
}

void CGCooldown::RegisterScriptMethods(lua_State* L) {
    CSimpleFrame::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, CGCooldownMethods, NUM_CG_COOLDOWN_SCRIPT_METHODS);
}

CGCooldown::CGCooldown(CSimpleFrame* parent) : CSimpleFrame(parent) {
    // TODO
}

int32_t CGCooldown::GetScriptMetaTable() {
    return CGCooldown::s_metatable;
}
