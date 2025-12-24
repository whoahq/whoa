#include "ui/simple/CSimpleHTML.hpp"
#include "ui/simple/CSimpleFontString.hpp"
#include "ui/simple/CSimpleFontedFrameFont.hpp"
#include "ui/simple/CSimpleHTMLScript.hpp"
#include "ui/simple/CSimpleTop.hpp"
#include "util/CStatus.hpp"
#include "util/SFile.hpp"
#include "util/StringTo.hpp"
#include <common/XML.hpp>

int32_t CSimpleHTML::s_metatable;
int32_t CSimpleHTML::s_objectType;

CONTENTNODE::~CONTENTNODE() {
    if (this->string) {
        delete string;
    }
}

void CSimpleHTML::CreateScriptMetaTable() {
    lua_State* L = FrameScript_GetContext();
    int32_t ref = FrameScript_Object::CreateScriptMetaTable(L, &CSimpleHTML::RegisterScriptMethods);
    CSimpleHTML::s_metatable = ref;
}

int32_t CSimpleHTML::GetObjectType() {
    if (!CSimpleHTML::s_objectType) {
        CSimpleHTML::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CSimpleHTML::s_objectType;
}

void CSimpleHTML::RegisterScriptMethods(lua_State* L) {
    CSimpleFrame::RegisterScriptMethods(L);
    FrameScript_Object::FillScriptMethodTable(L, SimpleHTMLMethods, NUM_SIMPLE_HTML_SCRIPT_METHODS);
}

CSimpleHTML::CSimpleHTML(CSimpleFrame* parent) : CSimpleHyperlinkedFrame(parent) {
    // TODO

    for (int32_t i = 0; i < NUM_HTML_TEXT_TYPES; i++) {
        auto m = SMemAlloc(sizeof(CSimpleFontedFrameFont), __FILE__, __LINE__, 0x0);
        auto font = new (m) CSimpleFontedFrameFont(this);
        this->m_fonts[i] = font;
    }

    // TODO
}

void CSimpleHTML::AddText(const char* text, HTML_TEXT_TYPE type, uint32_t justify) {
    // TODO auto stringMem = CDataAllocator::GetData(CSimpleFontString::s_allocator, 0x0, __FILE__, __LINE__);
    auto stringMem = SMemAlloc(sizeof(CSimpleFontString), __FILE__, __LINE__, 0x0);
    auto string = new (stringMem) CSimpleFontString(this, DRAWLAYER_ARTWORK, 1);

    if (this->m_layoutAnchor) {
        string->SetPoint(
            FRAMEPOINT_TOPLEFT,
            this->m_layoutAnchor,
            FRAMEPOINT_BOTTOMLEFT,
            0.0f,
            this->m_layoutOffset,
            1
        );
    } else {
        string->SetPoint(
            FRAMEPOINT_TOPLEFT,
            this,
            FRAMEPOINT_TOPLEFT,
            0.0f,
            0.0f,
            1
        );
    }

    string->SetWidth(this->GetWidth());

    auto font = this->m_fonts[type];
    if (!font->m_attributes.m_font.GetString()) {
        font = this->m_fonts[0];
    }
    string->SetFontObject(font);

    string->SetJustifyH(justify);
    // TODO something with style flag 0x1000?
    string->m_fontableFlags &= ~FLAG_STYLE_UPDATE;

    string->SetText(text, 1);

    this->m_layoutAnchor = string;
    this->m_layoutOffset = -CSimpleTop::RoundToPixelHeight(string->m_spacing);

    auto contentNode = this->m_content.NewNode(2, 0, 0x0);
    contentNode->string = string;
    // TODO contentNode->dword8 = SStrStr(text, "|H") != 0;
}

void CSimpleHTML::ClearContent() {
    this->m_content.Clear();

    this->m_layoutAnchor = nullptr;
    this->m_layoutOffset = 0.0f;

    // TODO clear hyperlinks
}

void CSimpleHTML::FontUpdated(CSimpleFontedFrameFont* font, int32_t a3) {
    // TODO
}

int32_t CSimpleHTML::GetScriptMetaTable() {
    return CSimpleHTML::s_metatable;
}

bool CSimpleHTML::IsA(int32_t type) {
    return type == CSimpleHTML::s_objectType
        || type == CSimpleFrame::s_objectType
        || type == CScriptRegion::s_objectType
        || type == CScriptObject::s_objectType;
}

void CSimpleHTML::LoadXML(const XMLNode* node, CStatus* status) {
    CSimpleFrame::LoadXML(node, status);

    const char* fontAttr = node->GetAttributeByName("font");
    if (fontAttr && *fontAttr) {
        auto font = CSimpleFont::GetFont(fontAttr, 0);

        if (font) {
            for (int32_t i = 0; i < NUM_HTML_TEXT_TYPES; i++) {
                this->m_fonts[i]->SetFontObject(font);
            }
        } else {
            status->Add(
                STATUS_WARNING,
                "%s %s: Couldn't find font object named %s",
                this->GetObjectTypeName(),
                this->GetDisplayName(),
                fontAttr
            );
        }
    }

    for (XMLNode* child = node->m_child; child; child = child->m_next) {
        if (!SStrCmpI(child->GetName(), "FontString", STORM_MAX_STR)) {
            this->m_fonts[HTML_TEXT_NORMAL]->LoadXML(child, status);
        } else if (!SStrCmpI(child->GetName(), "FontStringHeader1", STORM_MAX_STR)) {
            this->m_fonts[HTML_TEXT_HEADER1]->LoadXML(child, status);
        } else if (!SStrCmpI(child->GetName(), "FontStringHeader2", STORM_MAX_STR)) {
            this->m_fonts[HTML_TEXT_HEADER2]->LoadXML(child, status);
        } else if (!SStrCmpI(child->GetName(), "FontStringHeader3", STORM_MAX_STR)) {
            this->m_fonts[HTML_TEXT_HEADER3]->LoadXML(child, status);
        }
    }

    const char* fileAttr = node->GetAttributeByName("file");
    void* fileData;
    if (fileAttr && SFile::Load(nullptr, fileAttr, &fileData, nullptr, 1, 0x1, nullptr)) {
        this->SetText(static_cast<char*>(fileData), nullptr);
        SFile::Unload(fileData);
    }

    // TODO hyperlinkFormat
}

void CSimpleHTML::ParseBODY(const XMLNode* node, CStatus* status) {
    for (auto child = node->m_child; child; child = child->m_next) {
        if (!SStrCmpI(child->GetName(), "H1", STORM_MAX_STR)) {
            this->ParseP(child, HTML_TEXT_HEADER1, status);

        } else if (!SStrCmpI(child->GetName(), "H2", STORM_MAX_STR)) {
            this->ParseP(child, HTML_TEXT_HEADER2, status);

        } else if (!SStrCmpI(child->GetName(), "H3", STORM_MAX_STR)) {
            this->ParseP(child, HTML_TEXT_HEADER3, status);

        } else if (!SStrCmpI(child->GetName(), "P", STORM_MAX_STR)) {
            this->ParseP(child, HTML_TEXT_NORMAL, status);

        } else if (!SStrCmpI(child->GetName(), "BR", STORM_MAX_STR)) {
            this->AddText("\n", HTML_TEXT_NORMAL, 1);

        } else if (!SStrCmpI(child->GetName(), "IMG", STORM_MAX_STR)) {
            this->ParseIMG(child, status);

        } else  {
            status->Add(
                STATUS_WARNING,
                "Frame %s: Unknown element type: %s",
                this->GetDisplayName(),
                child->GetName()
            );
        }
    }
}

void CSimpleHTML::ParseIMG(const XMLNode* node, CStatus* status) {
    // TODO
}

void CSimpleHTML::ParseP(const XMLNode* node, HTML_TEXT_TYPE type, CStatus* status) {
    uint32_t justify = 1;
    auto justifyAttr = node->GetAttributeByName("align");
    if (justifyAttr && *justifyAttr) {
        StringToJustify(justifyAttr, justify);
    }

    auto body = node->GetBody();
    if (!body) {
        body = "";
    }

    auto text = SStrDupA(body, __FILE__, __LINE__);

    uint32_t offset = 0;
    for (auto child = node->m_child; child; child = child->m_next) {
        if (!SStrCmpI(child->GetName(), "BR", STORM_MAX_STR)) {
            char* newText = static_cast<char*>(SMemAlloc(SStrLen(text) + 3, __FILE__, __LINE__, 0x0));
            SStrCopy(newText, text, child->m_offset + offset + 1);
            SStrPack(newText, "|n", STORM_MAX_STR);
            SStrPack(newText, &text[child->m_offset + offset], STORM_MAX_STR);
            SMemFree(text, __FILE__, __LINE__, 0x0);
            text = newText;

            offset += 2;
        } else if (!SStrCmpI(child->GetName(), "A", STORM_MAX_STR)) {
            // TODO handle anchor child
        } else {
            status->Add(
                STATUS_WARNING,
                "Frame %s: Unknown element type: %s",
                this->GetDisplayName(),
                child->GetName()
            );
        }
    }

    int32_t v22 = 0;
    int32_t v36 = 1;
    auto v23 = text;
    auto v24 = text;

    if (*text) {
        do {
            switch (*v23) {
                case '\t':
                case '\n':
                case '\r':
                case ' ': {
                    if (!v36 && !v22) {
                        v22 = 1;
                        *v24 = ' ';
                        v24++;
                    }

                    break;
                }

                case '|': {
                    if (v23[1] != 'n') {
                        v22 = 0;
                        v36 = 0;

                        *v24 = *v23;
                        v24++;

                        break;
                    }

                    if (v24 > text && v22) {
                        v24--;
                    }

                    *v24 = '|';
                    v24++;
                    *v24 = 'n';
                    v24++;

                    v22 = 0;
                    v36 = 1;

                    v23++;

                    break;
                }

                default: {
                    v22 = 0;
                    v36 = 0;

                    *v24 = *v23;
                    v24++;

                    break;
                }
            }

            v23++;
        } while (*v23);

        if (v24 > text && v22) {
            v24--;
        }
    }

    *v24 = '\0';

    this->AddText(text, type, justify);

    SMemFree(text, __FILE__, __LINE__, 0x0);
}

bool CSimpleHTML::SetText(const char* text, CStatus* status) {
    static auto s_nullStatus = CStatus();

    if (!status) {
        status = &s_nullStatus;
    }

    if (!text) {
        text = "";
    }

    // TODO some sort of guard condition

    this->ClearContent();

    auto textLength = SStrLen(text);
    auto tree = XMLTree_Load(text, textLength);
    bool success = false;

    if (tree) {
        auto node = XMLTree_GetRoot(tree);

        if (!SStrCmpI(node->GetName(), "HTML", STORM_MAX_STR)) {
            for (auto child = node->m_child; child; child = child->m_next) {
                if (!SStrCmpI(child->GetName(), "BODY", STORM_MAX_STR)) {
                    success = true;
                    this->ParseBODY(child, status);

                    break;
                }

                status->Add(
                    STATUS_WARNING,
                    "Frame %s: Unknown element type: %s (expected BODY)",
                    this->GetDisplayName(),
                    node->GetName()
                );
            }
        } else {
            status->Add(
                STATUS_WARNING,
                "Frame %s: Unknown element type: %s (expected HTML)",
                this->GetDisplayName(),
                node->GetName()
            );
        }

        XMLTree_Free(tree);
    }

    if (success) {
        // TODO set flag

       return true;
    }

    this->AddText(text, HTML_TEXT_NORMAL, 1);

    return false;
}
