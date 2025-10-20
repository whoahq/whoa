#include "component/Texture.hpp"
#include <common/ObjectAlloc.hpp>

TSHashTable<CACHEENTRY, HASHKEY_NONE> s_cacheTable;
HASHKEY_NONE s_cacheKey;
uint32_t* s_entryHeap;

TCTEXTUREINFO::TCTEXTUREINFO() {
    this->width = 0;
    this->height = 0;
    this->mipCount = 0;
    this->alphaSize = 0;
    this->opaque = 1;
}

CACHEENTRY::CACHEENTRY() {
    this->m_asyncObject = nullptr;
    this->m_fileName[0] = '\0';
    this->m_refCount = 0;
    this->m_memHandle = 0;
    this->bitsB0 = 0;
    this->m_loaded = 0;
}

void CACHEENTRY::AddRef() {
    this->m_refCount++;
}

CACHEENTRY* TextureCacheAllocEntry() {
    if (!s_entryHeap) {
        auto heapId = static_cast<uint32_t*>(SMemAlloc(sizeof(uint32_t), __FILE__, __LINE__, 0));
        *heapId = ObjectAllocAddHeap(sizeof(CACHEENTRY), 1024, "TCACHEENTRY", true);

        s_entryHeap = heapId;
    }

    uint32_t memHandle;
    void* mem;

    if (!ObjectAlloc(*s_entryHeap, &memHandle, &mem, false)) {
        return nullptr;
    }

    auto entry = new (mem) CACHEENTRY();
    entry->m_memHandle = memHandle;

    return entry;
}

void* TextureCacheCreateTexture(const char* fileName) {
    auto hashval = SStrHash(fileName);
    auto texture = s_cacheTable.Ptr(hashval, s_cacheKey);

    if (!texture) {
        texture = TextureCacheAllocEntry();

        s_cacheTable.Insert(texture, hashval, s_cacheKey);

        SStrCopy(texture->m_fileName, fileName, sizeof(texture->m_fileName));
    }

    texture->AddRef();

    return texture;
}

void TextureCacheDestroyTexture(void* texture) {
    // TODO
}
