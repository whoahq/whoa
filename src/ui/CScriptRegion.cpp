#include "ui/CScriptRegion.hpp"
#include "ui/CScriptObject.hpp"
#include "ui/CScriptRegionScript.hpp"
#include "ui/CSimpleTop.hpp"
#include "util/Lua.hpp"
#include <common/XML.hpp>

int32_t CScriptRegion::s_objectType;
const char* CScriptRegion::s_objectTypeName = "Region";

int32_t CScriptRegion::GetObjectType() {
    if (!CScriptRegion::s_objectType) {
        CScriptRegion::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CScriptRegion::s_objectType;
}

void CScriptRegion::RegisterScriptMethods(lua_State* L) {
    CScriptObject::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, ScriptRegionMethods, NUM_SCRIPT_REGION_SCRIPT_METHODS);
}

// TODO verify return type
CLayoutFrame* CScriptRegion::GetLayoutParent() {
    if (!this->m_parent || this->m_parent->m_layoutScale == 0.0f) {
        return CSimpleTop::s_instance;
    } else {
        return (CLayoutFrame*)this->m_parent;
    }
}

CLayoutFrame* CScriptRegion::GetLayoutFrameByName(const char* name) {
    char fullName[1024];
    this->CreateName(name, fullName, 1024);

    int32_t type = CScriptRegion::GetObjectType();
    CScriptRegion* object = static_cast<CScriptRegion*>(this->GetScriptObjectByName(fullName, type));

    return static_cast<CLayoutFrame*>(object);
}

const char* CScriptRegion::GetObjectTypeName() {
    return CScriptRegion::s_objectTypeName;
}

// TODO verify return type
CScriptObject* CScriptRegion::GetScriptObjectParent() {
    return (CScriptObject*)(this->m_parent);
}

bool CScriptRegion::IsA(int32_t type) {
    return type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}

bool CScriptRegion::IsA(const char* typeName) {
    return !SStrCmpI(typeName, CScriptRegion::s_objectTypeName, 0x7FFFFFFF)
        || !SStrCmpI(typeName, CScriptObject::s_objectTypeName, 0x7FFFFFFF);
}

bool CScriptRegion::IsDragging() {
    // TODO
    return false;
}

void CScriptRegion::LoadXML(XMLNode* node, CStatus* status) {
    CLayoutFrame::LoadXML(node, status);

    const char* parentKey = node->GetAttributeByName("parentKey");

    if (parentKey && *parentKey) {
        lua_State* L = FrameScript_GetContext();

        CScriptObject* parent = this->GetScriptObjectParent();

        if (parent) {
            if (!parent->lua_registered) {
                parent->RegisterScriptObject(0);
            }

            lua_rawgeti(L, LUA_REGISTRYINDEX, parent->lua_objectRef);
            lua_pushstring(L, parentKey);

            if (!this->lua_registered) {
                this->RegisterScriptObject(0);
            }

            lua_rawgeti(L, LUA_REGISTRYINDEX, this->lua_objectRef);
            lua_settable(L, -3);
            lua_settop(L, -2);
        }
    }

    this->LoadXML_Animations(node, status);
}

void CScriptRegion::LoadXML_Animations(XMLNode* node, CStatus* status) {
    // TODO
}

void CScriptRegion::NotifyAnimBegin(CSimpleAnimGroup* animGroup) {
    // TODO
}

void CScriptRegion::NotifyAnimEnd(CSimpleAnimGroup* animGroup) {
    // TODO
}

void CScriptRegion::OnLayerUpdate(float elapsedSec) {
    // TODO
}

bool CScriptRegion::ProtectedFunctionsAllowed() {
    // TODO

    return true;
}

void CScriptRegion::StopAnimating() {
    // TODO
}
