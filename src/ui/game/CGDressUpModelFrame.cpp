#include "ui/game/CGDressUpModelFrame.hpp"
#include "ui/game/CGDressUpModelFrameScript.hpp"

int32_t CGDressUpModelFrame::s_metatable;
int32_t CGDressUpModelFrame::s_objectType;

CSimpleFrame* CGDressUpModelFrame::Create(CSimpleFrame* parent) {
    // TODO use CDataAllocator

    return STORM_NEW(CGDressUpModelFrame)(parent);
}

void CGDressUpModelFrame::CreateScriptMetaTable() {
    auto L = FrameScript_GetContext();
    CGDressUpModelFrame::s_metatable = FrameScript_Object::CreateScriptMetaTable(L, &CGDressUpModelFrame::RegisterScriptMethods);
}

int32_t CGDressUpModelFrame::GetObjectType() {
    if (!CGDressUpModelFrame::s_objectType) {
        CGDressUpModelFrame::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CGDressUpModelFrame::s_objectType;
}

void CGDressUpModelFrame::RegisterScriptMethods(lua_State* L) {
    CGCharacterModelBase::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, CGDressUpModelFrameMethods, NUM_CG_DRESS_UP_MODEL_FRAME_SCRIPT_METHODS);
}

CGDressUpModelFrame::CGDressUpModelFrame(CSimpleFrame* parent) : CGCharacterModelBase(parent) {
    // TODO
}

int32_t CGDressUpModelFrame::GetScriptMetaTable() {
    return CGDressUpModelFrame::s_metatable;
}
