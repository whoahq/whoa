#ifndef UTIL_MPQ_HPP
#define UTIL_MPQ_HPP

#include <cstddef>
#include <cstdint>

namespace Mpq {
    bool FileExists(const char* filename);
    bool ReadFile(const char* filename, uint8_t** data, size_t* size);
}

#endif
