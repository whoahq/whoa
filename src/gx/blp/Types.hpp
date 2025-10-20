#ifndef GX_BLP_TYPES_HPP
#define GX_BLP_TYPES_HPP

#include <cstdint>

enum MipMapAlgorithm {
    MMA_BOX = 0x0,
    MMA_CUBIC = 0x1,
    MMA_FULLDFT = 0x2,
    MMA_KAISER = 0x3,
    MMA_LINEARLIGHTKAISER = 0x4,
};

struct BlpPalPixel {
    char b;
    char g;
    char r;
    char pad;
};

struct BLPHeader {
    uint32_t magic;
    uint32_t formatVersion;
    char colorEncoding;
    char alphaSize;
    char preferredFormat;
    char hasMips;
    uint32_t width;
    uint32_t height;
    uint32_t mipOffsets[16];
    uint32_t mipSizes[16];

    union {
        BlpPalPixel palette[256];

        struct {
            uint32_t headerSize;
            char headerData[1020];
        } jpeg;
    } extended;
};

#endif
