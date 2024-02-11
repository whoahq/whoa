#include "gx/glsdl/GLMipmap.hpp"
#include "gx/glsdl/GLSDLDevice.hpp"
#include "gx/glsdl/GLFramebuffer.hpp"
#include <bc/Debug.hpp>

int32_t GLMipmap::GetDepthBits() {
    return this->m_DepthBits;
}

void GLMipmap::Attach(GLFramebuffer* framebuffer, GLenum attachPoint, int32_t a4) {
    if (!this->m_AttachPoints) {
        this->m_AttachPoints = new std::vector<GLAttachPoint>();
    }

    auto& attachPoints = *this->m_AttachPoints;
    auto framebufferID = framebuffer->m_FramebufferID;

    if (framebufferID >= attachPoints.size()) {
        attachPoints.resize(framebufferID + 1);
    } else {
        BLIZZARD_ASSERT(attachPoints[framebufferID].framebuffer != framebuffer || attachPoints[framebufferID].point != attachPoint);

        auto& attach = attachPoints[framebufferID];

        if (
            attach.point
            && (attach.point != GL_DEPTH_ATTACHMENT || attachPoint != GL_STENCIL_ATTACHMENT)
            && (attach.point != GL_STENCIL_ATTACHMENT || attachPoint != GL_DEPTH_ATTACHMENT)
        ) {
            framebuffer->Detach(attach.point);
        }
    }

    GLSDLDevice* device = GLSDLDevice::Get();

    auto currentTarget = device->GetCurrentTarget();
    device->BindFramebuffer(framebuffer);

    if (framebufferID) {
        if (this->m_Target == GL_TEXTURE_3D) {
            glFramebufferTexture3DEXT(
                GL_FRAMEBUFFER,
                attachPoint,
                GL_TEXTURE_3D,
                this->m_Texture->m_TextureID,
                this->m_Level,
                a4
            );
        } else {
            glFramebufferTexture2DEXT(
                GL_FRAMEBUFFER,
                attachPoint,
                this->m_Target,
                this->m_Texture->m_TextureID,
                this->m_Level
            );
        }
    }

    if (attachPoint == GL_DEPTH_ATTACHMENT && !this->m_DepthBits) {
        GLint depthBits = 0;
        glGetIntegerv(GL_DEPTH_BITS, &depthBits);
        this->m_DepthBits = depthBits;
    }

    device->BindFramebuffer(currentTarget);

    auto& attach = attachPoints[framebufferID];
    attach.framebuffer = framebuffer;
    attach.zOffset = a4;

    if (
        (attach.point != GL_DEPTH_ATTACHMENT || attachPoint != GL_STENCIL_ATTACHMENT)
        && (attach.point != GL_STENCIL_ATTACHMENT || attachPoint != GL_DEPTH_ATTACHMENT)
    ) {
        attach.point = attachPoint;
    } else {
        attach.point = GL_DEPTH_STENCIL;
    }
}

void GLMipmap::Detach(GLFramebuffer* framebuffer, GLenum attachPoint, bool a4) {
    GLuint framebufferID = framebuffer->m_FramebufferID;

    auto& attachPoints = *this->m_AttachPoints;

    BLIZZARD_ASSERT(attachPoints.size() >= framebufferID);
    BLIZZARD_ASSERT(attachPoints[framebufferID].framebuffer == framebuffer);

    if (!a4 && framebufferID) {
        GLSDLDevice* v12 = GLSDLDevice::Get();
        GLFramebuffer* v14 = v12->GetCurrentTarget();
        v12->BindFramebuffer(framebuffer);

        if (this->m_Target == GL_TEXTURE_3D) {
            glFramebufferTexture3DEXT(GL_FRAMEBUFFER, attachPoint, GL_TEXTURE_3D, 0, 0, 0);
        } else {
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER, attachPoint, this->m_Target, 0, 0);
        }

        v12->BindFramebuffer(v14);
    }

    GLAttachPoint* v9 = &attachPoints[framebufferID];

    if (v9->point == GL_DEPTH_STENCIL) {
        BLIZZARD_ASSERT(this->GetFormat() == GLTF_D24S8);

        if (attachPoint == GL_DEPTH_ATTACHMENT) {
            v9->point = GL_STENCIL_ATTACHMENT;
        } else if (attachPoint == GL_STENCIL_ATTACHMENT) {
            v9->point = GL_DEPTH_ATTACHMENT;
        } else {
            BLIZZARD_ASSERT(false);
        }
    } else {
        BLIZZARD_ASSERT(attachPoints[framebufferID].point == attachPoint);

        v9->framebuffer = 0;
        v9->point = 0;
        v9->zOffset = 0;

        // TODO
        // this->m_Texture->m_TimeStamp = Blizzard::Time::GetTimestamp();
    }
}

void GLMipmap::DetachAll() {
    if (!this->m_AttachPoints) {
        return;
    }

    auto& attachPoints = *this->m_AttachPoints;
    for (int32_t i = 0; i < attachPoints.size(); i++) {
        BLIZZARD_ASSERT(attachPoints[i].point != GL_ZERO);
        BLIZZARD_ASSERT(attachPoints[i].framebuffer->m_FramebufferID == i);

        attachPoints[i].framebuffer->Detach(attachPoints[i].point);
    }
}

GLTextureFormat GLMipmap::GetFormat() {
    return this->m_Texture->GetFormat();
}

TextureFormatInfo& GLMipmap::GetFormatInfo() {
    return this->m_Texture->GetFormatInfo();
};

uint16_t GLMipmap::GetHeight() {
    return this->m_Height;
}

int32_t GLMipmap::GetPitch() {
    int32_t bpp = this->GetFormatInfo().m_BytePerPixel;
    int32_t v4 = this->m_Texture->var12 >> this->m_Level;
    return v4 >= bpp ? v4 : bpp;
}

GLTexture* GLMipmap::GetTexture() {
    return this->m_Texture;
}

uint32_t GLMipmap::GetTextureID() {
    return this->m_Texture->m_TextureID;
}

uint16_t GLMipmap::GetWidth() {
    return this->m_Width;
}

void* GLMipmap::Map(GLEnum mode, const GLBox* area) {
    BLIZZARD_ASSERT(!this->m_Texture->IsSystemBuffer());
    BLIZZARD_ASSERT(this->m_Data != nullptr);
    BLIZZARD_ASSERT(!this->m_Texture->IsRenderTarget());
    BLIZZARD_ASSERT(mode != GL_ZERO);
    BLIZZARD_ASSERT(this->m_MapParams == nullptr);

    if (mode != GL_READ_ONLY) {
        this->m_Texture->m_MappedMipmaps++;
    }

    MapParams* mapParams = new MapParams();
    this->m_MapParams = mapParams;

    if (area) {
        BLIZZARD_ASSERT(area->width > 0);
        BLIZZARD_ASSERT(area->height > 0);
        BLIZZARD_ASSERT(area->depth > 0);
        BLIZZARD_ASSERT(!this->GetFormatInfo().m_IsCompressed || ((area->top & 0x3) == 0 && (area->left & 0x3) == 0 && (area->width & 0x3) == 0 && (area->height & 0x3) == 0));
        BLIZZARD_ASSERT((area->height + area->top) <= this->m_Height);
        BLIZZARD_ASSERT((area->depth + area->front) <= this->m_Depth);
        BLIZZARD_ASSERT((area->width + area->left) <= this->m_Width);

        mapParams->m_MapArea = {
            area->left,
            area->top,
            area->front,
            area->width,
            area->height,
            area->depth
        };

        int32_t size = this->GetFormatInfo().m_BytePerPixel
            * this->m_Width
            * mapParams->m_MapArea.depth
            * mapParams->m_MapArea.height;

        mapParams->m_Size = this->GetFormatInfo().m_IsCompressed
            ? size >> 4
            : size;

        mapParams->m_Unk7 = (this->GetFormatInfo().m_BytePerPixel * mapParams->m_MapArea.left)
            >> this->GetFormatInfo().m_IsCompressed ? 2 : 0;
    } else {
        mapParams->m_MapArea = {
            0,
            0,
            0,
            this->m_Width,
            this->m_Height,
            this->m_Depth
        };

        mapParams->m_Size = this->m_Size;

        mapParams->m_Unk7 = 0;
    }

    mapParams->m_MapMode = mode;

    int32_t rowPitch = this->GetPitch();

    BLIZZARD_ASSERT(((mapParams->m_MapArea.top * rowPitch) + mapParams->m_MapArea.left * this->GetFormatInfo().m_BytePerPixel) < (this->GetFormatInfo().m_IsCompressed ? this->m_Size << 4 : this->m_Size));

    int32_t v22 = rowPitch * this->m_Height;
    if (this->GetFormatInfo().m_IsCompressed) {
        v22 >>= 2;
    }

    unsigned char* v24 = this->m_Data
        + ((mapParams->m_MapArea.top * rowPitch) >> (this->GetFormatInfo().m_IsCompressed ? 2 : 0))
        + (mapParams->m_MapArea.front * v22);

    mapParams->m_Unk8 = v24;

    return v24 + mapParams->m_Unk7;
}

void* GLMipmap::Map(GLEnum mode, const GLRect* rect) {
    if (rect) {
        GLBox area = {
            rect->left,
            rect->top,
            0,
            rect->width,
            rect->height,
            1
        };

        return this->Map(mode, &area);
    } else {
        return this->Map(mode, static_cast<GLBox*>(nullptr));
    }
}

void GLMipmap::ReleaseObject() {
    BLIZZARD_ASSERT(this->m_MapParams == nullptr);

    this->RemoveDebugMipmap();
    this->DetachAll();

    if (this->m_AttachPoints) {
        delete this->m_AttachPoints;
    }

    this->m_AttachPoints = nullptr;
    this->m_Unk24 = 0;
}

void GLMipmap::RemoveDebugMipmap() {
    // TODO
}

void GLMipmap::ResetData(GLEnum target, int32_t level, unsigned char* data) {
    BLIZZARD_ASSERT(this->m_Target != GL_TEXTURE_3D || !this->GetFormatInfo().m_IsCompressed);

    this->m_Target = target;
    this->m_Level = level;
    this->m_Data = data;

    BLIZZARD_ASSERT(this->GetFormat() != GLTF_INVALID);
    BLIZZARD_ASSERT(this->GetFormat() < GLTF_NUM_TEXTURE_FORMATS);

    if (!this->m_Texture->IsSystemBuffer() && this->m_Texture->IsRenderTarget()) {
        BLIZZARD_ASSERT(!this->GetFormatInfo().m_IsCompressed);

        this->TexImage(nullptr);
        this->m_Unk24 = 1;
    }
}

void GLMipmap::ResetSize(uint32_t width, uint32_t height, uint32_t depth) {
    this->m_Width = width ? width : 1;
    this->m_Height = height ? height : 1;
    this->m_Depth = depth ? depth : 1;

    if (this->GetFormatInfo().m_IsCompressed) {
        BLIZZARD_ASSERT(this->m_Depth == 1);

        this->m_Width = (this->m_Width + 3) & 0xFFFC;
        this->m_Height = (this->m_Height + 3) & 0xFFFC;
    }

    uint32_t v11 = this->GetFormatInfo().m_BytePerPixel;
    uint32_t v20 = this->m_Texture->var12 >> this->m_Level;

    if (v20 >= v11) {
        v11 = v20;
    }

    uint32_t v15 = v11 * this->m_Height;
    uint32_t v12;

    if (this->GetFormatInfo().m_IsCompressed) {
        v12 = v15 >> 2;
    } else {
        v12 = v15;
    }

    this->m_Size = this->m_Depth * v12;
}

void GLMipmap::TexImage(const void* pixels) {
    BLIZZARD_ASSERT((this->m_Texture->IsRenderTarget() || pixels != nullptr) && GLSDLDevice::Get()->GetVertexArrayStates().buffers[eGLBT_PIXEL_UNPACK] == 0);

    if (this->m_Target == GL_TEXTURE_3D) {
        glTexImage3D(
            GL_TEXTURE_3D,
            this->m_Level,
            this->GetFormatInfo().m_InternalFormat,
            this->m_Width,
            this->m_Height,
            this->m_Depth,
            0,
            this->GetFormatInfo().m_DataFormat,
            this->GetFormatInfo().m_DataType,
            pixels
        );
    } else if (this->GetFormatInfo().m_IsCompressed) {
        glCompressedTexImage2D(
            this->m_Target,
            this->m_Level,
            this->GetFormatInfo().m_InternalFormat,
            this->m_Width,
            this->m_Height,
            0,
            this->m_Size,
            pixels
        );
    } else {
        glTexImage2D(
            this->m_Target,
            this->m_Level,
            this->GetFormatInfo().m_InternalFormat,
            this->m_Width,
            this->m_Height,
            0,
            this->GetFormatInfo().m_DataFormat,
            this->GetFormatInfo().m_DataType,
            pixels
        );
    }
}

void GLMipmap::TexSubImage(const GLBox& a2, int32_t size, const void* pixels) {
    BLIZZARD_ASSERT(!this->m_Texture->IsRenderTarget() && pixels != nullptr && GLSDLDevice::Get()->GetVertexArrayStates().buffers[eGLBT_PIXEL_UNPACK] == 0);

    if (this->m_Target == GL_TEXTURE_3D) {
        glPixelStorei(GL_UNPACK_ROW_LENGTH, this->m_Width);
        glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, this->m_Height);

        glTexSubImage3D(
            this->m_Target,
            this->m_Level,
            a2.left,
            a2.top,
            a2.front,
            a2.width,
            a2.height,
            a2.depth,
            this->GetFormatInfo().m_DataFormat,
            this->GetFormatInfo().m_DataType,
            pixels
        );

        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);
    } else if (this->GetFormatInfo().m_IsCompressed) {
        glCompressedTexSubImage2D(
            this->m_Target,
            this->m_Level,
            0,
            a2.top,
            this->m_Width,
            a2.height,
            this->GetFormatInfo().m_InternalFormat,
            size,
            pixels
        );
    } else {
        glPixelStorei(GL_UNPACK_ROW_LENGTH, this->m_Width);

        glTexSubImage2D(
            this->m_Target,
            this->m_Level,
            a2.left,
            a2.top,
            a2.width,
            a2.height,
            this->GetFormatInfo().m_DataFormat,
            this->GetFormatInfo().m_DataType,
            pixels
        );

        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    }
}

void GLMipmap::Unmap() {
    BLIZZARD_ASSERT(!this->m_Texture->IsRenderTarget());
    BLIZZARD_ASSERT(!this->m_Texture->IsSystemBuffer());
    BLIZZARD_ASSERT(this->m_MapParams != nullptr);

    if (this->m_MapParams->m_MapMode == GL_READ_ONLY) {
        delete this->m_MapParams;
        this->m_MapParams = nullptr;
        return;
    }

    GLSDLDevice* device = GLSDLDevice::Get();

    BLIZZARD_ASSERT(this->m_Texture->m_MappedMipmaps > 0);

    this->Unmap(this->m_MapParams);

    this->m_MapParams = nullptr;
}

void GLMipmap::Unmap(MapParams* mapParams) {
    BLIZZARD_ASSERT(mapParams != nullptr);

    this->m_Texture->Bind(nullptr, 0);

    if (this->m_Unk24) {
        if (this->GetFormatInfo().m_IsCompressed) {
            GLBox area = {
                0,
                mapParams->m_MapArea.top,
                mapParams->m_MapArea.front,
                this->m_Width,
                mapParams->m_MapArea.height,
                mapParams->m_MapArea.depth
            };

            this->TexSubImage(area, mapParams->m_Size, mapParams->m_Unk8);
        } else {
            this->TexSubImage(mapParams->m_MapArea, mapParams->m_Size, mapParams->m_Unk8 + mapParams->m_Unk7);
        }
    } else {
        this->TexImage(this->m_Data);
        this->m_Unk24 = 1;
    }

    delete mapParams;

    // TODO

    this->m_Texture->m_MappedMipmaps--;
}
