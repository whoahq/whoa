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
    extern uint32_t s_lockVertexCount;
}

uint32_t GxVertexAttribOffset(EGxVertexBufferFormat, EGxVertexAttrib);

CGxBuf* GxBufCreate(CGxPool*, uint32_t, uint32_t, uint32_t);

char* GxBufLock(CGxBuf* buf);

void GxBufUnlock(CGxBuf*, uint32_t);

CGxPool* GxPoolCreate(EGxPoolTarget, EGxPoolUsage, uint32_t, EGxPoolHintBits, char*);

void GxPrimIndexPtr(CGxBuf*);

void GxPrimIndexPtr(uint32_t indexCount, const uint16_t* indices);

void GxPrimVertexPtr(CGxBuf*, EGxVertexBufferFormat);

void GxPrimVertexPtr(uint32_t vertexCount, const C3Vector* pos, uint32_t posStride, const C3Vector* normal, uint32_t normalStride, const CImVector* color, uint32_t colorStride, const C2Vector* tex0, uint32_t tex0Stride, const C2Vector* tex1, uint32_t tex1Stride);

void GxPrimLockVertexPtrs(uint32_t vertexCount, const C3Vector* pos, uint32_t posStride, const C3Vector* normal, uint32_t normalStride, const CImVector* color, uint32_t colorStride, const unsigned char* bone, uint32_t boneStride, const C2Vector* tex0, uint32_t tex0Stride, const C2Vector* tex1, uint32_t tex1Stride);

void GxDrawLockedElements(EGxPrim primType, uint32_t indexCount, const uint16_t* indices);

void GxPrimUnlockVertexPtrs();

#endif
