#include "util/HMAC.hpp"
#include <cstring>
#include <common/SHA1.hpp>

#define HMAC_SHA1_IPAD 0x36
#define HMAC_SHA1_OPAD 0x5C
#define HMAC_SHA1_BLOCK_SIZE 64

void HMAC_SHA1(const uint8_t* key, uint32_t keyLen, const uint8_t* data, uint32_t dataLen, uint8_t* out) {
    uint8_t ibuf[HMAC_SHA1_BLOCK_SIZE], obuf[HMAC_SHA1_BLOCK_SIZE];

    for (uint32_t i = 0; i < keyLen; i++) {
        ibuf[i] = key[i] ^ HMAC_SHA1_IPAD;
        obuf[i] = key[i] ^ HMAC_SHA1_OPAD;
    }

    for (uint32_t i = keyLen; i < HMAC_SHA1_BLOCK_SIZE; i++) {
        ibuf[i] = HMAC_SHA1_IPAD;
        obuf[i] = HMAC_SHA1_OPAD;
    }

    SHA1_CONTEXT ctx;
    uint8_t idigest[SHA1_DIGEST_SIZE];

    // Inner

    SHA1_Init(&ctx);
    SHA1_Update(&ctx, ibuf, HMAC_SHA1_BLOCK_SIZE);
    SHA1_Update(&ctx, data, dataLen);
    SHA1_Final(idigest, &ctx);

    // Outer

    SHA1_Init(&ctx);
    SHA1_Update(&ctx, obuf, HMAC_SHA1_BLOCK_SIZE);
    SHA1_Update(&ctx, idigest, SHA1_DIGEST_SIZE);
    SHA1_Final(out, &ctx);
}
