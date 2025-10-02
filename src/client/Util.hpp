#ifndef CLIENT_UTIL_HPP
#define CLIENT_UTIL_HPP

#include <cstdint>

void ChecksumExecutables(const uint8_t* challenge, uint32_t challengeLength, uint8_t* checksum);

#endif
