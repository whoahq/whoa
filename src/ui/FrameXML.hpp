#ifndef UI_FRAME_XML_HPP
#define UI_FRAME_XML_HPP

#include <common/MD5.hpp>
#include <storm/Hash.hpp>

class CSimpleFrame;
class CStatus;
class XMLNode;
class XMLTree;

class FrameFactoryNode : public TSHashObject<FrameFactoryNode, HASHKEY_STRI> {
    public:
        CSimpleFrame* (*m_factory)(CSimpleFrame*);
        uint32_t m_unique;

        FrameFactoryNode() : TSHashObject<FrameFactoryNode, HASHKEY_STRI>() {};
};

class HashedNode : public TSHashObject<HashedNode, HASHKEY_STRI> {
    public:
        XMLNode* node;
        const char* tainted;
        uint32_t locked;
};

namespace FrameXML {
    extern int32_t s_debugLevel;
    extern TSHashTable<FrameFactoryNode, HASHKEY_STRI> s_factoryHash;
    extern TSHashTable<HashedNode, HASHKEY_STRI> s_nodeHash;
}

XMLNode* FrameXML_AcquireHashNode(const char*, const char*&, bool&);

int32_t FrameXML_CheckSignature(const char*, const char*, const unsigned char*, unsigned char*);

CSimpleFrame* FrameXML_CreateFrame(XMLNode*, CSimpleFrame*, CStatus*);

int32_t FrameXML_CreateFrames(const char*, const char*, MD5_CTX*, CStatus*);

void FrameXML_FreeHashNodes(void);

XMLTree* FrameXML_LoadXML(const char*, MD5_CTX*, CStatus*);

int32_t FrameXML_ProcessFile(const char*, const char*, MD5_CTX*, CStatus*);

void FrameXML_RegisterDefault(void);

int32_t FrameXML_RegisterFactory(const char*, CSimpleFrame* (*)(CSimpleFrame*), bool);

void FrameXML_ReleaseHashNode(const char*);

void FrameXML_StoreHashNode(XMLNode*, const char*, const char*, CStatus*);

#endif
