#include "ui/game/CGCharacterModelBase.hpp"
#include "ui/game/CGCharacterModelBaseScript.hpp"

int32_t CGCharacterModelBase::s_metatable;
int32_t CGCharacterModelBase::s_objectType;

CSimpleFrame* CGCharacterModelBase::Create(CSimpleFrame* parent) {
    // TODO use CDataAllocator

    return STORM_NEW(CGCharacterModelBase)(parent);
}

void CGCharacterModelBase::CreateScriptMetaTable() {
    auto L = FrameScript_GetContext();
    CGCharacterModelBase::s_metatable = FrameScript_Object::CreateScriptMetaTable(L, &CGCharacterModelBase::RegisterScriptMethods);
}

int32_t CGCharacterModelBase::GetObjectType() {
    if (!CGCharacterModelBase::s_objectType) {
        CGCharacterModelBase::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CGCharacterModelBase::s_objectType;
}

void CGCharacterModelBase::RegisterScriptMethods(lua_State* L) {
    CSimpleModel::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, CGCharacterModelBaseMethods, NUM_CG_CHARACTER_MODEL_BASE_SCRIPT_METHODS);
}

CGCharacterModelBase::CGCharacterModelBase(CSimpleFrame* parent) : CSimpleModel(parent) {
    // TODO
}

int32_t CGCharacterModelBase::GetScriptMetaTable() {
    return CGCharacterModelBase::s_metatable;
}
