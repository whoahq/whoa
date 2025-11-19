#include "ui/CSimpleCheckbox.hpp"
#include "ui/CSimpleCheckboxScript.hpp"
#include "ui/CSimpleTexture.hpp"
#include "ui/LoadXML.hpp"
#include "util/StringTo.hpp"
#include <common/XML.hpp>

int32_t CSimpleCheckbox::s_metatable;
int32_t CSimpleCheckbox::s_objectType;

void CSimpleCheckbox::CreateScriptMetaTable() {
    lua_State* L = FrameScript_GetContext();
    int32_t ref = FrameScript_Object::CreateScriptMetaTable(L, &CSimpleCheckbox::RegisterScriptMethods);
    CSimpleCheckbox::s_metatable = ref;
}

int32_t CSimpleCheckbox::GetObjectType() {
    if (!CSimpleCheckbox::s_objectType) {
        CSimpleCheckbox::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CSimpleCheckbox::s_objectType;
}

void CSimpleCheckbox::RegisterScriptMethods(lua_State* L) {
    CSimpleButton::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, SimpleCheckboxMethods, NUM_SIMPLE_CHECKBOX_SCRIPT_METHODS);
}

int32_t CSimpleCheckbox::GetScriptMetaTable() {
    return CSimpleCheckbox::s_metatable;
}

bool CSimpleCheckbox::IsA(int32_t type) {
    return type == CSimpleCheckbox::s_objectType
        || type == CSimpleButton::s_objectType
        || type == CSimpleFrame::s_objectType
        || type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}

int32_t CSimpleCheckbox::GetChecked() {
    return this->m_checked;
}

void CSimpleCheckbox::LoadXML(XMLNode* node, CStatus* status) {
    CSimpleButton::LoadXML(node, status);

    auto checkedAttr = node->GetAttributeByName("checked");
    if (checkedAttr && *checkedAttr) {
        auto checked = StringToBOOL(checkedAttr);
        this->SetChecked(checked, 0);
    }

    for (auto child = node->m_child; child; child = child->m_next) {
        if (!SStrCmpI(child->GetName(), "CheckedTexture")) {
            auto checkedTexture = LoadXML_Texture(child, this, status);
            this->SetCheckedTexture(checkedTexture);

        } else if (!SStrCmpI(child->GetName(), "DisabledCheckedTexture")) {
            auto disabledTexture = LoadXML_Texture(child, this, status);
            this->SetDisabledCheckedTexture(disabledTexture);

        }
    }
}

void CSimpleCheckbox::SetChecked(int32_t checked, int32_t force) {
    if (checked == this->m_checked && !force) {
        return;
    }

    this->m_checked = checked;

    if (this->m_checkedTexture) {
        this->m_checkedTexture->Hide();
    }

    if (this->m_disabledTexture) {
        this->m_disabledTexture->Hide();
    }

    if (this->m_checked) {
        if (this->m_disabledTexture && this->m_state == BUTTONSTATE_DISABLED) {
            this->m_disabledTexture->Show();
        } else if (this->m_checkedTexture) {
            this->m_checkedTexture->Show();
        }
    }
}

void CSimpleCheckbox::SetCheckedTexture(CSimpleTexture* texture) {
    if (this->m_checkedTexture) {
        delete this->m_checkedTexture;
    }

    if (texture) {
        texture->SetFrame(this, DRAWLAYER_ARTWORK_OVERLAY, 0);
    }

    this->m_checkedTexture = texture;
    this->SetChecked(this->m_checked, 1);
}

void CSimpleCheckbox::SetDisabledCheckedTexture(CSimpleTexture* texture) {
    if (this->m_disabledTexture) {
        delete this->m_disabledTexture;
    }

    if (texture) {
        texture->SetFrame(this, DRAWLAYER_ARTWORK_OVERLAY, 0);
    }

    this->m_disabledTexture = texture;
    this->SetChecked(this->m_checked, 1);
}
