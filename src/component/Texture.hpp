#ifndef COMPONENT_TEXTURE_HPP
#define COMPONENT_TEXTURE_HPP

#include <storm/Hash.hpp>

class CAsyncObject;

struct BlpPalPixel;

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
        void* m_data;
        uint32_t m_size     : 20;
        uint32_t m_missing  : 1;

        // Member functions
        CACHEENTRY();
        void AddRef();
        TCTEXTUREINFO& Info();
        bool IsLoading();
        bool IsMissing();
        int32_t LoadTexture();
        bool NeedsLoad();
};

void* TextureCacheCreateTexture(const char* fileName);

void TextureCacheDestroyTexture(void* texture);

int32_t TextureCacheGetInfo(void* handle, TCTEXTUREINFO& info, int32_t a3);

uint8_t* TextureCacheGetMip(void* handle, uint32_t mipLevel);

BlpPalPixel* TextureCacheGetPal(void* handle);

int32_t TextureCacheHasMips(void* handle);

#endif
