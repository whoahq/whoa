#include "util/SFile.hpp"
#include <cstring>
#include <limits>
#include <StormLib.h>
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include "util/Filesystem.hpp"

// TODO Proper implementation
int32_t SFile::Close(SFile* file) {
    SFileCloseFile(file->m_handle);
    delete file;
    return 1;
}

// TODO Proper implementation
uint32_t SFile::GetFileSize(SFile* file, uint32_t* filesizeHigh) {
    DWORD high = 0;
    DWORD low = SFileGetFileSize(file->m_handle, &high);

    if (filesizeHigh)
        *filesizeHigh = high;

    return low;
}

int32_t SFile::IsStreamingMode() {
    // TODO
    return 0;
}

// TODO Proper implementation
int32_t SFile::Load(SArchive* archive, const char* filename, void** buffer, size_t* bytes, size_t extraBytes, uint32_t flags, SOVERLAPPED* overlapped) {
    if (!buffer || !filename)
        return 0;

    *buffer = nullptr;
    if (bytes)
        *bytes = 0;

    SFile* file = nullptr;
    if (!SFile::OpenEx(nullptr, filename, 0, &file))
        return 0;

    uint32_t high = 0;
    uint64_t size = SFile::GetFileSize(file, &high);
    size |= ((uint64_t) high << 32);

    char* data = (char*) SMemAlloc(size + extraBytes, __FILE__, __LINE__, 0);

    if (!SFile::Read(file, data, size, nullptr, nullptr, nullptr)) {
        SMemFree(data, __FILE__, __LINE__, 0);
        SFile::Close(file);
        return 0;
    }

    if (extraBytes)
        memset(data + size, 0, extraBytes);

    if (bytes)
        *bytes = size;

    if (buffer)
        *buffer = data;

    SFile::Close(file);

    return 1;
}

int32_t SFile::Open(const char* filename, SFile** file) {
    return SFile::OpenEx(nullptr, filename, 0, file);
}

// TODO Proper implementation
int32_t SFile::OpenEx(SArchive* archive, const char* filename, uint32_t flags, SFile** file) {
    if (!file || !filename)
        return 0;

    *file = nullptr;

    char path[STORM_MAX_PATH];

    // Overflow protection
    if (SStrLen(filename) + 1 > sizeof(path))
        return 0;

    SStrCopy(path, filename, sizeof(path));
    OsFileToNativeSlashes(path);

    HANDLE handle;
    if (!SFileOpenFileEx(nullptr, path, SFILE_OPEN_LOCAL_FILE, &handle)) {
        OsFileToBackSlashes(path);
        if (!SFileOpenFileEx(g_mpqHandle, path, SFILE_OPEN_FROM_MPQ, &handle)) {
            return 0;
        }
    }

    *file = new SFile;
    (*file)->m_handle = handle;

    return 1;
}

// TODO Proper implementation
int32_t SFile::Read(SFile* file, void* buffer, size_t bytestoread, size_t* bytesread, SOVERLAPPED* overlapped, TASYNCPARAMBLOCK* asyncparam) {
    DWORD read = 0;
    if (SFileReadFile(file->m_handle, buffer, static_cast<DWORD>(bytestoread), &read, nullptr)) {
        if (bytesread)
            *bytesread = read;
        return 1;
    } else {
        if (bytesread)
            *bytesread = 0;
        return 0;
    }
}

int32_t SFile::Unload(void* ptr) {
    SMemFree(ptr, __FILE__, __LINE__, 0);
    return 1;
}
