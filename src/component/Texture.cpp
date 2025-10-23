#include "component/Texture.hpp"
#include "async/AsyncFile.hpp"
#include "async/AsyncFileRead.hpp"
#include "gx/Blp.hpp"
#include "gx/Texture.hpp"
#include "util/SFile.hpp"
#include <common/ObjectAlloc.hpp>

TSHashTable<CACHEENTRY, HASHKEY_NONE> s_cacheTable;
HASHKEY_NONE s_cacheKey;
uint32_t* s_entryHeap;

void LoadSuccessCallback(void* handle) {
    auto entry = static_cast<CACHEENTRY*>(handle);

    AsyncFileReadDestroyObject(entry->m_asyncObject);
    entry->m_asyncObject = nullptr;

    auto& header = *static_cast<BLPHeader*>(entry->m_data);
    // TODO CBLPFile::ValidateHeader(header);

    auto& info = entry->m_info;
    info.width = header.width;
    info.height = header.height;
    info.alphaSize = header.alphaSize;
    info.opaque = header.alphaSize == 0;
    info.mipCount = TextureCalcMipCount(info.width, info.height);
}

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
    this->m_data = nullptr;
    this->m_size = 0;
    this->m_missing = 0;
}

void CACHEENTRY::AddRef() {
    this->m_refCount++;
}

TCTEXTUREINFO& CACHEENTRY::Info() {
    return this->m_info;
}

bool CACHEENTRY::IsLoading() {
    return !this->m_missing && this->m_info.width == 0 && this->m_asyncObject;
}

bool CACHEENTRY::IsMissing() {
    return this->m_missing;
}

int32_t CACHEENTRY::LoadTexture() {
    SFile* file;
    if (!SFile::OpenEx(nullptr, this->m_fileName, 0x0, &file)) {
        // TODO discover file type + pick alternate filename
    }

    if (!file) {
        this->m_missing = 1;

        return 0;
    }

    this->m_asyncObject = AsyncFileReadAllocObject();
    this->m_asyncObject->userArg = this;
    this->m_asyncObject->userPostloadCallback = &LoadSuccessCallback;
    this->m_asyncObject->file = file;
    this->m_asyncObject->size = SFile::GetFileSize(file, nullptr);
    this->m_asyncObject->priority = -126;

    this->m_size = this->m_asyncObject->size;
    this->m_data = STORM_ALLOC(this->m_size);

    this->m_asyncObject->buffer = this->m_data;

    AsyncFileReadObject(this->m_asyncObject, 0);

    return 1;
}

bool CACHEENTRY::NeedsLoad() {
    return !this->m_data;
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

int32_t TextureCacheGetInfo(void* handle, TCTEXTUREINFO& info, int32_t force) {
    auto entry = static_cast<CACHEENTRY*>(handle);

    if (!entry) {
        return 0;
    }

    if (!entry->IsMissing()) {
        if (entry->NeedsLoad()) {
            entry->LoadTexture();
        }

        if (entry->IsLoading()) {
            if (force) {
                AsyncFileReadWait(entry->m_asyncObject);
            } else {
                // TODO increase streaming priority

                return 0;
            }
        }
    }

    info = entry->Info();

    return 1;
}

uint8_t* TextureCacheGetMip(void* handle, uint32_t mipLevel) {
    auto entry = static_cast<CACHEENTRY*>(handle);

    if (!entry || entry->IsMissing() || !entry->m_data) {
        return nullptr;
    }

    if (mipLevel >= entry->Info().mipCount) {
        return nullptr;
    }

    auto blpHeader = static_cast<BLPHeader*>(entry->m_data);
    auto mipOffset = blpHeader->mipOffsets[mipLevel];

    return static_cast<uint8_t*>(entry->m_data) + mipOffset;
}

BlpPalPixel* TextureCacheGetPal(void* handle) {
    auto entry = static_cast<CACHEENTRY*>(handle);

    if (entry->IsMissing() || !entry->m_data) {
        return nullptr;
    }

    auto blpHeader = static_cast<BLPHeader*>(entry->m_data);

    if (blpHeader->colorEncoding != COLOR_PAL) {
        return nullptr;
    }

    return blpHeader->extended.palette;
}

int32_t TextureCacheHasMips(void* handle) {
    auto entry = static_cast<CACHEENTRY*>(handle);
    return entry && entry->m_data && !entry->IsMissing();
}
