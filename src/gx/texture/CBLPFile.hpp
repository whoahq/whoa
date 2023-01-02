#ifndef GX_TEXTURE_C_BLP_FILE_HPP
#define GX_TEXTURE_C_BLP_FILE_HPP

#include "gx/Types.hpp"
#include "gx/texture/CGxTex.hpp"
#include <cstdint>
#include <storm/Array.hpp>

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

class CBLPFile {
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

    public:
        // Static variables
        static TSGrowableArray<unsigned char> s_blpFileLoadBuffer;

        // Member variables
        MipBits* m_images = nullptr;
        BLPHeader m_header;
        void* m_inMemoryImage = nullptr;
        int32_t m_inMemoryNeedsFree;
        uint32_t m_numLevels;
        uint32_t m_quality = 100;
        void* m_colorMapping;
        MipMapAlgorithm m_mipMapAlgorithm = MMA_BOX;
        char* m_lockDecompMem;

        // Member functions
        void Close(void);
        int32_t Lock2(const char*, PIXEL_FORMAT, uint32_t, unsigned char*, uint32_t&);
        int32_t LockChain2(const char*, PIXEL_FORMAT, MipBits*&, uint32_t, int32_t);
        int32_t Open(const char*, int32_t);
        int32_t Source(void*);
};

#endif
