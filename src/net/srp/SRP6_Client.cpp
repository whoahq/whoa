#include "net/srp/SRP6_Client.hpp"
#include "net/srp/SRP6_Random.hpp"
#include <cstring>
#include <common/BigInteger.hpp>

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
    auto N = BigIntegerFromBytes(largeSafePrime, largeSafePrimeLen);
    auto g = BigIntegerFromBytes(generator, generatorLen);
    auto x = BigIntegerFromInt(0);
    auto v = BigIntegerFromInt(0);
    auto v24 = BigIntegerFromInt(0);
    auto v25 = BigIntegerFromInt(0);
    auto a = BigIntegerFromInt(0);
    auto A = BigIntegerFromInt(0);
    auto u = BigIntegerFromInt(0);
    auto B = BigIntegerFromBytes(publicKey, publicKeyLen);
    auto k = BigIntegerFromInt(0);
    auto S = BigIntegerFromInt(0);

    int32_t result = 0;

    if (!largeSafePrime || largeSafePrimeLen - 1 > 31) {
        result = -1;
        goto cleanup;
    }

    if (!generator || generatorLen - 1 > 31) {
        result = -1;
        goto cleanup;
    }

    if (!salt || saltLen - 1 > 31) {
        result = -1;
        goto cleanup;
    }

    if (!publicKey || publicKeyLen < 31) {
        result = -1;
        goto cleanup;
    }

    SHA1_CONTEXT ctx1;

    SHA1_Init(&ctx1);
    SHA1_Update(&ctx1, salt, saltLen);
    SHA1_Update(&ctx1, this->interimDigest, sizeof(this->interimDigest));
    uint8_t v21[SHA1_DIGEST_SIZE];
    SHA1_Final(v21, &ctx1);

    BigIntegerFree(x);
    x = BigIntegerFromBytes(v21, sizeof(v21));

    BigIntegerModExp(v, g, x, N);

    SHA1_Init(&ctx1);
    SHA1_Update(&ctx1, largeSafePrime, largeSafePrimeLen);
    uint8_t v26[SHA1_DIGEST_SIZE];
    SHA1_Final(v26, &ctx1);

    SHA1_Init(&ctx1);
    SHA1_Update(&ctx1, generator, generatorLen);
    uint8_t v23[SHA1_DIGEST_SIZE];
    SHA1_Final(v23, &ctx1);

    for (int32_t i = 0; i < sizeof(v23); i += 5) {
        v26[i] ^= v23[i];
        auto v14 = v23[i + 2];
        v26[i + 1] ^= v23[i + 1];
        v26[i + 2] ^= v14;
        auto v15 = v23[i + 4];
        v26[i + 3] ^= v23[i + 3];
        v26[i + 4] ^= v15;
    }

    SHA1_CONTEXT ctx2;

    SHA1_Init(&ctx2);
    SHA1_Update(&ctx2, v26, sizeof(v26));
    SHA1_Update(&ctx2, this->accountNameDigest, sizeof(this->accountNameDigest));
    SHA1_Update(&ctx2, salt, saltLen);

    if (BigIntegerBitLen(N) >= 256) {
        uint8_t v19[32];
        // TODO
        // random.GenerateRandomBytes(v19, sizeof(v19));

        BigIntegerFree(a);
        a = BigIntegerFromBytes(v19, sizeof(v19));

        auto v16 = BigIntegerBitLen(N);

        BigIntegerAddInt(a, a, v16);
        BigIntegerModExp(A, g, a, N);
        BigIntegerToBytes(A, this->clientPublicKey, sizeof(this->clientPublicKey));

        SHA1_Update(&ctx2, this->clientPublicKey, sizeof(clientPublicKey));
        SHA1_Update(&ctx2, publicKey, publicKeyLen);

        SHA1_Update(&this->ctx, this->clientPublicKey, sizeof(clientPublicKey));

        SHA1_Init(&ctx1);
        SHA1_Update(&ctx1, this->clientPublicKey, sizeof(this->clientPublicKey));
        SHA1_Update(&ctx1, publicKey, publicKeyLen);
        SHA1_Final(v26, &ctx1);

        BigIntegerFree(u);
        u = BigIntegerFromBytes(v26, sizeof(v26));

        if (BigIntegerCmp(B, N) < 0 && BigIntegerCmpInt(B, 0)) {
            // Formula: S = (B - k*v)^(a + u*x) % N

            BigIntegerSub(k, N, v);
            BigIntegerAdd(B, B, k);
            BigIntegerAdd(B, B, k);
            BigIntegerAdd(B, B, k);
            BigIntegerMod(B, B, N);
            BigIntegerMul(k, x, u);
            BigIntegerAdd(k, k, a);
            BigIntegerModExp(S, B, k, N);

            uint8_t v18[32];
            BigIntegerToBytes(S, v18, sizeof(v18));

            SHA1_InterleaveHash(this->sessionKey, v18, sizeof(v18));
            SHA1_Update(&ctx2, this->sessionKey, sizeof(this->sessionKey));
            SHA1_Final(this->clientProof, &ctx2);

            SHA1_Update(&this->ctx, this->clientProof, sizeof(this->clientProof));
            SHA1_Update(&this->ctx, this->sessionKey, sizeof(this->sessionKey));

            result = 0;
        } else {
            result = -2;
        }
    } else {
        result = -1;
    }

cleanup:
    BigIntegerFree(S);
    BigIntegerFree(k);
    BigIntegerFree(B);
    BigIntegerFree(u);
    BigIntegerFree(A);
    BigIntegerFree(a);
    BigIntegerFree(v25);
    BigIntegerFree(v24);
    BigIntegerFree(v);
    BigIntegerFree(x);
    BigIntegerFree(g);
    BigIntegerFree(N);

    return result;
}

int32_t SRP6_Client::VerifyServerProof(const uint8_t* serverProof, uint32_t serverProofLen) {
    if (serverProofLen != 20) {
        return -2;
    }

    // Calculate expected server proof
    uint8_t localServerProof[SHA1_DIGEST_SIZE];
    SHA1_Final(localServerProof, &this->ctx);

    // Compare expected server proof with given server proof
    return memcmp(localServerProof, serverProof, sizeof(localServerProof));
}
