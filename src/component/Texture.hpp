#ifndef COMPONENT_TEXTURE_HPP
#define COMPONENT_TEXTURE_HPP

#include <storm/Hash.hpp>

class CAsyncObject;

struct TCTEXTUREINFO {
    uint16_t width;
    uint16_t height;
    uint32_t mipCount : 8;
    uint32_t alphaSize: 8;
    uint32_t opaque : 1;
    uint32_t pad : 15;
};

class CACHEENTRY : public TSHashObject<CACHEENTRY, HASHKEY_NONE> {
    public:
        // Member variables
        CAsyncObject* m_asyncObject = nullptr;
        char m_fileName[128];
        uint32_t m_refCount = 0;
        uint32_t m_memHandle = 0;

        // Member functions
        void AddRef();
};

void* TextureCacheCreateTexture(const char* fileName);

void TextureCacheDestroyTexture(void* texture);

#endif
