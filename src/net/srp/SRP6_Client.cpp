#include "net/srp/SRP6_Client.hpp"
#include <cstring>

int32_t SRP6_Client::BeginAuthentication(const char* accountName, const char* password) {
    if (!accountName || !password) {
        return -1;
    }

    SHA1_Init(&this->ctx);

    SHA1_CONTEXT ctx;

    SHA1_Init(&ctx);
    SHA1_Update(&ctx, reinterpret_cast<const uint8_t*>(accountName), strlen(accountName));
    SHA1_Final(this->accountNameDigest, &ctx);

    SHA1_Init(&ctx);
    SHA1_Update(&ctx, reinterpret_cast<const uint8_t*>(accountName), strlen(accountName));
    SHA1_Update(&ctx, reinterpret_cast<const uint8_t*>(":"), 1u);
    SHA1_Update(&ctx, reinterpret_cast<const uint8_t*>(password), strlen(password));
    SHA1_Final(this->interimDigest, &ctx);

    return 0;
}

int32_t SRP6_Client::CalculateProof(const uint8_t* largeSafePrime, uint32_t largeSafePrimeLen, const uint8_t* generator, uint32_t generatorLen, const uint8_t* salt, uint32_t saltLen, const uint8_t* publicKey, uint32_t publicKeyLen, SRP6_Random& random) {
    // TODO

    return 0;
}
