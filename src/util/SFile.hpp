#ifndef UTIL_S_FILE_HPP
#define UTIL_S_FILE_HPP

#include <cstdint>
#include <cstdlib>

class SArchive;
struct SOVERLAPPED;
struct TASYNCPARAMBLOCK;

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

        // Member variables
        void* m_mpq;
        void* m_file;
};

#endif
