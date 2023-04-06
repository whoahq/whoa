#include "gx/Buffer.hpp"
#include "gx/Device.hpp"

CGxVertexAttrib vertexAttribsP[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_P, GxVA_Position),            12 }
};

CGxVertexAttrib vertexAttribsPN[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PN, GxVA_Position),           24 },
    { GxVA_Normal,          4, GxVertexAttribOffset(GxVBF_PN, GxVA_Normal),             24 }
};

CGxVertexAttrib vertexAttribsPNC[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PNC, GxVA_Position),          28 },
    { GxVA_Normal,          4, GxVertexAttribOffset(GxVBF_PNC, GxVA_Normal),            28 },
    { GxVA_Color0,          0, GxVertexAttribOffset(GxVBF_PNC, GxVA_Color0),            28 }
};

CGxVertexAttrib vertexAttribsPNT[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PNT, GxVA_Position),          32 },
    { GxVA_Normal,          4, GxVertexAttribOffset(GxVBF_PNT, GxVA_Normal),            32 },
    { GxVA_TexCoord0,       3, GxVertexAttribOffset(GxVBF_PNT, GxVA_TexCoord0),         32 }
};

CGxVertexAttrib vertexAttribsPNCT[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PNCT, GxVA_Position),         36 },
    { GxVA_Normal,          4, GxVertexAttribOffset(GxVBF_PNCT, GxVA_Normal),           36 },
    { GxVA_Color0,          0, GxVertexAttribOffset(GxVBF_PNCT, GxVA_Color0),           36 },
    { GxVA_TexCoord0,       3, GxVertexAttribOffset(GxVBF_PNCT, GxVA_TexCoord0),        36 }
};

CGxVertexAttrib vertexAttribsPNT2[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PNT2, GxVA_Position),         40 },
    { GxVA_Normal,          4, GxVertexAttribOffset(GxVBF_PNT2, GxVA_Normal),           40 },
    { GxVA_TexCoord0,       3, GxVertexAttribOffset(GxVBF_PNT2, GxVA_TexCoord0),        40 },
    { GxVA_TexCoord1,       3, GxVertexAttribOffset(GxVBF_PNT2, GxVA_TexCoord1),        40 }
};

CGxVertexAttrib vertexAttribsPNCT2[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PNCT2, GxVA_Position),        44 },
    { GxVA_Normal,          4, GxVertexAttribOffset(GxVBF_PNCT2, GxVA_Normal),          44 },
    { GxVA_Color0,          0, GxVertexAttribOffset(GxVBF_PNCT2, GxVA_Color0),          44 },
    { GxVA_TexCoord0,       3, GxVertexAttribOffset(GxVBF_PNCT2, GxVA_TexCoord0),       44 },
    { GxVA_TexCoord1,       3, GxVertexAttribOffset(GxVBF_PNCT2, GxVA_TexCoord1),       44 }
};

CGxVertexAttrib vertexAttribsPC[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PC, GxVA_Position),           16 },
    { GxVA_Color0,          0, GxVertexAttribOffset(GxVBF_PC, GxVA_Color0),             16 }
};

CGxVertexAttrib vertexAttribsPCT[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PCT, GxVA_Position),          24 },
    { GxVA_Color0,          0, GxVertexAttribOffset(GxVBF_PCT, GxVA_Color0),            24 },
    { GxVA_TexCoord0,       3, GxVertexAttribOffset(GxVBF_PCT, GxVA_TexCoord0),         24 }
};

CGxVertexAttrib vertexAttribsPCT2[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PCT2, GxVA_Position),         32 },
    { GxVA_Color0,          0, GxVertexAttribOffset(GxVBF_PCT2, GxVA_Color0),           32 },
    { GxVA_TexCoord0,       3, GxVertexAttribOffset(GxVBF_PCT2, GxVA_TexCoord0),        32 },
    { GxVA_TexCoord1,       3, GxVertexAttribOffset(GxVBF_PCT2, GxVA_TexCoord1),        32 }
};

CGxVertexAttrib vertexAttribsPT[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PT, GxVA_Position),           20 },
    { GxVA_TexCoord0,       3, GxVertexAttribOffset(GxVBF_PT, GxVA_TexCoord0),          20 }
};

CGxVertexAttrib vertexAttribsPT2[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PT2, GxVA_Position),          28 },
    { GxVA_TexCoord0,       3, GxVertexAttribOffset(GxVBF_PT2, GxVA_TexCoord0),         28 },
    { GxVA_TexCoord1,       3, GxVertexAttribOffset(GxVBF_PT2, GxVA_TexCoord1),         28 }
};

CGxVertexAttrib vertexAttribsPBNT2[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PBNT2, GxVA_Position),        48 },
    { GxVA_BlendWeight,     2, GxVertexAttribOffset(GxVBF_PBNT2, GxVA_BlendWeight),     48 },
    { GxVA_BlendIndices,    1, GxVertexAttribOffset(GxVBF_PBNT2, GxVA_BlendIndices),    48 },
    { GxVA_Normal,          4, GxVertexAttribOffset(GxVBF_PBNT2, GxVA_Normal),          48 },
    { GxVA_TexCoord0,       3, GxVertexAttribOffset(GxVBF_PBNT2, GxVA_TexCoord0),       48 },
    { GxVA_TexCoord1,       3, GxVertexAttribOffset(GxVBF_PBNT2, GxVA_TexCoord1),       48 }
};

CGxVertexAttrib vertexAttribsPNC2T2[] = {
    { GxVA_Position,        4, GxVertexAttribOffset(GxVBF_PNC2T2, GxVA_Position),       48 },
    { GxVA_Normal,          4, GxVertexAttribOffset(GxVBF_PNC2T2, GxVA_Normal),         48 },
    { GxVA_Color0,          0, GxVertexAttribOffset(GxVBF_PNC2T2, GxVA_Color0),         48 },
    { GxVA_Color1,          0, GxVertexAttribOffset(GxVBF_PNC2T2, GxVA_Color0),         48 },
    { GxVA_TexCoord0,       3, GxVertexAttribOffset(GxVBF_PNC2T2, GxVA_TexCoord0),      48 },
    { GxVA_TexCoord1,       3, GxVertexAttribOffset(GxVBF_PNC2T2, GxVA_TexCoord1),      48 }
};

VertexBufDesc Buffer::s_vertexBufDesc[] = {
    // GxVBF_P
    { vertexAttribsP,       1, 12, GxPrim_Position },

    // GxVBF_PN
    { vertexAttribsPN,      2, 24, GxPrim_Position | GxPrim_Normal },

    // GxVBF_PNC
    { vertexAttribsPNC,     3, 28, GxPrim_Position | GxPrim_Normal | GxPrim_Color0 },

    // GxVBF_PNT
    { vertexAttribsPNT,     3, 32, GxPrim_Position | GxPrim_Normal | GxPrim_TexCoord0 },

    // GxVBF_PNCT
    { vertexAttribsPNCT,    4, 36, GxPrim_Position | GxPrim_Normal | GxPrim_Color0 | GxPrim_TexCoord0 },

    // GxVBF_PNT2
    { vertexAttribsPNT2,    4, 40, GxPrim_Position | GxPrim_Normal | GxPrim_TexCoord0 | GxPrim_TexCoord1 },

    // GxVBF_PNCT2
    { vertexAttribsPNCT2,   5, 44, GxPrim_Position | GxPrim_Normal | GxPrim_Color0 | GxPrim_TexCoord0 | GxPrim_TexCoord1 },

    // GxVBF_PC
    { vertexAttribsPC,      2, 16, GxPrim_Position | GxPrim_Color0 },

    // GxVBF_PCT
    { vertexAttribsPCT,     3, 24, GxPrim_Position | GxPrim_Color0 | GxPrim_TexCoord0 },

    // GxVBF_PCT2
    { vertexAttribsPCT2,    4, 32, GxPrim_Position | GxPrim_Color0 | GxPrim_TexCoord0 | GxPrim_TexCoord1 },

    // GxVBF_PT
    { vertexAttribsPT,      2, 20, GxPrim_Position | GxPrim_TexCoord0 },

    // GxVBF_PT2
    { vertexAttribsPT2,     3, 28, GxPrim_Position | GxPrim_TexCoord0 | GxPrim_TexCoord1 },

    // GxVBF_PBNT2
    { vertexAttribsPBNT2,   6, 48, GxPrim_Position | GxPrim_BlendWeight | GxPrim_BlendIndices | GxPrim_Normal | GxPrim_TexCoord0 | GxPrim_TexCoord1 },

    // GxVBF_PNC2T2
    { vertexAttribsPNC2T2,  6, 48, GxPrim_Position | GxPrim_Normal | GxPrim_Color0 | GxPrim_Color1 | GxPrim_TexCoord0 | GxPrim_TexCoord1 }
};

int32_t Buffer::s_vertexBufOffset[GxVertexBufferFormats_Last][GxVAs_Last] = {
    // GxVBF_P
    { 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PN
    { 0, -1, -1, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PNC
    { 0, -1, -1, 12, 24, -1, -1, -1, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PNT
    { 0, -1, -1, 12, -1, -1, 24, -1, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PNCT
    { 0, -1, -1, 12, 24, -1, 28, -1, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PNT2
    { 0, -1, -1, 12, -1, -1, 24, 32, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PNCT2
    { 0, -1, -1, 12, 24, -1, 28, 36, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PC
    { 0, -1, -1, -1, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PCT
    { 0, -1, -1, -1, 12, -1, 16, -1, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PCT2
    { 0, -1, -1, -1, 12, -1, 16, 24, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PT
    { 0, -1, -1, -1, -1, -1, 12, -1, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PT2
    { 0, -1, -1, -1, -1, -1, 12, 20, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PBNT2
    { 0, 12, 16, 20, -1, -1, 32, 40, -1, -1, -1, -1, -1, -1 },

    // GxVBF_PNC2T2
    { 0, -1, -1, 12, 24, 28, 32, 40, -1, -1, -1, -1, -1, -1 }
};

uint32_t GxVertexAttribOffset(EGxVertexBufferFormat format, EGxVertexAttrib attrib) {
    return Buffer::s_vertexBufOffset[format][attrib];
}

CGxBuf* GxBufCreate(CGxPool* pool, uint32_t itemSize, uint32_t itemCount, uint32_t index) {
    return g_theGxDevicePtr->BufCreate(pool, itemSize, itemCount, index);
}

void GxBufData(CGxBuf* buf, const void* data, uint32_t size, uint32_t offset) {
    if (size == 0) {
        size = buf->m_itemSize * buf->m_itemCount;
    }

    g_theGxDevicePtr->BufData(buf, data, size, offset);
    buf->unk1C = 1;
}

char* GxBufLock(CGxBuf* buf) {
    return g_theGxDevicePtr->BufLock(buf);
}

void GxBufUnlock(CGxBuf* buf, uint32_t size) {
    g_theGxDevicePtr->BufUnlock(buf, size);
    buf->unk1C = 1;
}

CGxPool* GxPoolCreate(EGxPoolTarget target, EGxPoolUsage usage, uint32_t size, EGxPoolHintBits hint, char* name) {
    return g_theGxDevicePtr->PoolCreate(target, usage, size, hint, name);
}

void GxPrimIndexPtr(CGxBuf* buf) {
    g_theGxDevicePtr->PrimIndexPtr(buf);
}

void GxPrimVertexPtr(CGxBuf* buf, EGxVertexBufferFormat format) {
    auto desc = &Buffer::s_vertexBufDesc[format];

    g_theGxDevicePtr->PrimVertexFormat(buf, desc->attribs, desc->attribCount);
    g_theGxDevicePtr->PrimVertexMask(desc->mask);
    g_theGxDevicePtr->PrimVertexPtr(buf, format);
}
