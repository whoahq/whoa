#ifndef GX_BUFFER_TYPES_HPP
#define GX_BUFFER_TYPES_HPP

#include <cstdint>
#include <tempest/Vector.hpp>

enum EGxPoolHintBits {
    GxPoolHintBit_Unk0 = 0,
    GxPoolHintBit_Unk1 = 1,
    GxPoolHintBit_Unk2 = 2,
    GxPoolHintBit_Unk3 = 3
};

enum EGxPoolTarget {
    GxPoolTarget_Vertex = 0,
    GxPoolTarget_Index = 1,
    GxPoolTargets_Last = 2
};

enum EGxPoolUsage {
    GxPoolUsage_Static = 0,
    GxPoolUsage_Dynamic = 1,
    GxPoolUsage_Stream = 2,
    GxPoolUsages_Last = 3
};

enum EGxVertexAttrib {
    GxVA_Position = 0,
    GxVA_BlendWeight = 1,
    GxVA_BlendIndices = 2,
    GxVA_Normal = 3,
    GxVA_Color0 = 4,
    GxVA_Color1 = 5,
    GxVA_TexCoord0 = 6,
    GxVA_TexCoord1 = 7,
    GxVA_TexCoord2 = 8,
    GxVA_TexCoord3 = 9,
    GxVA_TexCoord4 = 10,
    GxVA_TexCoord5 = 11,
    GxVA_TexCoord6 = 12,
    GxVA_TexCoord7 = 13,
    GxVAs_Last = 14
};

enum EGxVertexBufferFormat {
    GxVBF_P = 0,
    GxVBF_PN = 1,
    GxVBF_PNC = 2,
    GxVBF_PNT = 3,
    GxVBF_PNCT = 4,
    GxVBF_PNT2 = 5,
    GxVBF_PNCT2 = 6,
    GxVBF_PC = 7,
    GxVBF_PCT = 8,
    GxVBF_PCT2 = 9,
    GxVBF_PT = 10,
    GxVBF_PT2 = 11,
    GxVBF_PBNT2 = 12,
    GxVBF_PNC2T2 = 13,
    GxVertexBufferFormats_Last = 14
};

struct ubyte4 {
    union {
        uint8_t b[4];
        uint32_t u;
    };
};

struct CGxVertexAttrib {
    EGxVertexAttrib attrib;
    uint32_t type;
    uint32_t offset;
    uint32_t bufSize;
};

struct CGxVertexPBNT2 {
    C3Vector p;
    ubyte4 bw;
    ubyte4 bi;
    C3Vector n;
    C2Vector tc[2];
};

struct CGxVertexPCT {
    C3Vector p;
    CImVector c;
    C2Vector tc[1];
};

#endif
