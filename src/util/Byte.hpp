#ifndef UTIL_BYTE_HPP
#define UTIL_BYTE_HPP

#include <cstdint>

#define BYTEn(x, n) (*((uint8_t*)&(x)+n))
#define BYTE1(x) BYTEn(x, 1)
#define BYTE2(x) BYTEn(x, 2)
#define BYTE3(x) BYTEn(x, 3)
#define BYTE4(x) BYTEn(x, 4)

#define LOBYTE(x) (*((uint8_t*)&(x)))
#define HIBYTE(x) (*((uint8_t*)&(x)+1))
#define LOWORD(x) (*((uint16_t*)&(x)))
#define HIWORD(x) (*((uint16_t*)&(x)+1))

#endif
