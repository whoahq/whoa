#include "ui/game/CGQuestPOIFrame.hpp"
#include "ui/game/CGQuestPOIFrameScript.hpp"

int32_t CGQuestPOIFrame::s_metatable;
int32_t CGQuestPOIFrame::s_objectType;

CSimpleFrame* CGQuestPOIFrame::Create(CSimpleFrame* parent) {
    // TODO use CDataAllocator

    return STORM_NEW(CGQuestPOIFrame)(parent);
}

void CGQuestPOIFrame::CreateScriptMetaTable() {
    auto L = FrameScript_GetContext();
    CGQuestPOIFrame::s_metatable = FrameScript_Object::CreateScriptMetaTable(L, &CGQuestPOIFrame::RegisterScriptMethods);
}

int32_t CGQuestPOIFrame::GetObjectType() {
    if (!CGQuestPOIFrame::s_objectType) {
        CGQuestPOIFrame::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CGQuestPOIFrame::s_objectType;
}

void CGQuestPOIFrame::RegisterScriptMethods(lua_State* L) {
    CSimpleFrame::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, CGQuestPOIFrameMethods, NUM_CG_QUEST_POI_FRAME_SCRIPT_METHODS);
}

CGQuestPOIFrame::CGQuestPOIFrame(CSimpleFrame* parent) : CSimpleFrame(parent) {
    // TODO
}

int32_t CGQuestPOIFrame::GetScriptMetaTable() {
    return CGQuestPOIFrame::s_metatable;
}

