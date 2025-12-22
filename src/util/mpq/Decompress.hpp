#pragma once

#include <cstddef>
#include <cstdint>

namespace MpqCompression {
    bool DecompressBlock(const uint8_t* in, size_t inSize, uint8_t* out, size_t outSize);
}
