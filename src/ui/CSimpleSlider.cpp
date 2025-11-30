#include "ui/CSimpleSlider.hpp"
#include "math/Utils.hpp"
#include "ui/CSimpleSliderScript.hpp"
#include "ui/CSimpleTexture.hpp"
#include "ui/LoadXML.hpp"
#include "util/Lua.hpp"
#include "util/StringTo.hpp"
#include <common/XML.hpp>

int32_t CSimpleSlider::s_metatable;
int32_t CSimpleSlider::s_objectType;

void CSimpleSlider::CreateScriptMetaTable() {
    lua_State* L = FrameScript_GetContext();
    int32_t ref = FrameScript_Object::CreateScriptMetaTable(L, &CSimpleSlider::RegisterScriptMethods);
    CSimpleSlider::s_metatable = ref;
}

int32_t CSimpleSlider::GetObjectType() {
    if (!CSimpleSlider::s_objectType) {
        CSimpleSlider::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CSimpleSlider::s_objectType;
}

void CSimpleSlider::RegisterScriptMethods(lua_State* L) {
    CSimpleFrame::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, SimpleSliderMethods, NUM_SIMPLE_SLIDER_SCRIPT_METHODS);
}

FrameScript_Object::ScriptIx* CSimpleSlider::GetScriptByName(const char* name, ScriptData& data) {
    auto parentScript = CSimpleFrame::GetScriptByName(name, data);

    if (parentScript) {
        return parentScript;
    }

    if (!SStrCmpI(name, "OnValueChanged", STORM_MAX_STR)) {
        data.wrapper = "return function(self,value) %s end";
        return &this->m_onValueChanged;
    }

    if (!SStrCmpI(name, "OnMinMaxChanged", STORM_MAX_STR)) {
        data.wrapper = "return function(self,min,max) %s end";
        return &this->m_onMinMaxChanged;
    }

    return nullptr;
}

int32_t CSimpleSlider::GetScriptMetaTable() {
    return CSimpleSlider::s_metatable;
}

bool CSimpleSlider::IsA(int32_t type) {
    return type == CSimpleSlider::s_objectType
        || type == CSimpleFrame::s_objectType
        || type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}

void CSimpleSlider::LoadXML(XMLNode* node, CStatus* status) {
    this->CSimpleFrame::LoadXML(node, status);

    int32_t drawLayer = DRAWLAYER_ARTWORK_OVERLAY;

    auto drawLayerStr = node->GetAttributeByName("drawLayer");
    if (drawLayerStr && *drawLayerStr) {
        StringToDrawLayer(drawLayerStr, drawLayer);
    }

    for (auto child = node->m_child; child; child = child->m_next) {
        if (!SStrCmpI(child->GetName(), "ThumbTexture")) {
            auto thumbTexture = LoadXML_Texture(child, this, status);
            this->SetThumbTexture(thumbTexture, drawLayer);
        }
    }

    // TODO
}

void CSimpleSlider::RunOnMinMaxChangedScript() {
    if (!this->m_onMinMaxChanged.luaRef) {
        return;
    }

    auto L = FrameScript_GetContext();

    lua_pushnumber(L, this->m_baseValue);
    lua_pushnumber(L, this->m_range + this->m_baseValue);

    this->RunScript(this->m_onMinMaxChanged, 2, nullptr);
}

void CSimpleSlider::RunOnValueChangedScript() {
    if (!this->m_onValueChanged.luaRef) {
        return;
    }

    auto L = FrameScript_GetContext();

    lua_pushnumber(L, this->m_value);

    this->RunScript(this->m_onValueChanged, 1, nullptr);
}

void CSimpleSlider::SetMinMaxValues(float min, float max) {
    float range = max - min;

    if (this->m_rangeSet && AreEqual(this->m_range, range, WHOA_EPSILON_1) && AreEqual(this->m_baseValue, min, WHOA_EPSILON_1)) {
        return;
    }

    this->m_baseValue = min;
    this->m_range = range;
    this->m_changed = 1;
    this->m_rangeSet = 1;

    this->RunOnMinMaxChangedScript();

    if (this->m_valueSet) {
        // Fit current value within range
        this->SetValue(this->m_value);
    }
}

void CSimpleSlider::SetThumbTexture(CSimpleTexture* texture, int32_t drawLayer) {
    if (this->m_thumbTexture == texture) {
        return;
    }

    if (this->m_thumbTexture) {
        delete this->m_thumbTexture;
    }

    if (texture) {
        texture->SetFrame(this, drawLayer, 1);
        texture->ClearAllPoints();
    }

    this->m_changed = 1;
    this->m_thumbTexture = texture;
}

void CSimpleSlider::SetValue(float value) {
    if (!this->m_rangeSet) {
        return;
    }

    auto v4 = value;
    auto v5 = std::max(value, this->m_baseValue);
    auto maxValue = this->m_range + this->m_baseValue;

    if (maxValue >= v5) {
        if (v4 < this->m_baseValue) {
            v4 = this->m_baseValue;
        }
    } else {
        v4 = maxValue;
    }

    auto newValue = this->Sub96BC10(v4);

    // Clamp to max value
    if (newValue + this->m_valueStep >= maxValue) {
        newValue = maxValue;
    }

    // Clamp to base value
    if (newValue - this->m_valueStep < this->m_baseValue) {
        newValue = this->m_baseValue;
    }

    if (!this->m_valueSet || fabs(newValue - this->m_value) >= 0.00000023841858) {
      this->m_value = newValue;
      this->m_changed = 1;
      this->m_valueSet = 1;

      this->RunOnValueChangedScript();
    }
}

float CSimpleSlider::Sub96BC10(float value) {
    // TODO
    return value;
}
