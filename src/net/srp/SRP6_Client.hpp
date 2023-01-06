#ifndef NET_SRP_SRP6_CLIENT_HPP
#define NET_SRP_SRP6_CLIENT_HPP

#include <common/SHA1.hpp>

class SRP6_Client {
    public:
    // Member variables
    uint8_t accountNameDigest[SHA1_DIGEST_SIZE];
    uint8_t interimDigest[SHA1_DIGEST_SIZE];
    SHA1_CONTEXT ctx;

    // Member functions
    int32_t BeginAuthentication(const char* accountName, const char* password);
};

#endif
