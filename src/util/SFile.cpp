#include "util/SFile.hpp"
#include <cstring>
#include <limits>
#include <storm/Memory.hpp>
#include <storm/String.hpp>

static char s_basepath[STORM_MAX_PATH] = {0};
static char s_datapath[STORM_MAX_PATH] = {0};

// TODO Proper implementation
int32_t SFile::Close(SFile* file) {
    delete file->m_filename;

    file->m_stream->close();
    delete file->m_stream;

    delete file;

    return 1;
}

// TODO Proper implementation
size_t SFile::GetFileSize(SFile* file, size_t* filesizeHigh) {
    return file->m_size;
}

int32_t SFile::IsStreamingMode() {
    // TODO
    return 0;
}

// TODO Proper implementation
int32_t SFile::Load(SArchive* archive, const char* filename, void** buffer, size_t* bytes, size_t extraBytes, uint32_t flags, SOVERLAPPED* overlapped) {
    auto pathLen = SStrLen(filename);
    char path[STORM_MAX_PATH];
    SStrCopy(path, filename, sizeof(path));

    for (int32_t i = 0; i < pathLen; ++i) {
        if (path[i] == '\\') {
            path[i] = '/';
        }
    }

    std::ifstream file (path, std::ios::in | std::ios::binary | std::ios::ate);
    size_t size;
    char* data;

    if (file.is_open()) {
        size = static_cast<size_t>(file.tellg());

        if (bytes) {
            *bytes = size;
        }

        data = new char[size + extraBytes];

        file.seekg(0, std::ios::beg);
        file.read(data, size);
        file.close();

        if (extraBytes) {
            memset(data + size, 0, extraBytes);
        }

        *buffer = data;

        return 1;
    } else {
        return 0;
    }
}

int32_t SFile::Open(const char* filename, SFile** file) {
    return SFile::OpenEx(nullptr, filename, 0, file);
}

// TODO Proper implementation
int32_t SFile::OpenEx(SArchive* archive, const char* filename, uint32_t flags, SFile** file) {
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
    file->m_stream->read((char*)buffer, bytestoread);

    if (bytesread) {
        *bytesread = file->m_stream->gcount();
    }

    return 1;
}

int32_t SFile::Unload(void* ptr) {
    SMemFree(ptr, __FILE__, __LINE__, 0);
    return 1;
}

int32_t SFile::SetBasePath(const char* path) {
    SStrCopy(s_basepath, path, STORM_MAX_PATH);

    if (s_basepath != '\0') {
        auto len = SStrLen(s_basepath);
        if (s_basepath[len-1] != '\\') {
            SStrPack(s_basepath, "\\", STORM_MAX_PATH);
        }
    }

    // TODO

    // SFileSetBasePath(path);

    return 1;
}

int32_t SFile::SetDataPath(const char* path) {
    SStrCopy(s_datapath, path, STORM_MAX_PATH);

    if (s_datapath != '\0') {
        auto len = SStrLen(s_datapath);
        if (s_basepath[len-1] != '\\') {
            SStrPack(s_datapath, "\\", STORM_MAX_PATH);
        }
    }

    return 1;
}

int32_t SFile::GetBasePath(char* buffer, size_t bufferchars) {
    SStrCopy(buffer, s_basepath, bufferchars);
    return 1;
}

int32_t SFile::GetDataPath(char* buffer, size_t bufferchars) {
    SStrCopy(buffer, s_datapath, bufferchars);
}
