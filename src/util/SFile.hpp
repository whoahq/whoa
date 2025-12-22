#ifndef UTIL_S_FILE_HPP
#define UTIL_S_FILE_HPP

#include <cstddef>
#include <cstdint>
#include <fstream>

class SArchive;
struct SOVERLAPPED;
struct TASYNCPARAMBLOCK;

class SFile {
    public:
        // Static functions
        static int32_t Close(SFile*);
        static int32_t FileExists(const char* filename);
        static size_t GetFileSize(SFile*, size_t*);
        static int32_t IsStreamingMode();
        static int32_t IsStreamingTrial();
        static int32_t Load(SArchive*, const char*, void**, size_t*, size_t, uint32_t, SOVERLAPPED*);
        static int32_t Open(const char*, SFile**);
        static int32_t OpenEx(SArchive*, const char*, uint32_t, SFile**);
        static int32_t Read(SFile*, void*, size_t, size_t*, SOVERLAPPED*, TASYNCPARAMBLOCK*);
        static int32_t Unload(void*);

        // Member variables
        const char* m_filename;
        std::ifstream* m_stream; // TODO Proper implementation
        uint8_t* m_buffer;
        size_t m_offset;
        std::streamsize m_size; // TODO Proper implementation
};

#endif
