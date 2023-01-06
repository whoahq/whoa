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
