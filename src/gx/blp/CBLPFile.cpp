#include "gx/blp/CBLPFile.hpp"
#include "gx/Texture.hpp"
#include "util/SFile.hpp"
#include <storm/Error.hpp>
#include <storm/Memory.hpp>
#include <cstring>

TSGrowableArray<unsigned char> CBLPFile::s_blpFileLoadBuffer;

void CBLPFile::Close() {
    this->m_inMemoryImage = nullptr;

    if (this->m_images) {
        SMemFree(this->m_images, __FILE__, __LINE__, 0x0);
    }

    this->m_images = nullptr;
}

int32_t CBLPFile::Lock2(const char* fileName, PIXEL_FORMAT format, uint32_t mipLevel, unsigned char* data, uint32_t& stride) {
    STORM_ASSERT(this->m_inMemoryImage);

    if (mipLevel && (!(this->m_header.hasMips & 0xF) || mipLevel >= this->m_numLevels)) {
        return 0;
    }

    unsigned char* mipData = static_cast<unsigned char*>(this->m_inMemoryImage) + this->m_header.mipOffsets[mipLevel];
    size_t mipSize = this->m_header.mipSizes[mipLevel];

    switch (this->m_header.colorEncoding) {
        case COLOR_PAL:
            if (format != PIXEL_ARGB8888) {
                return 0;
            }

            {
                uint32_t width = this->m_header.width >> mipLevel;
                uint32_t height = this->m_header.height >> mipLevel;

                if (!width) {
                    width = 1;
                }

                if (!height) {
                    height = 1;
                }

                uint32_t pixelCount = width * height;
                auto indices = reinterpret_cast<uint8_t*>(mipData);
                const uint8_t* alpha = nullptr;

                if (this->m_header.alphaSize) {
                    alpha = indices + pixelCount;
                }

                const BlpPalPixel* palette = this->m_header.extended.palette;
                auto out = reinterpret_cast<uint32_t*>(data);

                for (uint32_t i = 0; i < pixelCount; ++i) {
                    const auto& pal = palette[indices[i]];
                    uint8_t a = 0xFF;

                    switch (this->m_header.alphaSize) {
                        case 1: {
                            uint8_t byte = alpha[i >> 3];
                            a = ((byte >> (i & 7)) & 0x1) ? 0xFF : 0x00;
                            break;
                        }

                        case 4: {
                            uint8_t byte = alpha[i >> 1];
                            uint8_t nibble = (i & 1) ? (byte >> 4) : (byte & 0x0F);
                            a = static_cast<uint8_t>((nibble << 4) | nibble);
                            break;
                        }

                        case 8:
                            a = alpha[i];
                            break;

                        default:
                            break;
                    }

                    out[i] = (static_cast<uint32_t>(a) << 24)
                        | (static_cast<uint32_t>(static_cast<uint8_t>(pal.r)) << 16)
                        | (static_cast<uint32_t>(static_cast<uint8_t>(pal.g)) << 8)
                        | static_cast<uint32_t>(static_cast<uint8_t>(pal.b));
                }
            }

            return 1;

        case COLOR_DXT:
            switch (format) {
                case PIXEL_DXT1:
                case PIXEL_DXT3:
                case PIXEL_DXT5:
                    memcpy(data, mipData, mipSize);
                    return 1;

                case PIXEL_ARGB8888:
                case PIXEL_ARGB1555:
                case PIXEL_ARGB4444:
                case PIXEL_RGB565:
                    // TODO
                    return 0;

                case PIXEL_ARGB2565:
                    return 0;

                default:
                    return 0;
            }

        case COLOR_3:
            memcpy(data, mipData, mipSize);
            return 1;

        default:
            return 0;
    }
}

int32_t CBLPFile::LockChain2(const char* fileName, PIXEL_FORMAT format, MipBits*& images, uint32_t mipLevel, int32_t a6) {
    if (mipLevel && (!(this->m_header.hasMips & 0xF) || mipLevel >= this->m_numLevels)) {
        return 0;
    }

    if (images) {
        if (a6 && (this->m_header.colorEncoding == COLOR_DXT || this->m_header.colorEncoding == COLOR_3)) {
            if (this->m_header.colorEncoding == COLOR_3 || (format != PIXEL_ARGB4444 && format != PIXEL_RGB565 && format != PIXEL_ARGB1555 && format != PIXEL_ARGB8888)) {
                uint32_t* offset = this->m_header.mipOffsets;

                for (int32_t i = 0; *offset; offset++, i++) {
                    void* address = static_cast<char*>(this->m_inMemoryImage) + *offset;
                    MipBits* image = static_cast<MipBits*>(address);
                    reinterpret_cast<MipBits**>(images)[i] = image;
                }

                this->m_inMemoryImage = nullptr;
                return 1;
            }
        }

        uint32_t v13 = this->m_header.height >> mipLevel;

        if (v13 <= 1) {
            v13 = 1;
        }

        uint32_t v14 = this->m_header.width >> mipLevel;

        if (v14 <= 1) {
            v14 = 1;
        }

        if (!MippedImgSet(images, format, v14, v13)) {
            return 0;
        }
    } else {
        uint32_t v9 = this->m_header.height >> mipLevel;

        if (v9 <= 1) {
            v9 = 1;
        }

        uint32_t v10 = this->m_header.width >> mipLevel;

        if (v10 <= 1) {
            v10 = 1;
        }

        images = MippedImgAllocA(format, v10, v9, __FILE__, __LINE__);

        if (!images) {
            return 0;
        }
    }

    MipBits** ptr = reinterpret_cast<MipBits**>(images);

    for (int32_t level = mipLevel, i = 0; level < this->m_numLevels; level++, i++) {
        if (!this->Lock2(fileName, format, level, reinterpret_cast<unsigned char*>(ptr[i]), mipLevel)) {
            return 0;
        }
    }

    this->m_inMemoryImage = nullptr;

    return 1;
}

int32_t CBLPFile::Open(const char* filename, int32_t a3) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(filename);
    STORM_VALIDATE_END;

    this->m_inMemoryImage = nullptr;

    if (this->m_images) {
        SMemFree(this->m_images, __FILE__, __LINE__, 0);
    }

    size_t v8 = a3 != 0;

    this->m_images = nullptr;

    SFile* fileptr;

    if (!SFile::OpenEx(nullptr, filename, v8, &fileptr)) {
        return 0;
    }

    int32_t blpSize = SFile::GetFileSize(fileptr, 0);
    CBLPFile::s_blpFileLoadBuffer.SetCount(blpSize);

    size_t bytesRead;

    SFile::Read(
        fileptr,
        CBLPFile::s_blpFileLoadBuffer.m_data,
        CBLPFile::s_blpFileLoadBuffer.Count(),
        &bytesRead,
        0,
        0
    );

    SFile::Close(fileptr);

    return this->Source(CBLPFile::s_blpFileLoadBuffer.m_data);
}

int32_t CBLPFile::Source(void* fileBits) {
    this->m_inMemoryImage = nullptr;

    if (this->m_images) {
        SMemFree(this->m_images, __FILE__, __LINE__, 0);
    }

    this->m_images = nullptr;
    this->m_inMemoryNeedsFree = 0;
    this->m_inMemoryImage = fileBits;

    memcpy(&this->m_header, fileBits, sizeof(this->m_header));

    if (this->m_header.magic != 0x32504C42 || this->m_header.formatVersion != 1) {
        return 0;
    }

    if (this->m_header.hasMips & 0xF) {
        this->m_numLevels = CalcLevelCount(this->m_header.width, this->m_header.height);
    } else {
        this->m_numLevels = 1;
    }

    return 1;
}
