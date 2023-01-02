#ifndef GX_TEXTURE_HPP
#define GX_TEXTURE_HPP

#include "gx/Types.hpp"
#include "gx/texture/CGxTex.hpp"
#include "gx/texture/CTexture.hpp"

typedef HOBJECT HTEXTURE;

typedef void (TEXTURE_CALLBACK)(EGxTexCommand, uint32_t, uint32_t, uint32_t, uint32_t, void*, uint32_t&, const void*&);

class CImVector;

void AsyncTextureWait(CTexture*);

uint32_t CalcLevelCount(uint32_t, uint32_t);

uint32_t CalcLevelOffset(uint32_t, uint32_t, uint32_t, uint32_t);

uint32_t CalcLevelSize(uint32_t, uint32_t, uint32_t, uint32_t);

uint32_t GetBitDepth(uint32_t);

uint32_t GxCalcTexelStrideInBytes(EGxTexFormat, uint32_t);

int32_t GxTexCreate(CGxTexParms const&, CGxTex*&);

int32_t GxTexCreate(EGxTexTarget, uint32_t, uint32_t, uint32_t, EGxTexFormat, EGxTexFormat, CGxTexFlags, void*, void (*)(EGxTexCommand, uint32_t, uint32_t, uint32_t, uint32_t, void*, uint32_t&, const void*&), const char*, CGxTex*&);

void GxTexDestroy(CGxTex* texId);

void GxTexParameters(const CGxTex* texId, CGxTexParms& parms);

bool GxTexReusable(CGxTexParms&);

void GxTexSetWrap(CGxTex* texId, EGxTexWrapMode wrapU, EGxTexWrapMode wrapV);

void GxTexUpdate(CGxTex*, int32_t, int32_t, int32_t, int32_t, int32_t);

void GxTexUpdate(CGxTex*, CiRect&, int32_t);

TEXTURE_CALLBACK GxuUpdateSingleColorTexture;

MipBits* MippedImgAllocA(uint32_t, uint32_t, uint32_t, const char*, int32_t);

uint32_t MippedImgCalcSize(uint32_t, uint32_t, uint32_t);

CGxTex* TextureAllocGxTex(EGxTexTarget, uint32_t, uint32_t, uint32_t, EGxTexFormat, CGxTexFlags, void*, void (*userFunc)(EGxTexCommand, uint32_t, uint32_t, uint32_t, uint32_t, void*, uint32_t&, const void*&), EGxTexFormat);

HTEXTURE TextureCacheGetTexture(char*, char*, CGxTexFlags);

HTEXTURE TextureCacheGetTexture(const CImVector&);

void TextureCacheNewTexture(CTexture*, CGxTexFlags);

void TextureCacheNewTexture(CTexture*, const CImVector&);

HTEXTURE TextureCreate(const char*, CGxTexFlags, CStatus*, int32_t);

HTEXTURE TextureCreate(uint32_t, uint32_t, EGxTexFormat, EGxTexFormat, CGxTexFlags, void*, void (*)(EGxTexCommand, uint32_t, uint32_t, uint32_t, uint32_t, void*, uint32_t&, const void*&), const char*, int32_t);

HTEXTURE TextureCreate(EGxTexTarget, uint32_t, uint32_t, uint32_t, EGxTexFormat, EGxTexFormat, CGxTexFlags, void*, void (*)(EGxTexCommand, uint32_t, uint32_t, uint32_t, uint32_t, void*, uint32_t&, const void*&), const char*, int32_t);

HTEXTURE TextureCreateSolid(const CImVector&);

int32_t TextureGetDimensions(HTEXTURE, uint32_t*, uint32_t*, int32_t);

void TextureIncreasePriority(CTexture*);

void TextureInitialize(void);

int32_t TextureIsSame(HTEXTURE textureHandle, const char* fileName);

void TextureFreeGxTex(CGxTex* texId);

CGxTex* TextureGetGxTex(CTexture*, int32_t, CStatus*);

CGxTex* TextureGetGxTex(HTEXTURE, int32_t, CStatus*);

CTexture* TextureGetTexturePtr(HTEXTURE);

#endif
