#include "ui/CSimpleFontable.hpp"
#include "ui/CSimpleFont.hpp"

CSimpleFontable::~CSimpleFontable() {
    this->SetFontObject(nullptr);
}

CSimpleFont* CSimpleFontable::GetFontObject() {
    return this->m_fontObject;
}

void CSimpleFontable::SetFontObject(CSimpleFont* fontObject) {
    if (this->m_fontObject == fontObject) {
        return;
    }

    if (this->m_fontObject) {
        this->m_fontObject->m_fontableList.UnlinkNode(this);
    }

    this->m_fontObject = fontObject;

    if (fontObject) {
        fontObject->m_fontableList.LinkToTail(this);

        fontObject->m_attributes.m_flags |= FLAG_COMPLETE_UPDATE;
        this->FontObjectUpdated(fontObject->m_attributes);
        fontObject->m_attributes.m_flags &= ~(FLAG_COMPLETE_UPDATE);
    }
}
