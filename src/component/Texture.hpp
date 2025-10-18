#ifndef COMPONENT_TEXTURE_HPP
#define COMPONENT_TEXTURE_HPP

#include <storm/Hash.hpp>

class CAsyncObject;

class CACHEENTRY : public TSHashObject<CACHEENTRY, HASHKEY_NONE> {
    public:
        // Member variables
        CAsyncObject* m_asyncObject = nullptr;
        char m_fileName[128];
        uint32_t m_refCount = 0;
        void* m_memHandle = nullptr;

};

CACHEENTRY* TextureCacheCreateTexture(const char* fileName);

void TextureCacheDestroyTexture(void* texture);

#endif
