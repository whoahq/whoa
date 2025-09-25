#include "ui/CSimpleFont.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Font.hpp"
#include "ui/CSimpleFontScript.hpp"
#include "ui/FrameXML.hpp"
#include "ui/LoadXML.hpp"
#include "ui/Types.hpp"
#include "util/CStatus.hpp"
#include "util/StringTo.hpp"
#include <common/XML.hpp>
#include <storm/String.hpp>

TSHashTable<SIMPLEFONT, HASHKEY_STRI> CSimpleFont::s_fontList;
int32_t CSimpleFont::s_metatable;
int32_t CSimpleFont::s_objectType;

SIMPLEFONT::SIMPLEFONT() : TSHashObject<SIMPLEFONT, HASHKEY_STRI>() {
    auto m = SMemAlloc(sizeof(CSimpleFont), __FILE__, __LINE__, 0x0);
    this->font = new (m) CSimpleFont();
}

void CSimpleFont::CreateScriptMetaTable() {
    lua_State* L = FrameScript_GetContext();
    int32_t ref = FrameScript_Object::CreateScriptMetaTable(L, &CSimpleFont::RegisterScriptMethods);
    CSimpleFont::s_metatable = ref;
}

CSimpleFont* CSimpleFont::GetFont(const char* name, int32_t a2) {
    SIMPLEFONT* hashed = CSimpleFont::s_fontList.Ptr(name);

    if (hashed) {
        return hashed->font;
    }

    if (!a2) {
        return nullptr;
    }

    hashed = CSimpleFont::s_fontList.New(name, 0, 0);

    CSimpleFont* font = hashed->font;

    if (font->m_name) {
        font->UnregisterScriptObject(font->m_name);
    }

    font->m_name = hashed->m_key.GetString();

    if (font->m_name) {
        font->RegisterScriptObject(font->m_name);
    }

    return hashed->font;
}

int32_t CSimpleFont::GetObjectType() {
    if (!CSimpleFont::s_objectType) {
        CSimpleFont::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CSimpleFont::s_objectType;
}

void CSimpleFont::RegisterScriptMethods(lua_State* L) {
    FrameScript_Object::FillScriptMethodTable(L, SimpleFontMethods, NUM_SIMPLE_FONT_SCRIPT_METHODS);
}

CSimpleFont::CSimpleFont() : FrameScript_Object() {
    // TODO
}

void CSimpleFont::FontObjectUpdated(CSimpleFontStringAttributes& attributes) {
    attributes.Update(this->m_attributes, this->m_fontableFlags);
    this->UpdateObjects();
}

char* CSimpleFont::GetName() {
    return const_cast<char*>(this->m_name);
}

int32_t CSimpleFont::GetScriptMetaTable() {
    return CSimpleFont::s_metatable;
}

bool CSimpleFont::IsA(int32_t type) {
    return type == CSimpleFont::s_objectType;
}

void CSimpleFont::LoadXML(XMLNode* node, CStatus* status) {
    const char* inheritsAttr = node->GetAttributeByName("inherits");
    if (inheritsAttr && *inheritsAttr) {
        auto font = CSimpleFont::GetFont(inheritsAttr, 0);

        if (font) {
            this->SetFontObject(font);
        } else {
            status->Add(
                STATUS_WARNING,
                "Couldn't find inherited font: %s",
                inheritsAttr
            );
        }
    }

    const char* fontAttr = node->GetAttributeByName("font");
    if (fontAttr && *fontAttr) {
        auto font = CSimpleFont::GetFont(fontAttr, 0);

        if (font) {
            this->SetFontObject(font);
        } else {
            float fontHeight = 0.0f;

            XMLNode* fontHeightNode = node->GetChildByName("FontHeight");
            if (fontHeightNode) {
                LoadXML_Value(fontHeightNode, fontHeight, status);
            }

            if (fontHeight == 0.0f) {
                status->Add(
                    STATUS_WARNING,
                    "Font %s: Missing font height in %s element",
                    this->GetDisplayName(),
                    node->GetName()
                );

                return;
            }

            uint32_t fontFlags = 0;

            const char* outlineAttr = node->GetAttributeByName("outline");
            if (outlineAttr && *outlineAttr) {
                if (!SStrCmpI(outlineAttr, "NORMAL", STORM_MAX_STR)) {
                    fontFlags |= FONT_OUTLINE;
                } else if (!SStrCmpI(outlineAttr, "THICK", STORM_MAX_STR)) {
                    fontFlags |= (FONT_OUTLINE | FONT_THICKOUTLINE);
                }
            }

            const char* monochromeAttr = node->GetAttributeByName("monochrome");
            if (monochromeAttr && *monochromeAttr && StringToBOOL(monochromeAttr)) {
                fontFlags |= FONT_MONOCHROME;
            }

            if (this->m_attributes.SetFont(fontAttr, fontHeight, fontFlags)) {
                this->m_fontableFlags &= ~FLAG_FONT_UPDATE;
            } else {
                status->Add(
                    STATUS_WARNING,
                    "Font %s: Unable to load font file %s",
                    this->GetDisplayName(),
                    fontAttr
                );
            }
        }
    }

    const char* spacingAttr = node->GetAttributeByName("spacing");
    if (spacingAttr && *spacingAttr) {
        float spacing = SStrToFloat(spacingAttr);
        float ndcSpacing = spacing / (CoordinateGetAspectCompensation() * 1024.0);
        float ddcSpacing = NDCToDDCWidth(ndcSpacing);

        this->m_attributes.SetSpacing(ddcSpacing);
        this->m_fontableFlags &= ~FLAG_SPACING_UPDATE;
    }

    const char* justifyVAttr = node->GetAttributeByName("justifyV");
    if (justifyVAttr && *justifyVAttr) {
        uint32_t justify;

        if (StringToJustify(justifyVAttr, justify)) {
            this->m_attributes.SetJustifyV(justify);
            this->m_fontableFlags &= ~FLAG_STYLE_UPDATE;
        }
    }

    const char* justifyHAttr = node->GetAttributeByName("justifyH");
    if (justifyHAttr && *justifyHAttr) {
        uint32_t justify;

        if (StringToJustify(justifyHAttr, justify)) {
            this->m_attributes.SetJustifyH(justify);
            this->m_fontableFlags &= ~FLAG_STYLE_UPDATE;
        }
    }

    const char* nonspacewrapAttr = node->GetAttributeByName("nonspacewrap");
    if (nonspacewrapAttr && *nonspacewrapAttr) {
        bool nonspacewrap = StringToBOOL(nonspacewrapAttr);
        this->m_attributes.SetNonSpaceWrap(nonspacewrap);
    }

    const char* indentedAttr = node->GetAttributeByName("indented");
    if (indentedAttr && *indentedAttr) {
        bool indented = StringToBOOL(indentedAttr);
        this->m_attributes.SetIndented(indented);
    }

    for (XMLNode* child = node->m_child; child; child = child->m_next) {
        if (!SStrCmpI(child->GetName(), "Color", STORM_MAX_STR)) {
            CImVector color;
            LoadXML_Color(child, color);
            this->m_attributes.SetColor(color);
            this->m_fontableFlags &= ~FLAG_COLOR_UPDATE;

        } else if(!SStrCmpI(child->GetName(), "Shadow", STORM_MAX_STR)) {
            C2Vector shadowOffset = { 0.001f, -0.001f };
            CImVector shadowColor = { 0x00, 0x00, 0x00, 0xFF };

            auto colorChild = child->GetChildByName("Color");
            if (colorChild) {
                LoadXML_Color(colorChild, shadowColor);
            }

            auto offsetChild = child->GetChildByName("Offset");
            if (offsetChild) {
                LoadXML_Dimensions(offsetChild, shadowOffset.x, shadowOffset.y, status);
            }

            this->m_attributes.SetShadow(shadowColor, shadowOffset);
            this->m_fontableFlags &= ~FLAG_SHADOW_UPDATE;
        }
    }
}

void CSimpleFont::UpdateObjects() {
    if (this->m_attributes.m_flags) {
        for (auto node = this->m_fontableList.Head(); node; node = this->m_fontableList.Link(node)->Next()) {
            node->FontObjectUpdated(this->m_attributes);
        }

        this->m_attributes.m_flags &= ~FLAG_COMPLETE_UPDATE;
    }
}
