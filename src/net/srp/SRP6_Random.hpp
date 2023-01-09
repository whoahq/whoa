#ifndef NET_SRP_SRP6_RANDOM_HPP
#define NET_SRP_SRP6_RANDOM_HPP

#include <cstdint>

class SRP6_Random {
    public:
    // Member variables
    char m_randkey1[20];
    char m_randkey2[20];
    char m_randpool[20];
    uint32_t m_inpool;

    // Member functions
    SRP6_Random(const void* seed, uint32_t seedLen);
};

#endif
