#include "net/srp/SRP6_Random.hpp"
#include <cstring>
#include <common/SHA1.hpp>

SRP6_Random::SRP6_Random(const void* seed, uint32_t seedLen) {
    SHA1_CONTEXT ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, static_cast<const uint8_t*>(seed), seedLen);
    SHA1_Final(reinterpret_cast<uint8_t*>(this->m_randkey1), &ctx);

    memcpy(this->m_randkey2, this->m_randkey1, sizeof(this->m_randkey2));

    memset(this->m_randpool, 0, sizeof(this->m_randpool));

    this->m_inpool = 0;
}
