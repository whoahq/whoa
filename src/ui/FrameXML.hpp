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

XMLNode* FrameXML_AcquireHashNode(const char* name, const char*& tainted, bool& locked);

int32_t FrameXML_CheckSignature(const char* tocPath, const char* a2, const unsigned char* key, unsigned char* digest);

CSimpleFrame* FrameXML_CreateFrame(XMLNode* node, CSimpleFrame* parent, CStatus* status);

int32_t FrameXML_CreateFrames(const char* tocPath, const char* a2, MD5_CTX* md5, CStatus* status);

void FrameXML_FreeHashNodes();

XMLTree* FrameXML_LoadXML(const char* filePath, MD5_CTX* md5, CStatus* status);

int32_t FrameXML_ProcessFile(const char* filePath, const char* a2, MD5_CTX* md5, CStatus* status);

void FrameXML_RegisterDefault();

int32_t FrameXML_RegisterFactory(const char* type, CSimpleFrame* (*factory)(CSimpleFrame*), bool);

void FrameXML_ReleaseHashNode(const char* name);

void FrameXML_StoreHashNode(XMLNode* node, const char* name, const char* a3, CStatus* status);

#endif
