#ifndef UTIL_S_FILE_HPP
#define UTIL_S_FILE_HPP

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>

class SArchive;
struct SOVERLAPPED;
struct TASYNCPARAMBLOCK;
class Blizzard::File::StreamRecord;

class SFile {
    public:
        // Static functions
        static int32_t Close(SFile*);
        static size_t GetFileSize(SFile*, size_t*);
        static int32_t IsStreamingMode(void);
        static int32_t Load(SArchive*, const char*, void**, size_t*, size_t, uint32_t, SOVERLAPPED*);
        static int32_t Open(const char*, SFile**);
        static int32_t OpenEx(SArchive*, const char*, uint32_t, SFile**);
        static int32_t Read(SFile*, void*, size_t, size_t*, SOVERLAPPED*, TASYNCPARAMBLOCK*);
        static int32_t Unload(void*);
        static int32_t SetBasePath(const char* path);
        static int32_t SetDataPath(const char* path);
        static int32_t GetBasePath(char* path, size_t capacity);
        static int32_t GetDataPath(char* path, size_t capacity);

        // Member variables
        const char* m_filename;
        Blizzard::File::StreamRecord* m_stream; // TODO Proper implementation
        uint64_t m_size; // TODO Proper implementation
};

#endif
