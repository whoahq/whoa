#include "ui/game/CGTabardModelFrame.hpp"
#include "ui/game/CGTabardModelFrameScript.hpp"

int32_t CGTabardModelFrame::s_metatable;
int32_t CGTabardModelFrame::s_objectType;

CSimpleFrame* CGTabardModelFrame::Create(CSimpleFrame* parent) {
    // TODO use CDataAllocator

    return STORM_NEW(CGTabardModelFrame)(parent);
}

void CGTabardModelFrame::CreateScriptMetaTable() {
    auto L = FrameScript_GetContext();
    CGTabardModelFrame::s_metatable = FrameScript_Object::CreateScriptMetaTable(L, &CGTabardModelFrame::RegisterScriptMethods);
}

int32_t CGTabardModelFrame::GetObjectType() {
    if (!CGTabardModelFrame::s_objectType) {
        CGTabardModelFrame::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CGTabardModelFrame::s_objectType;
}

void CGTabardModelFrame::RegisterScriptMethods(lua_State* L) {
    CGCharacterModelBase::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, CGTabardModelFrameMethods, NUM_CG_TABARD_MODEL_FRAME_SCRIPT_METHODS);
}

CGTabardModelFrame::CGTabardModelFrame(CSimpleFrame* parent) : CGCharacterModelBase(parent) {
    // TODO
}

int32_t CGTabardModelFrame::GetScriptMetaTable() {
    return CGTabardModelFrame::s_metatable;
}
