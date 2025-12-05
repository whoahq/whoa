#include "ui/FrameXML.hpp"
#include "ui/CSimpleButton.hpp"
#include "ui/CSimpleCheckbox.hpp"
#include "ui/CSimpleEditBox.hpp"
#include "ui/CSimpleFont.hpp"
#include "ui/CSimpleFrame.hpp"
#include "ui/CSimpleHTML.hpp"
#include "ui/CSimpleModel.hpp"
#include "ui/CSimpleScrollFrame.hpp"
#include "ui/CSimpleSlider.hpp"
#include "util/CStatus.hpp"
#include "util/SFile.hpp"
#include <cstdlib>
#include <cstring>
#include <common/XML.hpp>
#include <storm/String.hpp>

int32_t FrameXML::s_debugLevel;
TSHashTable<FrameFactoryNode, HASHKEY_STRI> FrameXML::s_factoryHash;
TSHashTable<HashedNode, HASHKEY_STRI> FrameXML::s_nodeHash;

CSimpleFrame* Create_SimpleButton(CSimpleFrame* parent) {
    // TODO
    // auto m = CDataAllocator::GetData(CSimpleFrame::s_simpleButtonHeap, 0, __FILE__, __LINE__);

    auto m = SMemAlloc(sizeof(CSimpleButton), __FILE__, __LINE__, 0x0);
    return new (m) CSimpleButton(parent);
}

CSimpleFrame* Create_SimpleCheckButton(CSimpleFrame* parent) {
    // TODO
    // auto m = CDataAllocator::GetData(CSimpleCheckbox::s_simpleCheckboxHeap, 0, __FILE__, __LINE__);

    auto m = SMemAlloc(sizeof(CSimpleCheckbox), __FILE__, __LINE__, 0x0);
    return new (m) CSimpleCheckbox(parent);
}

CSimpleFrame* Create_SimpleEditBox(CSimpleFrame* parent) {
    // TODO
    // auto m = CDataAllocator::GetData(CSimpleEditBox::s_simpleEditBoxHeap, 0, __FILE__, __LINE__);

    auto m = SMemAlloc(sizeof(CSimpleEditBox), __FILE__, __LINE__, 0x0);
    return new (m) CSimpleEditBox(parent);
}

CSimpleFrame* Create_SimpleFrame(CSimpleFrame* parent) {
    // TODO
    // auto m = CDataAllocator::GetData(CSimpleFrame::s_simpleFrameHeap, 0, __FILE__, __LINE__);

    auto m = SMemAlloc(sizeof(CSimpleFrame), __FILE__, __LINE__, 0x0);
    return new (m) CSimpleFrame(parent);
}

CSimpleFrame* Create_SimpleMessageFrame(CSimpleFrame* parent) {
    // TODO

    return nullptr;
}

CSimpleFrame* Create_SimpleModel(CSimpleFrame* parent) {
    // TODO
    // auto m = CDataAllocator::GetData(CSimpleFrame::s_simpleModelHeap, 0, __FILE__, __LINE__);

    auto m = SMemAlloc(sizeof(CSimpleModel), __FILE__, __LINE__, 0x0);
    return new (m) CSimpleModel(parent);
}

CSimpleFrame* Create_SimpleScrollFrame(CSimpleFrame* parent) {
    // TODO
    // auto m = CDataAllocator::GetData(CSimpleScrollFrame::s_simpleScrollHeap, 0, __FILE__, __LINE__);

    auto m = SMemAlloc(sizeof(CSimpleScrollFrame), __FILE__, __LINE__, 0x0);
    return new (m) CSimpleScrollFrame(parent);
}

CSimpleFrame* Create_SimpleScrollingMessageFrame(CSimpleFrame* parent) {
    // TODO

    return nullptr;
}

CSimpleFrame* Create_SimpleSlider(CSimpleFrame* parent) {
    // TODO
    // auto m = CDataAllocator::GetData(CSimpleSlider::s_simpleSliderHeap, 0, __FILE__, __LINE__);

    auto m = SMemAlloc(sizeof(CSimpleSlider), __FILE__, __LINE__, 0x0);
    return new (m) CSimpleSlider(parent);
}

CSimpleFrame* Create_SimpleHTML(CSimpleFrame* parent) {
    // TODO
    // auto m = CDataAllocator::GetData(CSimpleHTML::s_simpleHTMLHeap, 0, __FILE__, __LINE__);

    auto m = SMemAlloc(sizeof(CSimpleHTML), __FILE__, __LINE__, 0x0);
    return new (m) CSimpleHTML(parent);
}

CSimpleFrame* Create_SimpleStatusBar(CSimpleFrame* parent) {
    // TODO

    return nullptr;
}

CSimpleFrame* Create_SimpleColorSelect(CSimpleFrame* parent) {
    // TODO

    return nullptr;
}

CSimpleFrame* Create_SimpleMovieFrame(CSimpleFrame* parent) {
    // TODO

    return nullptr;
}

XMLNode* FrameXML_AcquireHashNode(const char* name, const char*& tainted, bool& locked) {
    HashedNode* hashedNode = FrameXML::s_nodeHash.Ptr(name);

    if (!hashedNode) {
        return nullptr;
    }

    locked = hashedNode->locked;
    tainted = hashedNode->tainted;
    hashedNode->locked = true;

    return hashedNode->node;
}

int32_t FrameXML_CheckSignature(const char* tocPath, const char* a2, const unsigned char* key, unsigned char* digest) {
    // TODO

    return 3;
}

/**
 * Find and return the appropriate parent name.
 *
 * Parent priorities:
 * 1. parent attribute on local node
 * 2. parent attribute on last inherited node with parent attribute present
 */
const char* FindParent(XMLNode* node) {
    const char* parentName = node->GetAttributeByName("parent");

    if (parentName && *parentName) {
        return parentName;
    }

    const char* inheritNames = node->GetAttributeByName("inherits");

    if (!inheritNames || !*inheritNames) {
        return nullptr;
    }

    char inheritName[1024];

    do {
        SStrTokenize(&inheritNames, inheritName, 0x400u, " ,", 0);

        if (!*inheritName) {
            break;
        }

        auto hashedNode = FrameXML::s_nodeHash.Ptr(inheritName);

        if (hashedNode) {
            auto inheritNode = hashedNode->node;
            int32_t v10 = hashedNode->locked;
            hashedNode->locked = true;

            if (inheritNode && !v10) {
                parentName = inheritNode->GetAttributeByName("parent");
                FrameXML::s_nodeHash.Ptr(inheritName)->locked = false;
            }
        }
    } while (*inheritName);

    return parentName;
}

CSimpleFrame* FrameXML_CreateFrame(XMLNode* node, CSimpleFrame* parent, CStatus* status) {
    if (FrameXML::s_debugLevel > 0) {
        const char* name = node->GetAttributeByName("name");

        if (name && *name) {
            status->Add(STATUS_INFO, "-- Creating %s named %s", node->m_name.m_str, name);
        } else {
            status->Add(STATUS_INFO, "-- Creating unnamed %s", node->m_name.m_str);
        }
    }

    auto factoryNode = FrameXML::s_factoryHash.Ptr(node->m_name.m_str);

    if (!factoryNode) {
        status->Add(STATUS_WARNING, "Unknown frame type: %s", node->m_name.m_str);
        return nullptr;
    }

    const char* parentName = FindParent(node);

    if (parentName && *parentName) {
        int32_t parentType = CSimpleFrame::GetObjectType();
        parent = static_cast<CSimpleFrame*>(CScriptObject::GetScriptObjectByName(parentName, parentType));

        if (!parent) {
            status->Add(STATUS_WARNING, "Couldn't find frame parent: %s", parentName);
        }
    }

    CSimpleFrame* frame = factoryNode->m_factory(parent);

    if (frame) {
        if (factoryNode->m_unique) {
            // TODO
            // Delete the factory node
            // sub_565030(factoryNode);
            // FrameXML::s_factoryHash.InternalDelete(factoryNode);
        }

        frame->PreLoadXML(node, status);
        frame->LoadXML(node, status);
        frame->PostLoadXML(node, status);

        return frame;
    } else {
        status->Add(STATUS_WARNING, "Unable to create frame type: %s", node->m_name.m_str);
        return nullptr;
    }
}

int32_t FrameXML_CreateFrames(const char* tocPath, const char* a2, MD5_CTX* md5, CStatus* status) {
    if (!status) {
        status = new CStatus;
    }

    char v15[260];
    v15[0] = 0;
    memset(v15 + 1, 0, 259);

    const char* v5 = tocPath;

    const char* v6 = SStrChrR(tocPath, '\\');

    if (v6) {
        uintptr_t v7 = v6 - tocPath + 1;
        uintptr_t v8 = v7;

        if (v7 < 259) {
            SStrCopy(v15, tocPath, v7 + 1);
            v15[v8] = 0;
        }
    }

    void* tocBuffer;
    size_t tocBytes;

    if (!SFile::Load(nullptr, tocPath, &tocBuffer, &tocBytes, 1, 1, nullptr)) {
        status->Add(STATUS_ERROR, "Couldn't open %s", a2);
        return 0;
    }

    MD5Update(md5, static_cast<unsigned char*>(tocBuffer), tocBytes);

    const char* tocData = static_cast<char*>(tocBuffer);

    // Advance past UTF-8 BOM if present
    if (*tocData == static_cast<char>(0xEF) && *(tocData + 1) == static_cast<char>(0xBB) && *(tocData + 2) == static_cast<char>(0xBF)) {
        tocData = tocData + 3;
    }

    CStatus v21;
    char tocLine[1024];
    char tocEntryPath[260];

    do {
        SStrTokenize(&tocData, tocLine, 1024, "\r\n", 0);

        if (!*tocLine) {
            break;
        }

        if (*tocLine == '#') {
            continue;
        }

        SStrCopy(tocEntryPath, v15, 260);
        SStrPack(tocEntryPath, tocLine, 260);

        size_t v10 = SStrLen(tocEntryPath);

        bool v11 = tocEntryPath[v10 - 1] == ' ';
        char* v12 = &tocEntryPath[v10];

        if (v11) {
            do {
                if (v12 <= tocEntryPath) {
                    break;
                }

                --v12;
            } while (*(v12 - 1) == ' ');
        }

        *v12 = 0;

        FrameXML_ProcessFile(tocEntryPath, a2, md5, &v21);

        // TODO
        // if (s_progressCallback && s_progressFiles < s_progressTotal) {
        //     ++s_progressFiles;
        //     v20 = s_progressTotal;
        //     v13 = (double)(unsigned int)s_progressFiles / (double)(unsigned int)s_progressTotal;
        //     s_progressCallback(LODWORD(v13), s_progressParam);
        //     v5 = a2;
        // }
    } while (*tocLine);

    // TODO
    // XMLTree_Cleanup();

    SFile::Unload(tocBuffer);

    // TODO
    // if (s_debugLevel > 0 || v25 > 0) {
    //     v21.Prepend(STATUS_INFO, "** Loading table of contents %s", v5);
    // }

    // TODO
    // status->Add(v21);

    return 1;
}

void FrameXML_FreeHashNodes() {
    FrameXML::s_nodeHash.Clear();
}

XMLTree* FrameXML_LoadXML(const char* filePath, MD5_CTX* md5, CStatus* status) {
    void* xmlBuffer;
    size_t xmlBytes;

    if (SFile::Load(nullptr, filePath, &xmlBuffer, &xmlBytes, 0, 1, nullptr)) {
        MD5Update(md5, static_cast<unsigned char*>(xmlBuffer), xmlBytes);

        XMLTree* tree = XMLTree_Load(static_cast<char*>(xmlBuffer), xmlBytes);

        if (!tree) {
            status->Add(STATUS_ERROR, "Couldn't parse XML in %s", filePath);
        }

        SFile::Unload(xmlBuffer);

        return tree;
    } else {
        status->Add(STATUS_ERROR, "Couldn't open %s", filePath);

        return nullptr;
    }
}

int32_t FrameXML_ProcessFile(const char* filePath, const char* a2, MD5_CTX* md5, CStatus* status) {
    const char* v5 = filePath;

    char v25[260];
    char* i;
    char* j;

    // Deal with relative file paths
    if (SStrStr(filePath, "..")) {
        SStrCopy(v25, filePath, 260);

        for (i = (char*)SStrStr(v25, ".."); i; i = (char*)SStrStr(v25, "..")) {
            char v7 = *(i - 1);

            if (v7 != '\\' && v7 != '/') {
                break;
            }

            char v8 = i[2];

            if (v8 != '\\' && v8 != '/') {
                break;
            }

            for (j = i - 2; j >= v25; --j) {
                if (*j == '\\') {
                    break;
                }

                if (*j == '/') {
                    break;
                }
            }

            SStrCopy(j + 1, i + 3, 0x7FFFFFFF);
        }

        filePath = v25;
        v5 = v25;
    }

    const char* v10 = SStrChrR(v5, '.');

    // If file ends in .lua, execute it and return
    if (v10 && !SStrCmpI(v10, ".lua", 0x7FFFFFFFu)) {
        return FrameScript_ExecuteFile(v5, a2, md5, status);
    }

    // Assume all other files are xml
    XMLTree* tree = FrameXML_LoadXML(v5, md5, status);

    if (!tree) {
        return 0;
    }

    CStatus unkStatus;

    XMLNode* node = XMLTree_GetRoot(tree)->m_child;

    char v26[271];
    char v27[264];

    // TODO
    // Should come from some kind of Lua headers
    const char* lua_tainted = nullptr;

    while (node) {
        // <Include>
        if (!SStrCmpI(node->GetName(), "Include")) {
            auto fileAttr = node->GetAttributeByName("file");

            if (fileAttr) {
                const char* v15 = SStrChrR(v5, 92);

                if (v15) {
                    uint32_t v13 = v15 - v5 + 1;

                    if (v13 < 260 ) {
                        SStrCopy(v27, v5, 260);
                        v27[v13] = 0;
                        SStrPack(v27, fileAttr, 260);
                    }
                } else {
                    SStrCopy(v27, fileAttr, 260);
                }

                FrameXML_ProcessFile(v27, a2, md5, &unkStatus);
            } else {
                unkStatus.Add(STATUS_ERROR, "Element 'Include' without file attribute");
            }

        // <Script>
        } else if (!SStrCmpI(node->GetName(), "Script")) {
            auto fileAttr = node->GetAttributeByName("file");

            if (fileAttr) {
                const char* v17 = SStrChrR(v5, 92);

                if (v17) {
                    uint32_t v18 = v17 - v5 + 1;

                    if (v18 < 260) {
                        SStrCopy(v27, filePath, 260);
                        v27[v18] = 0;
                        SStrPack(v27, fileAttr, 260);
                    }

                    v5 = filePath;
                } else {
                    SStrCopy(v27, fileAttr, 260);
                }

                FrameScript_ExecuteFile(v27, a2, md5, &unkStatus);
            }

            char* v19 = node->m_body;

            if (v19 && *v19) {
                SStrPrintf(v26, 271, "%s:<Scripts>", v5);
                FrameScript_Execute(v19, v26, lua_tainted);
            }

        // <Font>
        } else if (!SStrCmpI(node->GetName(), "Font")) {
            auto nameAttr = node->GetAttributeByName("name");

            if (nameAttr && *nameAttr) {
                CSimpleFont* font = CSimpleFont::GetFont(nameAttr, 1);
                font->LoadXML(node, status);
            } else {
                unkStatus.Add(STATUS_WARNING, "Unnamed font node at top level");
            }

        // Everything else (frame nodes)
        } else {
            auto virtualAttr = node->GetAttributeByName("virtual");

            if (virtualAttr && !SStrCmpI(virtualAttr, "true")) {
                auto nameAttr = node->GetAttributeByName("name");

                if (nameAttr && *nameAttr) {
                    FrameXML_StoreHashNode(node, nameAttr, lua_tainted, &unkStatus);
                } else {
                    unkStatus.Add(STATUS_WARNING, "Unnamed virtual node at top level");
                }
            } else {
                FrameXML_CreateFrame(node, nullptr, &unkStatus);
                CLayoutFrame::ResizePending();
            }
        }

        node = node->m_next;
    }

    XMLTree_Free(tree);

    // TODO
    // if (s_debugLevel > 0 || v34 > 0) {
    //     unkStatus.Prepend(STATUS_INFO, "++ Loading file %s", v5);
    // }

    // TODO
    // status->Unk8(unkStatus);

    return 1;
}

int32_t FrameXML_RegisterFactory(const char* type, CSimpleFrame* (*factory)(CSimpleFrame*), bool a3) {
    if (FrameXML::s_factoryHash.Ptr(type)) {
        // Already registered
        return 0;
    }

    FrameFactoryNode* node = FrameXML::s_factoryHash.New(type, 0, 0);

    node->m_factory = factory;
    node->m_unique = a3;

    return 1;
}

void FrameXML_RegisterDefault() {
    FrameXML_RegisterFactory("Button", &Create_SimpleButton, 0);
    FrameXML_RegisterFactory("CheckButton", &Create_SimpleCheckButton, 0);
    FrameXML_RegisterFactory("EditBox", &Create_SimpleEditBox, 0);
    FrameXML_RegisterFactory("Frame", &Create_SimpleFrame, 0);
    FrameXML_RegisterFactory("MessageFrame", &Create_SimpleMessageFrame, 0);
    FrameXML_RegisterFactory("Model", &Create_SimpleModel, 0);
    FrameXML_RegisterFactory("ScrollFrame", &Create_SimpleScrollFrame, 0);
    FrameXML_RegisterFactory("ScrollingMessageFrame", &Create_SimpleScrollingMessageFrame, 0);
    FrameXML_RegisterFactory("Slider", &Create_SimpleSlider, 0);
    FrameXML_RegisterFactory("SimpleHTML", &Create_SimpleHTML, 0);
    FrameXML_RegisterFactory("StatusBar", &Create_SimpleStatusBar, 0);
    FrameXML_RegisterFactory("ColorSelect", &Create_SimpleColorSelect, 0);
    FrameXML_RegisterFactory("MovieFrame", &Create_SimpleMovieFrame, 0);
}

void FrameXML_ReleaseHashNode(const char* name) {
    HashedNode* hashedNode = FrameXML::s_nodeHash.Ptr(name);
    hashedNode->locked = false;
}

void FrameXML_StoreHashNode(XMLNode* node, const char* name, const char* a3, CStatus* status) {
    HashedNode* hashed = FrameXML::s_nodeHash.Ptr(name);

    if (hashed) {
        if (!hashed->tainted || a3) {
            status->Add(STATUS_WARNING, "Virtual object named %s already exists", name);
            return;
        }

        XMLNode *v6 = hashed->node;

        if (v6) {
            delete v6;

            // TODO
            // CDataAllocator::PutData((int)XMLNode::s_XMLNodeHeap, v6, 0, 0);
        }
    } else {
        HashedNode* hashed = FrameXML::s_nodeHash.New(name, 0, 0);

        hashed->node = node;
        hashed->tainted = a3;
    }

    // TODO
    // sub_814650(a1);

    if (FrameXML::s_debugLevel > 0) {
        status->Add(STATUS_INFO, "-- Added virtual frame %s", name);
    }
}
