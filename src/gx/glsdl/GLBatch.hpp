#ifndef GX_GL_SDL_GL_BATCH_HPP
#define GX_GL_SDL_GL_BATCH_HPP

#include "gx/glsdl/GLBuffer.hpp"
#include "gx/glsdl/GLShader.hpp"
#include "gx/glsdl/GLTexture.hpp"
#include "gx/glsdl/GLTypes.hpp"
#include "gx/glsdl/GLVertexFormat.hpp"
#include <cstdint>

class GLBatch {
    public:
        GLShader* var0;
        GLShader* var1;
        GLTexture* textures[16];
        GLBuffer* var3;
        GLBuffer* var4[4];
        uint32_t var5[4];
        uint32_t var6[4];
        GLVertexFormat* var7;
        uint32_t var8;
        uint32_t var9;
        uint32_t var10;
        uint32_t var11;
        uint32_t var12;
        uint32_t var13;
        uint32_t var14;
        int32_t var15;
        bool var16;
        bool var17;
        GLStates var18;
        GLTexture2D* colorBuffer[4];
        GLTexture2D* var20;
        uint32_t var21[128];
        char var22[64];
        char var23[1024];
        uint32_t var24;
        bool var25;
        int64_t var26;
        int64_t var27;
        float var28[4096];
        float var29[1024];
};

#endif
