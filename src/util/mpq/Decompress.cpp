#include "util/mpq/Decompress.hpp"

#include <bzlib.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <vector>

#include "adpcm.h"
#include "huff.h"
#include "pklib.h"
#include "sparse.h"

namespace MpqCompression {
namespace {
    constexpr uint8_t MPQ_COMP_HUFFMANN = 0x01;
    constexpr uint8_t MPQ_COMP_ZLIB = 0x02;
    constexpr uint8_t MPQ_COMP_PKWARE = 0x08;
    constexpr uint8_t MPQ_COMP_BZIP2 = 0x10;
    constexpr uint8_t MPQ_COMP_SPARSE = 0x20;
    constexpr uint8_t MPQ_COMP_ADPCM_MONO = 0x40;
    constexpr uint8_t MPQ_COMP_ADPCM_STEREO = 0x80;
    constexpr uint8_t MPQ_COMP_LZMA = 0x12;

    struct MpqDataInfo {
        unsigned char* in;
        unsigned char* inEnd;
        unsigned char* out;
        unsigned char* outEnd;
    };

    unsigned int ReadInputData(char* buf, unsigned int* size, void* param) {
        MpqDataInfo* info = static_cast<MpqDataInfo*>(param);
        unsigned int maxAvail = static_cast<unsigned int>(info->inEnd - info->in);
        unsigned int toRead = *size;
        if (toRead > maxAvail) {
            toRead = maxAvail;
        }
        std::memcpy(buf, info->in, toRead);
        info->in += toRead;
        return toRead;
    }

    void WriteOutputData(char* buf, unsigned int* size, void* param) {
        MpqDataInfo* info = static_cast<MpqDataInfo*>(param);
        unsigned int maxWrite = static_cast<unsigned int>(info->outEnd - info->out);
        unsigned int toWrite = *size;
        if (toWrite > maxWrite) {
            toWrite = maxWrite;
        }
        std::memcpy(info->out, buf, toWrite);
        info->out += toWrite;
    }

    int DecompressZlib(void* out, int* outSize, void* in, int inSize) {
        z_stream z = {};
        z.next_in = reinterpret_cast<Bytef*>(in);
        z.avail_in = static_cast<uInt>(inSize);
        z.total_in = inSize;
        z.next_out = reinterpret_cast<Bytef*>(out);
        z.avail_out = static_cast<uInt>(*outSize);
        z.total_out = 0;
        z.zalloc = nullptr;
        z.zfree = nullptr;

        int result = inflateInit(&z);
        if (result == Z_OK) {
            result = inflate(&z, Z_FINISH);
            *outSize = static_cast<int>(z.total_out);
            inflateEnd(&z);
        }

        return result >= Z_OK;
    }

    int DecompressBzip2(void* out, int* outSize, void* in, int inSize) {
        bz_stream strm = {};
        strm.next_in = reinterpret_cast<char*>(in);
        strm.avail_in = inSize;
        strm.next_out = reinterpret_cast<char*>(out);
        strm.avail_out = *outSize;
        strm.bzalloc = nullptr;
        strm.bzfree = nullptr;
        strm.opaque = nullptr;

        int result = BZ2_bzDecompressInit(&strm, 0, 0);
        if (result == BZ_OK) {
            result = BZ2_bzDecompress(&strm);
            *outSize = strm.total_out_lo32;
            BZ2_bzDecompressEnd(&strm);
        }

        return result >= BZ_OK;
    }

    int DecompressPKLIB(void* out, int* outSize, void* in, int inSize) {
        std::vector<char> work(EXP_BUFFER_SIZE);
        std::memset(work.data(), 0, work.size());

        MpqDataInfo info = {
            static_cast<unsigned char*>(in),
            static_cast<unsigned char*>(in) + inSize,
            static_cast<unsigned char*>(out),
            static_cast<unsigned char*>(out) + *outSize
        };

        unsigned int result = explode(ReadInputData, WriteOutputData, work.data(), &info);
        *outSize = static_cast<int>(info.out - static_cast<unsigned char*>(out));
        return result == CMP_NO_ERROR ? 1 : 0;
    }

    int DecompressHuff(void* out, int* outSize, void* in, int inSize) {
        THuffmannTree tree(false);
        TInputStream stream(in, static_cast<size_t>(inSize));
        unsigned int result = tree.Decompress(out, static_cast<unsigned int>(*outSize), &stream);
        *outSize = static_cast<int>(result);
        return result != 0;
    }

    int DecompressAdpcmMono(void* out, int* outSize, void* in, int inSize) {
        int result = DecompressADPCM(out, *outSize, in, inSize, 1);
        *outSize = result;
        return result > 0;
    }

    int DecompressAdpcmStereo(void* out, int* outSize, void* in, int inSize) {
        int result = DecompressADPCM(out, *outSize, in, inSize, 2);
        *outSize = result;
        return result > 0;
    }

    int DecompressSparseWrapper(void* out, int* outSize, void* in, int inSize) {
        return DecompressSparse(out, outSize, in, inSize);
    }

    struct DecompressEntry {
        uint8_t mask;
        int (*fn)(void* out, int* outSize, void* in, int inSize);
    };

    constexpr DecompressEntry kDecompressTable[] = {
        {MPQ_COMP_BZIP2, DecompressBzip2},
        {MPQ_COMP_PKWARE, DecompressPKLIB},
        {MPQ_COMP_ZLIB, DecompressZlib},
        {MPQ_COMP_HUFFMANN, DecompressHuff},
        {MPQ_COMP_ADPCM_STEREO, DecompressAdpcmStereo},
        {MPQ_COMP_ADPCM_MONO, DecompressAdpcmMono},
        {MPQ_COMP_SPARSE, DecompressSparseWrapper}
    };

    constexpr uint8_t kKnownMask =
        MPQ_COMP_HUFFMANN |
        MPQ_COMP_ZLIB |
        MPQ_COMP_PKWARE |
        MPQ_COMP_BZIP2 |
        MPQ_COMP_SPARSE |
        MPQ_COMP_ADPCM_MONO |
        MPQ_COMP_ADPCM_STEREO;
}

bool DecompressBlock(const uint8_t* in, size_t inSize, uint8_t* out, size_t outSize) {
    if (inSize == outSize) {
        std::memcpy(out, in, outSize);
        return true;
    }

    if (inSize < 1) {
        return false;
    }

    if (inSize > static_cast<size_t>(std::numeric_limits<int>::max())) {
        return false;
    }

    if (outSize > static_cast<size_t>(std::numeric_limits<int>::max())) {
        return false;
    }

    uint8_t type = in[0];
    const uint8_t* payload = in + 1;
    size_t payloadSize = inSize - 1;

    if (type == 0) {
        if (payloadSize != outSize) {
            return false;
        }
        std::memcpy(out, payload, outSize);
        return true;
    }

    if (type == MPQ_COMP_LZMA) {
        return false;
    }

    if ((type & ~kKnownMask) != 0) {
        return false;
    }

    int steps = 0;
    for (const auto& entry : kDecompressTable) {
        if (type & entry.mask) {
            ++steps;
        }
    }

    if (steps == 0) {
        return false;
    }

    std::vector<uint8_t> work;
    uint8_t* workBuf = nullptr;
    if (steps > 1) {
        work.resize(outSize);
        workBuf = work.data();
    }

    const uint8_t* currentInput = payload;
    size_t currentSize = payloadSize;
    int remaining = steps - 1;

    for (const auto& entry : kDecompressTable) {
        if ((type & entry.mask) == 0) {
            continue;
        }

        uint8_t* output = (remaining & 1) ? workBuf : out;
        int outputSize = static_cast<int>(outSize);
        int inputSize = static_cast<int>(currentSize);

        if (!entry.fn(output, &outputSize, const_cast<uint8_t*>(currentInput), inputSize)) {
            return false;
        }

        if (outputSize <= 0) {
            return false;
        }

        currentInput = output;
        currentSize = static_cast<size_t>(outputSize);
        --remaining;
    }

    return currentSize == outSize;
}
}
