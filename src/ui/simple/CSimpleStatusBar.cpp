#include "ui/simple/CSimpleStatusBar.hpp"
#include "ui/simple/CSimpleStatusBarScript.hpp"
#include "util/Lua.hpp"

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

FrameScript_Object::ScriptIx* CSimpleStatusBar::GetScriptByName(const char* name, ScriptData& data) {
    auto script = this->CSimpleFrame::GetScriptByName(name, data);

    if (script) {
        return script;
    }

    if (!SStrCmpI(name, "OnValueChanged")) {
        script = &this->m_onValueChanged;
        data.wrapper = "return function(self,value) %s end";
    } else if (!SStrCmpI(name, "OnMinMaxChanged")) {
        script = &this->m_onMinMaxChanged;
        data.wrapper = "return function(self,min,max) %s end";
    }

    return script;
}

int32_t CSimpleStatusBar::GetScriptMetaTable() {
    return CSimpleStatusBar::s_metatable;
}

void CSimpleStatusBar::RunOnValueChangedScript() {
    if (!this->m_onValueChanged.luaRef) {
        return;
    }

    auto L = FrameScript_GetContext();

    lua_pushnumber(L, this->m_value);

    this->RunScript(this->m_onValueChanged, 1, nullptr);
}

void CSimpleStatusBar::SetValue(float value) {
    if (!this->m_rangeSet) {
        return;
    }

    // Clamp value
    value = std::min(std::max(value, this->m_minValue), this->m_maxValue);

    if (this->m_valueSet && this->m_value == value) {
        return;
    }

    this->m_value = value;

    this->m_changed = true;
    this->m_valueSet = true;

    this->RunOnValueChangedScript();
}
