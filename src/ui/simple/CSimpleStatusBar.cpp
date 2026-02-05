#include "ui/simple/CSimpleStatusBar.hpp"
#include "ui/LoadXML.hpp"
#include "ui/simple/CSimpleStatusBarScript.hpp"
#include "ui/simple/CSimpleTexture.hpp"
#include "util/CStatus.hpp"
#include "util/Lua.hpp"
#include "util/StringTo.hpp"
#include <common/XML.hpp>

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

float CSimpleStatusBar::GetMaxValue() const {
    return this->m_maxValue;
}

float CSimpleStatusBar::GetMinValue() const {
    return this->m_minValue;
}

float CSimpleStatusBar::GetValue() const {
    return this->m_value;
}

bool CSimpleStatusBar::IsA(int32_t type) {
    return type == CSimpleStatusBar::s_objectType
        || type == CSimpleFrame::s_objectType
        || type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}

void CSimpleStatusBar::LoadXML(const XMLNode* node, CStatus* status) {
    this->CSimpleFrame::LoadXML(node, status);

    int32_t drawlayer = DRAWLAYER_ARTWORK;
    auto drawlayerAttr = node->GetAttributeByName("drawLayer");
    if (drawlayerAttr && *drawlayerAttr) {
        StringToDrawLayer(drawlayerAttr, drawlayer);
    }

    for (auto child = node->GetChild(); child; child = child->GetSibling()) {
        if (!SStrCmpI(child->GetName(), "BarTexture")) {
            auto texture = LoadXML_Texture(child, this, status);
            this->SetBarTexture(texture, drawlayer);
        } else if (!SStrCmpI(child->GetName(), "BarColor")) {
            CImVector color = {};
            LoadXML_Color(child, color);
            this->SetStatusBarColor(color);
        }
    }

    auto minValueAttr = node->GetAttributeByName("minValue");
    if (minValueAttr && *minValueAttr) {
        auto maxValueAttr = node->GetAttributeByName("maxValue");
        if (maxValueAttr && *maxValueAttr) {
            auto minValue = SStrToFloat(minValueAttr);
            auto maxValue = SStrToFloat(maxValueAttr);

            if (minValue < -1.0e12 || minValue > 1.0e12 || maxValue < -1.0e12 || maxValue > 1.0e12) {
                status->Add(STATUS_ERROR, "Frame %s: Min or Max out of range", this->GetDisplayName());
            } else if (maxValue - minValue > 1.0e12) {
                status->Add(STATUS_ERROR, "Frame %s: Min and Max too far apart", this->GetDisplayName());
            } else {
                this->SetMinMaxValues(minValue, maxValue);
            }

            auto defaultValueAttr = node->GetAttributeByName("defaultValue");
            if (defaultValueAttr && *defaultValueAttr) {
                auto defaultValue = SStrToFloat(defaultValueAttr);
                this->SetValue(defaultValue);
            }
        }
    }

    auto orientationAttr = node->GetAttributeByName("orientation");
    if (orientationAttr && *orientationAttr) {
        ORIENTATION orientation;
        if (StringToOrientation(orientationAttr, orientation)) {
            this->SetOrientation(orientation);
        } else {
            status->Add(STATUS_WARNING, "Frame %s: Unknown orientation %s in element %s", this->GetDisplayName(), orientationAttr, node->GetName());
        }
    }

    auto rotatesTextureAttr = node->GetAttributeByName("rotatesTexture");
    if (rotatesTextureAttr && *rotatesTextureAttr) {
        auto rotatesTexture = StringToBOOL(rotatesTextureAttr);
        this->SetRotatesTexture(rotatesTexture);
    }
}

void CSimpleStatusBar::RunOnMinMaxChangedScript() {
    if (!this->m_onMinMaxChanged.luaRef) {
        return;
    }

    auto L = FrameScript_GetContext();

    lua_pushnumber(L, this->m_minValue);
    lua_pushnumber(L, this->m_maxValue);

    this->RunScript(this->m_onMinMaxChanged, 2, nullptr);
}

void CSimpleStatusBar::RunOnValueChangedScript() {
    if (!this->m_onValueChanged.luaRef) {
        return;
    }

    auto L = FrameScript_GetContext();

    lua_pushnumber(L, this->m_value);

    this->RunScript(this->m_onValueChanged, 1, nullptr);
}

void CSimpleStatusBar::SetBarTexture(CSimpleTexture* texture, int32_t drawlayer) {
    // No change
    if (this->m_barTexture == texture) {
        return;
    }

    if (this->m_barTexture) {
        delete this->m_barTexture;
    }

    if (texture) {
        texture->SetFrame(this, drawlayer, true);

        texture->SetPoint(FRAMEPOINT_BOTTOMLEFT, this, FRAMEPOINT_BOTTOMLEFT, 0.0f, 0.0f, true);
        texture->SetPoint(FRAMEPOINT_BOTTOMRIGHT, this, FRAMEPOINT_BOTTOMRIGHT, 0.0f, 0.0f, true);
        texture->SetPoint(FRAMEPOINT_TOPLEFT, this, FRAMEPOINT_TOPLEFT, 0.0f, 0.0f, true);
        texture->SetPoint(FRAMEPOINT_TOPRIGHT, this, FRAMEPOINT_TOPRIGHT, 0.0f, 0.0f, true);
    }

    this->m_barTexture = texture;

    this->m_changed = true;
}

void CSimpleStatusBar::SetMinMaxValues(float min, float max) {
    if (min > max) {
        min = max;
    }

    // No change
    if (this->m_rangeSet && this->m_minValue == min && this->m_maxValue == max) {
        return;
    }

    this->m_minValue = min;
    this->m_maxValue = max;

    this->m_changed = true;
    this->m_rangeSet = true;

    this->RunOnMinMaxChangedScript();

    if (this->m_valueSet) {
        this->SetValue(this->m_value);
    }
}

void CSimpleStatusBar::SetOrientation(ORIENTATION orientation) {
    // TODO
}

void CSimpleStatusBar::SetRotatesTexture(int32_t enabled) {
    // TODO
}

void CSimpleStatusBar::SetStatusBarColor(const CImVector& color) {
    if (this->m_barTexture) {
        this->m_barTexture->SetVertexColor(color);
    }
}

void CSimpleStatusBar::SetValue(float value) {
    if (!this->m_rangeSet) {
        return;
    }

    // Clamp value
    value = std::min(std::max(value, this->m_minValue), this->m_maxValue);

    // No change
    if (this->m_valueSet && this->m_value == value) {
        return;
    }

    this->m_value = value;

    this->m_changed = true;
    this->m_valueSet = true;

    this->RunOnValueChangedScript();
}
