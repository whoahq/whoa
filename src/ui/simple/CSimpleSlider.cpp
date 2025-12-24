#include "ui/simple/CSimpleSlider.hpp"
#include "math/Utils.hpp"
#include "ui/LoadXML.hpp"
#include "ui/simple/CSimpleSliderScript.hpp"
#include "ui/simple/CSimpleTexture.hpp"
#include "util/Lua.hpp"
#include "util/StringTo.hpp"
#include <common/XML.hpp>
#include <tempest/Math.hpp>

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

void CSimpleSlider::LoadXML(const XMLNode* node, CStatus* status) {
    this->CSimpleFrame::LoadXML(node, status);

    // Thumb

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

    // Value step

    auto valueStepStr = node->GetAttributeByName("valueStep");
    auto valueStep = valueStepStr && *valueStepStr ? SStrToFloat(valueStepStr) : 0.001f;
    this->SetValueStep(valueStep);

    // Min, max, and default values

    auto minValueStr = node->GetAttributeByName("minValue");
    if (minValueStr && *minValueStr) {
        auto minValue = SStrToFloat(minValueStr);

        auto maxValueStr = node->GetAttributeByName("maxValue");
        if (maxValueStr && *maxValueStr) {
            auto maxValue = SStrToFloat(maxValueStr);

            this->SetMinMaxValues(minValue, maxValue);

            auto defaultValueStr = node->GetAttributeByName("defaultValue");
            if (defaultValueStr && *defaultValueStr) {
                auto defaultValue = SStrToFloat(defaultValueStr);

                this->SetValue(defaultValue);
            }
        }
    }

    // Orientation

    auto orientationStr = node->GetAttributeByName("orientation");
    if (orientationStr && *orientationStr) {
        ORIENTATION orientation;

        if (StringToOrientation(orientationStr, orientation)) {
            this->SetOrientation(orientation);
        } else {
            status->Add(
                STATUS_WARNING,
                "Frame %s: Unknown orientation %s in element %s",
                this->GetDisplayName(),
                orientationStr,
                node->GetName()
            );
        }
    }
}

void CSimpleSlider::OnFrameSizeChanged(const CRect& rect) {
    this->CSimpleFrame::OnFrameSizeChanged(rect);

    this->m_changed = 1;
}


void CSimpleSlider::OnLayerHide() {
    this->m_buttonDown = 0;

    this->CSimpleFrame::OnLayerHide();
}

int32_t CSimpleSlider::OnLayerMouseDown(const CMouseEvent& evt, const char* btn) {
    // TODO something like CSimpleFrame::IsEnabled or CSimpleFrame::GetFrameFlag
    if (!(this->m_flags & 0x400)) {
        this->m_buttonDown = 1;
        this->OnLayerTrackUpdate(evt);
    }

    return this->CSimpleFrame::OnLayerMouseDown(evt, btn);
}

int32_t CSimpleSlider::OnLayerMouseUp(const CMouseEvent& evt, const char* btn) {
    this->m_buttonDown = 0;

    return this->CSimpleFrame::OnLayerMouseUp(evt, btn);
}

int32_t CSimpleSlider::OnLayerTrackUpdate(const CMouseEvent& evt) {
    if (this->m_buttonDown && this->m_thumbTexture) {
        auto thumbTexture = this->m_thumbTexture;

        float area;
        float offset;

        if (this->m_orientation == ORIENTATION_VERTICAL) {
            area = this->m_rect.maxY - this->m_rect.minY - thumbTexture->GetHeight();
            offset = this->m_rect.maxY - (thumbTexture->GetHeight() / 2.0f) - evt.y;
        } else {
            area = this->m_rect.maxX - this->m_rect.minX - thumbTexture->GetWidth();
            offset = evt.x - ((thumbTexture->GetWidth() / 2.0f) + this->m_rect.minX);
        }

        float value = (this->m_range + this->m_baseValue - this->m_baseValue) * (offset / area) + this->m_baseValue;

        this->SetValue(value);
    }

    return this->CSimpleFrame::OnLayerTrackUpdate(evt);
}

void CSimpleSlider::OnLayerUpdate(float elapsedSec) {
    this->CSimpleFrame::OnLayerUpdate(elapsedSec);

    if (this->m_changed && this->m_thumbTexture && this->m_rangeSet && this->m_valueSet) {
        auto valueRange = this->m_range + this->m_baseValue - this->m_baseValue;
        auto valueOffset = CMath::fequal(valueRange, 0.0f)
            ? 0.0f
            : (this->m_value - this->m_baseValue) / valueRange;

        if (this->m_orientation == ORIENTATION_VERTICAL) {
            auto rangeY = (this->m_rect.maxY - this->m_rect.minY) / this->m_layoutScale;
            auto offsetY = -((rangeY - this->m_thumbTexture->GetHeight()) * valueOffset);

            this->m_thumbTexture->SetPoint(FRAMEPOINT_TOP, this, FRAMEPOINT_TOP, 0.0f, offsetY, 1);
        } else {
            auto rangeX = (this->m_rect.maxX - this->m_rect.minX) / this->m_layoutScale;
            auto offsetX = (rangeX - this->m_thumbTexture->GetWidth()) * valueOffset;

            this->m_thumbTexture->SetPoint(FRAMEPOINT_LEFT, this, FRAMEPOINT_LEFT, offsetX, 0.0f, 1);
        }

        this->m_changed = 0;
    }
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

void CSimpleSlider::SetOrientation(ORIENTATION orientation) {
    this->m_orientation = orientation;

    if (this->m_thumbTexture) {
        this->m_thumbTexture->ClearAllPoints();
    }

    this->m_changed = 1;
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

    auto newValue = this->StepValue(v4);

    // Clamp to max value
    if (newValue + this->m_valueStep > maxValue) {
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

void CSimpleSlider::SetValueStep(float valueStep) {
    valueStep = std::max(valueStep, 0.00000011920929f);

    if (CMath::fequal(this->m_valueStep, valueStep)) {
        return;
    }

    this->m_valueStep = valueStep;

    if (this->m_valueSet) {
        this->SetValue(this->m_value);
    }
}

float CSimpleSlider::StepValue(float value) {
    if (this->m_valueStep == 0.0f) {
        return value;
    }

    auto valueOffset = value - this->m_baseValue;
    auto halfStep = this->m_valueStep / 2.0f;

    if (valueOffset <= 0.0f) {
        return CMath::fint((valueOffset - halfStep) / this->m_valueStep) * this->m_valueStep + this->m_baseValue;
    }

    return CMath::fint((valueOffset + halfStep) / this->m_valueStep) * this->m_valueStep + this->m_baseValue;
}

void CSimpleSlider::UnregisterRegion(CSimpleRegion* region) {
    if (this->m_thumbTexture == region) {
        this->m_thumbTexture = nullptr;
    }

    this->CSimpleFrame::UnregisterRegion(region);
}
