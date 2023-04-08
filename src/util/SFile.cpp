#include "util/SFile.hpp"
#include <cstring>
#include <limits>
#include <StormLib.h>
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include "util/Filesystem.hpp"

// TODO Proper implementation
int32_t SFile::Close(SFile* file) {
    SFileCloseFile(file->m_file);
    delete file;
    return 1;
}

// TODO Proper implementation
size_t SFile::GetFileSize(SFile* file, size_t* filesizeHigh) {
    DWORD highPart = 0;
    DWORD lowPart = SFileGetFileSize(file->m_file, &highPart);

    if (filesizeHigh)
        *filesizeHigh = highPart;
    return lowPart;
}

int32_t SFile::IsStreamingMode() {
    // TODO
    return 0;
}

// TODO Proper implementation
int32_t SFile::Load(SArchive* archive, const char* filename, void** buffer, size_t* bytes, size_t extraBytes, uint32_t flags, SOVERLAPPED* overlapped) {
    auto pathLen = SStrLen(filename);

    char archivePath[STORM_MAX_PATH] = { 0 };
    char localPath[STORM_MAX_PATH] = { 0 };
    char subPath[STORM_MAX_PATH] = { 0 };

    bool containsPath = false;

    for (int32_t i = 0; i < pathLen; ++i) {
        if (filename[i] == '/') {
            containsPath = true;
#ifdef WHOA_SYSTEM_WIN
            localPath[i] = '\\';
#else
            localPath[i] = '/';
#endif
            archivePath[i] = '\\';
        } else if (filename[i] == '\\') {
            containsPath = true;
#ifdef WHOA_SYSTEM_WIN
            localPath[i] = '\\';
#else
            localPath[i] = '/';
#endif
            archivePath[i] = '\\';
        } else {
            localPath[i] = filename[i];
            archivePath[i] = filename[i];
        }
    }

    if (!containsPath) {
#ifdef WHOA_SYSTEM_WIN
        SStrCopy(subPath, "Data\\enGB\\", sizeof(subPath));
#else
        SStrCopy(subPath, "Data/enGB/", sizeof(subPath));
#endif
        strcat(subPath, filename);
    }


    HANDLE file;
    if (!SFileOpenFileEx(nullptr, localPath, SFILE_OPEN_LOCAL_FILE, &file)) {
        if (containsPath || !SFileOpenFileEx(nullptr, subPath, SFILE_OPEN_LOCAL_FILE, &file)) {
            if (!SFileOpenFileEx(g_mpqHandle, archivePath, SFILE_OPEN_FROM_MPQ, &file))
                return 0;
        }
    }

    DWORD highPart = 0;
    size_t size = SFileGetFileSize(file, &highPart);
    size |= (highPart << 32);

    if (bytes)
        *bytes = size;

    char* data = (char*) SMemAlloc(size + extraBytes, __FILE__, __LINE__, 0);

    SFileReadFile(file, data, size, &highPart, nullptr);

    if (extraBytes)
        memset(data + size, 0, extraBytes);

    if (buffer)
        *buffer = data;

    SFileCloseFile(file);

    return 1;
}

int32_t SFile::Open(const char* filename, SFile** file) {
    return SFile::OpenEx(nullptr, filename, 0, file);
}

// TODO Proper implementation
int32_t SFile::OpenEx(SArchive* archive, const char* filename, uint32_t flags, SFile** file) {
    auto pathLen = SStrLen(filename);

    char archivePath[STORM_MAX_PATH] = { 0 };
    char localPath[STORM_MAX_PATH] = { 0 };

    for (int32_t i = 0; i < pathLen; ++i) {
        if (filename[i] == '/') {
#ifdef  WHOA_SYSTEM_WIN
            localPath[i] = '\\';
#else
            localPath[i] = '/';
#endif
            archivePath[i] = '\\';
        } else if (filename[i] == '\\') {
#ifdef WHOA_SYSTEM_WIN
            localPath[i] = '\\';
#else
            localPath[i] = '/';
#endif
            archivePath[i] = '\\';
        } else {
            localPath[i] = filename[i];
            archivePath[i] = filename[i];
        }
    }

    HANDLE handle;
    bool local = true;
    if (!SFileOpenFileEx(nullptr, localPath, SFILE_OPEN_LOCAL_FILE, &handle)) {
        local = false;
        if (!SFileOpenFileEx(g_mpqHandle, archivePath, SFILE_OPEN_FROM_MPQ, &handle)) {
            *file = nullptr;
            return 0;
        }
    }

    SFile* fileptr = new SFile;
    fileptr->m_mpq = local ? nullptr : g_mpqHandle;
    fileptr->m_file = handle;
    *file = fileptr;

    return 1;
}

// TODO Proper implementation
int32_t SFile::Read(SFile* file, void* buffer, size_t bytestoread, size_t* bytesread, SOVERLAPPED* overlapped, TASYNCPARAMBLOCK* asyncparam) {
    DWORD read = 0;
    if (SFileReadFile(file->m_file, buffer, bytestoread, &read, nullptr)) {
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
