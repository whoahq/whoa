#ifndef UTIL_ZLIB_HPP
#define UTIL_ZLIB_HPP

#include <cstdint>

struct ZlibAllocBufferHeader {
    char* nextPtr;
    uint32_t bufSize;
    char* bufStart;
};

void* ZlibAlloc(void* opaque, uint32_t items, uint32_t size);

int32_t ZlibDecompress(void* dest, uint32_t* destLen, const void* source, uint32_t sourceLen);

void ZlibFree(void* opaque, void* ptr);

#endif
