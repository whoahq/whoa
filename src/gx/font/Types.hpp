#ifndef GX_FONT_TYPES_HPP
#define GX_FONT_TYPES_HPP

#include <cstdint>
#include <common/Handle.hpp>

typedef HOBJECT HTEXTBLOCK;
typedef HOBJECT HTEXTFONT;
typedef HOBJECT HFACE;

enum QUOTEDCODE {
    CODE_INVALIDCODE = 0x0,
    CODE_COLORON = 0x1,
    CODE_COLORRESTORE = 0x2,
    CODE_NEWLINE = 0x3,
    CODE_PIPE = 0x4,
    CODE_HYPERLINKSTART = 0x5,
    CODE_HYPERLINKSTOP = 0x6,
    CODE_TEXTURESTART = 0x7,
    CODE_TEXTURESTOP = 0x8,
    NUM_QUOTEDCODES = 0x9
};

enum EGxStringFlags {
    EGxStringFlags_DropShadow   = 0x1,
    EGxStringFlags_NoWrap       = 0x2,
    EGxStringFlags_FixedSize    = 0x4,
    EGxStringFlags_FixedColor   = 0x8,
    EGxStringFlags_FixedSpacing = 0x10,
    EGxStringFlags_Flag20       = 0x20,
    EGxStringFlags_NoSpaceWrap  = 0x40,
    EGxStringFlags_Billboarded  = 0x80,
};

struct EMBEDDEDPARSEINFO {
    // TODO
};

struct GXUEMBEDDEDTEXTUREINFO {
    float float0 = 0.0f;
    float float4 = 0.0f;
    float float8 = 0.0f;
    float floatC = 0.0f;
    uint32_t dword10 = 0;
    uint32_t dword14 = 0;
    float float18 = 0.0f;
    float float1C = 0.0f;
    float float20 = 0.0f;
    float float24 = 0.0f;
    float float28 = 0.0f;
    float float2C = 0.0f;
    float float30 = 0.0f;
    float float34 = 0.0f;
};

#endif
