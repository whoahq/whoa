#include "util/SFile.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
#include <storm/Memory.hpp>
#include <storm/String.hpp>
#include "util/Mpq.hpp"

// TODO Proper implementation
int32_t SFile::Close(SFile* file) {
    delete file->m_filename;

    if (file->m_stream) {
        file->m_stream->close();
        delete file->m_stream;
        file->m_stream = nullptr;
    }

    if (file->m_buffer) {
        delete[] file->m_buffer;
        file->m_buffer = nullptr;
    }

    delete file;

    return 1;
}

// TODO Proper implementation
int32_t SFile::FileExists(const char* filename) {
    if (Mpq::FileExists(filename)) {
        return 1;
    }

    auto pathLen = SStrLen(filename);
    char path[STORM_MAX_PATH];
    SStrCopy(path, filename, sizeof(path));

    for (int32_t i = 0; i < pathLen; ++i) {
        if (path[i] == '\\') {
            path[i] = '/';
        }
    }

    std::ifstream f(path);
    return f.good();
}

// TODO Proper implementation
size_t SFile::GetFileSize(SFile* file, size_t* filesizeHigh) {
    return file->m_size;
}

int32_t SFile::IsStreamingMode() {
    // TODO
    return 0;
}

int32_t SFile::IsStreamingTrial() {
    // TODO
    return 0;
}

// TODO Proper implementation
int32_t SFile::Load(SArchive* archive, const char* filename, void** buffer, size_t* bytes, size_t extraBytes, uint32_t flags, SOVERLAPPED* overlapped) {
    SFile* file = nullptr;

    if (!SFile::OpenEx(archive, filename, flags, &file) || !file) {
        return 0;
    }

    size_t size = static_cast<size_t>(SFile::GetFileSize(file, nullptr));

    if (bytes) {
        *bytes = size;
    }

    char* data = new char[size + extraBytes];
    size_t bytesRead = 0;

    int32_t result = SFile::Read(file, data, size, &bytesRead, nullptr, nullptr);
    SFile::Close(file);

    if (!result) {
        delete[] data;
        return 0;
    }

    if (extraBytes) {
        memset(data + size, 0, extraBytes);
    }

    *buffer = data;

    return 1;
}

int32_t SFile::Open(const char* filename, SFile** file) {
    return SFile::OpenEx(nullptr, filename, 0, file);
}

// TODO Proper implementation
int32_t SFile::OpenEx(SArchive* archive, const char* filename, uint32_t flags, SFile** file) {
    if (!archive) {
        uint8_t* data = nullptr;
        size_t size = 0;

        if (Mpq::ReadFile(filename, &data, &size)) {
            SFile* fileptr = new SFile;

            fileptr->m_filename = strdup(filename);
            fileptr->m_stream = nullptr;
            fileptr->m_buffer = data;
            fileptr->m_offset = 0;
            fileptr->m_size = static_cast<std::streamsize>(size);

            *file = fileptr;

            return 1;
        }
    }

    auto pathLen = SStrLen(filename);
    char path[STORM_MAX_PATH];
    SStrCopy(path, filename, sizeof(path));

    for (int32_t i = 0; i < pathLen; ++i) {
        if (path[i] == '\\') {
            path[i] = '/';
        }
    }

    SFile* fileptr = new SFile;

    fileptr->m_filename = strdup(filename);
    fileptr->m_buffer = nullptr;
    fileptr->m_offset = 0;

    std::ifstream* stream = new std::ifstream(path, std::ios::in | std::ios::binary | std::ios::ate);

    if (!stream->is_open()) {
        *file = nullptr;
        return 0;
    }

    stream->seekg(0, std::ios::beg);

    stream->ignore(std::numeric_limits<std::streamsize>::max());
    std::streamsize size = stream->gcount();
    stream->clear();

    stream->seekg(0, std::ios::beg);

    fileptr->m_stream = stream;
    fileptr->m_size = size;

    *file = fileptr;

    return 1;
}

// TODO Proper implementation
int32_t SFile::Read(SFile* file, void* buffer, size_t bytestoread, size_t* bytesread, SOVERLAPPED* overlapped, TASYNCPARAMBLOCK* asyncparam) {
    if (file->m_buffer) {
        size_t available = static_cast<size_t>(file->m_size) - file->m_offset;
        size_t toRead = std::min(bytestoread, available);
        std::memcpy(buffer, file->m_buffer + file->m_offset, toRead);
        file->m_offset += toRead;

        if (bytesread) {
            *bytesread = toRead;
        }

        return 1;
    }

    file->m_stream->read(static_cast<char*>(buffer), bytestoread);

    if (bytesread) {
        *bytesread = file->m_stream->gcount();
    }

    return 1;
}

int32_t SFile::Unload(void* ptr) {
    SMemFree(ptr, __FILE__, __LINE__, 0);
    return 1;
}
