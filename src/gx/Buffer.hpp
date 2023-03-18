#ifndef GX_BUFFER_HPP
#define GX_BUFFER_HPP

#include "gx/buffer/CGxBuf.hpp"
#include "gx/buffer/CGxPool.hpp"
#include "gx/buffer/Types.hpp"
#include <cstdint>

class CGxBuf;
class CGxPool;

struct VertexBufDesc {
    CGxVertexAttrib* attribs;
    uint32_t attribCount;
    uint32_t size;
    uint32_t mask;
};

namespace Buffer {
    extern VertexBufDesc s_vertexBufDesc[GxVertexBufferFormats_Last];
    extern int32_t s_vertexBufOffset[GxVertexBufferFormats_Last][GxVAs_Last];
}

uint32_t GxVertexAttribOffset(EGxVertexBufferFormat, EGxVertexAttrib);

CGxBuf* GxBufCreate(CGxPool*, uint32_t, uint32_t, uint32_t);

char* GxBufLock(CGxBuf* buf);

void GxBufUnlock(CGxBuf*, uint32_t);

CGxPool* GxPoolCreate(EGxPoolTarget, EGxPoolUsage, uint32_t, EGxPoolHintBits, char*);

void GxPrimIndexPtr(CGxBuf*);

void GxPrimVertexPtr(CGxBuf*, EGxVertexBufferFormat);

void GxPrimLockVertexPtrs(uint32_t, const C3Vector*, uint32_t, const C3Vector*, uint32_t, const CImVector*, uint32_t, const unsigned char*, uint32_t, const C2Vector*, uint32_t, const C2Vector*, uint32_t);

void GxPrimDrawElements(EGxPrimType, uint32_t, uint16_t*);

void GxPrimUnlockVertexPtrs();

#endif
