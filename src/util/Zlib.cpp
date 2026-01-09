#include "util/Zlib.hpp"
#include <storm/Error.hpp>
#include <storm/Memory.hpp>
#include <zlib.h>

int zlib_uncompress(Bytef* dest, uInt* destLen, const Bytef* source, uInt sourceLen) {
    char buffer[47000];
    ZlibAllocBufferHeader header = { buffer, sizeof(buffer), buffer };

    z_stream strm;
    strm.next_in = const_cast<Bytef*>(source);
    strm.avail_in = sourceLen;
    strm.next_out = dest;
    strm.avail_out = *destLen;
    strm.zalloc = &ZlibAlloc;
    strm.zfree = &ZlibFree;
    strm.opaque = &header;

    auto err = inflateInit(&strm);
    if (err != Z_OK) {
        return err;
    }

    err = inflate(&strm, Z_FINISH);
    if (err == Z_STREAM_END) {
        *destLen = strm.total_out;
        return inflateEnd(&strm);
    }

    inflateEnd(&strm);
    return err ? err : Z_BUF_ERROR;
}

void* ZlibAlloc(void* opaque, uint32_t items, uint32_t size) {
    auto buffer = static_cast<ZlibAllocBufferHeader*>(opaque);
    STORM_ASSERT(buffer);

    uint32_t total = items * size;

    // Align to 4 bytes
    if ((total & 3) != 0) {
        total = (total & ~3) + 4;
    }

    auto ptr = buffer->nextPtr;
    size_t used = buffer->nextPtr - buffer->bufStart;
    size_t remaining = buffer->bufSize - used;

    // Fall back to heap if stack allocated buffer is exhausted
    if (total >= remaining) {
        return STORM_ALLOC(total);
    }

    buffer->nextPtr = ptr + total;
    return ptr;
}

void ZlibFree(void* opaque, void* ptr) {
    auto* buffer = static_cast<ZlibAllocBufferHeader*>(opaque);
    auto bufferStart = buffer->bufStart;
    auto bufferEnd = buffer->bufStart + buffer->bufSize;

    // Only free if pointer is outside of stack allocated buffer
    if (ptr < bufferStart || ptr >= bufferEnd) {
        STORM_FREE(ptr);
    }
}

int32_t ZlibDecompress(void* dest, uint32_t* destLen, const void* source, uint32_t sourceLen) {
    return zlib_uncompress(
        static_cast<Bytef*>(dest),
        destLen,
        static_cast<const Bytef*>(source),
        sourceLen
    );
}
