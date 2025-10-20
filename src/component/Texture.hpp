#ifndef COMPONENT_TEXTURE_HPP
#define COMPONENT_TEXTURE_HPP

#include <storm/Hash.hpp>

class CAsyncObject;

struct TCTEXTUREINFO {
    uint16_t width;
    uint16_t height;
    uint32_t mipCount   : 8;
    uint32_t alphaSize  : 8;
    uint32_t opaque     : 1;
    uint32_t pad        : 15;

    TCTEXTUREINFO();
};

class CACHEENTRY : public TSHashObject<CACHEENTRY, HASHKEY_NONE> {
    public:
        // Member variables
        CAsyncObject* m_asyncObject;
        TCTEXTUREINFO m_info;
        char m_fileName[128];
        uint32_t m_refCount;
        uint32_t m_memHandle;
        uint32_t m_size     : 20;
        uint32_t m_loaded   : 1;

        // Member functions
        CACHEENTRY();
        void AddRef();
};

void* TextureCacheCreateTexture(const char* fileName);

void TextureCacheDestroyTexture(void* texture);

int32_t TextureCacheGetInfo(void* texture, TCTEXTUREINFO& info, int32_t a3);

#endif
