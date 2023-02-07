#ifndef NET_SRP_SRP6_CLIENT_HPP
#define NET_SRP_SRP6_CLIENT_HPP

#include <common/SHA1.hpp>

class SRP6_Random;

class SRP6_Client {
    public:
    // Member variables
    uint8_t clientPublicKey[32];
    uint8_t sessionKey[40];
    uint8_t clientProof[20];
    uint8_t accountNameDigest[SHA1_DIGEST_SIZE];
    uint8_t interimDigest[SHA1_DIGEST_SIZE];
    SHA1_CONTEXT ctx;

    // Member functions
    int32_t BeginAuthentication(const char* accountName, const char* password);
    int32_t CalculateProof(const uint8_t* largeSafePrime, uint32_t largeSafePrimeLen, const uint8_t* generator, uint32_t generatorLen, const uint8_t* salt, uint32_t saltLen, const uint8_t* publicKey, uint32_t publicKeyLen, SRP6_Random& random);
    int32_t VerifyServerProof(const uint8_t* serverProof, uint32_t serverProofLen);
};

#endif
