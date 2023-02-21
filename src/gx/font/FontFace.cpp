#include "gx/font/FontFace.hpp"
#include "gx/font/FaceData.hpp"
#include "util/SFile.hpp"
#include <storm/Error.hpp>
#include <storm/Hash.hpp>

TSHashTable<FACEDATA, HASHKEY_STRI> s_faceHash;

void FontFaceCloseHandle(HFACE handle) {
    STORM_ASSERT(handle);

    HandleClose(handle);

    FACEDATA* dataPtr = reinterpret_cast<FACEDATA*>(handle);

    if (dataPtr->m_refcount <= 1) {
        auto selfReference = dataPtr->selfReference;
        dataPtr->selfReference = nullptr;
        HandleClose(selfReference);
    }
}

FT_Face FontFaceGetFace(HFACE handle) {
    STORM_ASSERT(handle);

    return reinterpret_cast<FACEDATA*>(handle)->face;
}

const char* FontFaceGetFontName(HFACE handle) {
    STORM_ASSERT(handle);

    return reinterpret_cast<FACEDATA*>(handle)->m_key.m_str;
}

HFACE FontFaceGetHandle(const char* fileName, FT_Library library) {
    if (!library || !fileName || !*fileName) {
        return nullptr;
    }

    if (auto existing = s_faceHash.Ptr(fileName)) {
        return HandleDuplicate(existing->selfReference);
    }

    void* data = nullptr;
    size_t size;

    if (!SFile::Load(nullptr, fileName, &data, &size, 0, 0x3, nullptr) || !data) {
        if (data) {
           SFile::Unload(data);
        }

        return nullptr;
    }

    FT_Face theFace;

    if (FT_New_Memory_Face(library, (FT_Byte*)data, size, 0, &theFace) != FT_Err_Ok || !theFace) {
        if (data) {
           SFile::Unload(data);
        }

        return nullptr;
    }

    if (FT_Select_Charmap(theFace, ft_encoding_unicode) != FT_Err_Ok) {
        if (data) {
           SFile::Unload(data);
        }

        return nullptr;
    }

    auto faceData = s_faceHash.New(fileName, 0, 0);
    auto handle = HandleCreate(faceData);

    faceData->data = data;
    faceData->face = theFace;
    faceData->selfReference = handle;

    return HandleDuplicate(handle);
}
