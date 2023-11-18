#include "gx/glsdl/GLTexture.hpp"
#include "gx/glsdl/GLSDLDevice.hpp"
#include "gx/glsdl/GLPool.hpp"
#include "gx/glsdl/GLUtil.hpp"
#include "gx/texture/CGxTex.hpp"
#include <deque>
#include <bc/Debug.hpp>
#include <bc/Memory.hpp>

Blizzard::Thread::TLSSlot GLTexture::m_Bindings[4];

void* GLTexture::CreateBindings(void* ptr) {
    return new std::deque<std::vector<Binding>>;
}

void GLTexture::DestroyBindings(void* ptr) {
    delete static_cast<std::deque<std::vector<Binding>>*>(ptr);
}

void GLTexture::Bind(GLSDLDevice* device, bool force) {
    BLIZZARD_ASSERT(!this->IsSystemBuffer());
    BLIZZARD_ASSERT(this->m_Depth != 0);

    if (!device) {
        device = GLSDLDevice::Get();
    }

    BLIZZARD_ASSERT(device != nullptr);

    auto& bindings = this->GetBindings();
    uint32_t deviceID = device->GetID();

    if (deviceID >= bindings.size()) {
        bindings.resize(deviceID + 1);
    }

    bindings[deviceID].device = device;

    uint32_t currentActiveTexture = device->m_States.binding.currentActiveTexture;

    if (bindings[deviceID].boundStages[currentActiveTexture]) {
        return;
    }

    if (force) {
        bindings[deviceID].boundStages[currentActiveTexture] = 1;
        device->BindTexture(this->m_TextureType, this);
        return;
    }

    for (int32_t i = 0; i < 16; i++) {
        if (bindings[deviceID].boundStages[i]) {
            device->SetActiveTexture(i);
            return;
        }
    }

    bindings[deviceID].boundStages[currentActiveTexture] = 1;
    device->BindTexture(this->m_TextureType, this);
}

void GLTexture::FreeTexture() {
    auto device = GLSDLDevice::Get();

    int32_t numFace = this->m_TextureType == GL_TEXTURE_CUBE_MAP ? 6 : 1;
    for (int32_t face = 0; face < numFace; face++) {
        if (this->m_Mipmaps[face]) {
            delete[] this->m_Mipmaps[face];
        }
    }

    if (this->m_Mipmaps) {
        delete[] this->m_Mipmaps;
    }

    this->m_Mipmaps = nullptr;
    this->m_Depth = 0;

    // TODO this->Sub690D0();

    glDeleteTextures(1, &this->m_TextureID);

    this->m_GenerateMipmaps = 0;
    this->m_MaxMipmapLevel = 1000;
    this->m_BaseMipmapLevel = 0;
    this->m_CompareMode = 0;

    this->m_Sampler.mipmapBias = 0.0f;
    this->m_Sampler.borderColor = { 0.0f, 0.0f, 0.0f, 0.0f };
    this->m_Sampler.addressModeS = GL_REPEAT;
    this->m_Sampler.addressModeT = GL_REPEAT;
    this->m_Sampler.addressModeR = GL_REPEAT;
    this->m_Sampler.magFilterMode = GL_LINEAR;
    this->m_Sampler.minFilterMode = GL_NEAREST_MIPMAP_LINEAR;
    this->m_Sampler.maxAnisotropy = 1.0f;

    Blizzard::Memory::Free(this->m_Data);
    this->m_Data = nullptr;

    switch (this->m_TextureType) {
        case GL_TEXTURE_3D:
            // TODO GLPool<GLTexture3D>::GLObjectPool::Push(GLPool<GLTexture3D>::m_pool + 264, this);
            break;

        case GL_TEXTURE_CUBE_MAP:
            // TODO GLPool<GLTextureCubeMap>::GLObjectPool::Push(GLPool<GLTextureCubeMap>::m_pool + 520, this);
            break;

        case GL_TEXTURE_2D:
            // TODO  GLPool<GLTexture2D>::GLObjectPool::Push(GLPool<GLTexture2D>::m_pool + 131080, this);
            break;
    }
}

std::vector<GLTexture::Binding>& GLTexture::GetBindings() {
    uint32_t index = GLSDLTextureTypeToIndex(this->m_TextureType);

    uint32_t id;

    if (index == 0) {
        id = this->m_TextureID - 33;
    } else if (index == 1) {
        id = this->m_TextureID - 32801;
    } else if (index == 2) {
        id = this->m_TextureID - 32865;
    } else if (index == 3) {
        id = this->m_TextureID - 1;
    }

    auto target = static_cast<std::deque<std::vector<Binding>>*>(
        Blizzard::Thread::RegisterLocalStorage(
            &GLTexture::m_Bindings[index],
            GLTexture::CreateBindings,
            nullptr,
            GLTexture::DestroyBindings
        )
    );

    if (id >= target->size()) {
        target->resize(id + 1);
    }

    return (*target)[id];
}

GLTextureFormat GLTexture::GetFormat() {
    return this->m_Format;
}

TextureFormatInfo& GLTexture::GetFormatInfo() {
    return k_TextureFormatInfo[this->m_Format];
}

GLMipmap* GLTexture::GetMipmap(uint32_t level, GLEnum face) {
    BLIZZARD_ASSERT(face >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && face <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    BLIZZARD_ASSERT(level < this->m_NumMipmap);
    BLIZZARD_ASSERT(this->m_Mipmaps != nullptr);
    BLIZZARD_ASSERT(this->m_Mipmaps[face - GL_TEXTURE_CUBE_MAP_POSITIVE_X] != nullptr);

    return &this->m_Mipmaps[face - GL_TEXTURE_CUBE_MAP_POSITIVE_X][level];
}

bool GLTexture::IsRenderTarget() {
    return this->m_Flags & GLTFLAG_RENDERTARGET;
}

bool GLTexture::IsSystemBuffer() {
    return this->m_Flags & GLTFLAG_SYSTEM_BUFFER;
}

bool GLTexture::IsValid() {
    return this->m_Depth;
}

void* GLTexture::Map(uint32_t level, const GLRect* a3, uint32_t& a4, GLEnum a5) {
    BLIZZARD_ASSERT(this->m_TextureType != GL_TEXTURE_3D);

    auto mipmap = this->GetMipmap(level, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    a4 = mipmap->GetPitch();
    return mipmap->Map(a5, a3);
}

void GLTexture::RecreateGLTexture() {
    if (this->m_TextureType == GL_TEXTURE_RECTANGLE) {
        return;
    }

    bool isCubeMap = this->m_TextureType == GL_TEXTURE_CUBE_MAP;
    int32_t numFace = isCubeMap ? 6 : 1;

    for (int32_t face = 0; face < numFace; face++) {
        for (int32_t level = 0; level < this->m_NumMipmap; level++) {
            this->m_Mipmaps[face][level].Map(GL_WRITE_ONLY, static_cast<GLBox*>(nullptr));
            this->m_Mipmaps[face][level].Unmap();
        }
    }

    glTexParameterf(this->m_TextureType, GL_TEXTURE_LOD_BIAS, this->m_Sampler.mipmapBias);
    glTexParameteri(this->m_TextureType, GL_TEXTURE_WRAP_S, this->m_Sampler.addressModeS);
    glTexParameteri(this->m_TextureType, GL_TEXTURE_WRAP_T, this->m_Sampler.addressModeT);
    glTexParameteri(this->m_TextureType, GL_TEXTURE_WRAP_R, this->m_Sampler.addressModeR);
    glTexParameteri(this->m_TextureType, GL_TEXTURE_MIN_FILTER, this->m_Sampler.minFilterMode);
    glTexParameteri(this->m_TextureType, GL_TEXTURE_MAG_FILTER, this->m_Sampler.magFilterMode);
    glTexParameterf(this->m_TextureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->m_Sampler.maxAnisotropy);
    glTexParameterfv(this->m_TextureType, GL_TEXTURE_BORDER_COLOR, reinterpret_cast<GLfloat*>(&this->m_Sampler.borderColor));
    glTexParameteri(this->m_TextureType, GL_TEXTURE_MAX_LEVEL, this->m_MaxMipmapLevel);
    glTexParameteri(this->m_TextureType, GL_TEXTURE_BASE_LEVEL, this->m_BaseMipmapLevel);
    glTexParameteri(this->m_TextureType, GL_GENERATE_MIPMAP, this->m_GenerateMipmaps);
}

void GLTexture::ResizeMipmaps() {
    BLIZZARD_ASSERT(this->m_Mipmaps == nullptr);

    int32_t numFace = this->m_TextureType == GL_TEXTURE_CUBE_MAP ? 6 : 1;

    this->m_Mipmaps = new GLMipmap*[numFace];

    for (int32_t face = 0; face < numFace; face++) {
        this->m_Mipmaps[face] = new GLMipmap[this->m_NumMipmap];
    }
}

void GLTexture::SetAddressModeR(GLEnum mode) {
    // TODO
}

void GLTexture::SetAddressModeS(GLEnum mode) {
    if (this->m_Sampler.addressModeS == mode) {
        return;
    }

    if (mode == GL_CLAMP_TO_EDGE) {
        this->Bind(nullptr, 0);
        glTexParameteri(this->m_TextureType, GL_TEXTURE_WRAP_S, mode);
        this->m_Sampler.addressModeS = mode;
    } else {
        // Workaround for buggy GPU (possibly ATI Radeon X1900)
        if (GLSDLDevice::GetRendererInfo().renderer_id == 0x21900) {
            if (this->m_Width & (this->m_Width - 1)) {
                return;
            }

            if (this->m_Height & (this->m_Height - 1)) {
                return;
            }
        }

        this->Bind(nullptr, 0);
        glTexParameteri(this->m_TextureType, GL_TEXTURE_WRAP_S, mode);
        this->m_Sampler.addressModeS = mode;
    }
}

void GLTexture::SetAddressModeT(GLEnum mode) {
    if (this->m_Sampler.addressModeT == mode) {
        return;
    }

    if (mode == GL_CLAMP_TO_EDGE) {
        this->Bind(nullptr, 0);
        glTexParameteri(this->m_TextureType, GL_TEXTURE_WRAP_T, mode);
        this->m_Sampler.addressModeT = mode;
    } else {
        // Workaround for buggy GPU (possibly ATI Radeon X1900)
        if (GLSDLDevice::GetRendererInfo().renderer_id == 0x21900) {
            if (this->m_Width & (this->m_Width - 1)) {
                return;
            }

            if (this->m_Height & (this->m_Height - 1)) {
                return;
            }
        }

        this->Bind(nullptr, 0);
        glTexParameteri(this->m_TextureType, GL_TEXTURE_WRAP_T, mode);
        this->m_Sampler.addressModeT = mode;
    }
}

void GLTexture::SetBorderColor(const GLColor4f& color) {
    // TODO
}

void GLTexture::SetCompareMode(GLEnum compareMode) {
    BLIZZARD_ASSERT(
        this->GetFormatInfo().m_DataFormat == GL_DEPTH_COMPONENT
        || this->GetFormatInfo().m_DataFormat == GL_DEPTH_STENCIL_EXT
        || compareMode == GL_NONE
    );

    if (this->m_CompareMode != compareMode) {
        this->Bind(nullptr, 0);
        glTexParameteri(this->m_TextureType, GL_TEXTURE_COMPARE_MODE, compareMode);
        this->m_CompareMode = compareMode;
    }
}

void GLTexture::SetMagFilterMode(GLEnum mode) {
    if (this->m_Sampler.magFilterMode == mode) {
        return;
    }

    if (GLSDLDevice::GetRendererInfo().vendor_id == 2 && this->IsRenderTarget() && mode != GL_LINEAR) {
        return;
    }

    this->Bind(nullptr, 0);
    glTexParameteri(this->m_TextureType, GL_TEXTURE_MAG_FILTER, mode);
    this->m_Sampler.magFilterMode = mode;
}

void GLTexture::SetMaxAnisotropy(int32_t maxAnisotropy) {
    if (this->m_Sampler.maxAnisotropy == maxAnisotropy) {
        return;
    }

    this->Bind(nullptr, 0);
    glTexParameterf(this->m_TextureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    this->m_Sampler.maxAnisotropy = maxAnisotropy;
}

void GLTexture::SetMinFilterMode(GLEnum mode) {
    if (this->m_Sampler.minFilterMode == mode) {
        return;
    }

    if (GLSDLDevice::GetRendererInfo().vendor_id == 2 && this->IsRenderTarget() && mode != GL_LINEAR) {
        return;
    }

    this->Bind(nullptr, 0);
    glTexParameteri(this->m_TextureType, GL_TEXTURE_MIN_FILTER, mode);
    this->m_Sampler.minFilterMode = mode;
}

void GLTexture::SetupTexture() {
    BLIZZARD_ASSERT(this->m_NumMipmap == 1 || (this->m_Flags & GLTFLAG_AUTOGEN_MIPMAP) == 0);
    BLIZZARD_ASSERT(!this->IsRenderTarget() || this->m_NumMipmap == 1);
    BLIZZARD_ASSERT(!this->IsRenderTarget() || (this->m_Flags & GLTFLAG_READ_ACCESS) == 0);

    GLSDLDevice* device = GLSDLDevice::Get();

    if (this->GetFormatInfo().m_IsCompressed) {
        int32_t smallestDim = std::min(this->m_Width, this->m_Height);

        BLIZZARD_ASSERT(smallestDim >= 4);

        if (smallestDim == 4) {
            this->m_NumMipmap = 1;
        } else if (smallestDim == 8) {
            this->m_NumMipmap = 2;
        } else if (smallestDim == 16) {
            this->m_NumMipmap = 3;
        } else if (smallestDim == 32) {
            this->m_NumMipmap = 4;
        } else if (smallestDim == 64) {
            this->m_NumMipmap = 5;
        } else if (smallestDim == 128) {
            this->m_NumMipmap = 6;
        } else if (smallestDim == 256) {
            this->m_NumMipmap = 7;
        } else if (smallestDim == 512) {
            this->m_NumMipmap = 8;
        } else if (smallestDim == 1024) {
            this->m_NumMipmap = 9;
        } else if (smallestDim == 2048) {
            this->m_NumMipmap = 10;
        } else if (smallestDim == 4096) {
            this->m_NumMipmap = 11;
        } else {
            int32_t i = smallestDim >> 1;
            int32_t n = 0;

            while (i) {
                i >>= 1;
                n++;
            }

            this->m_NumMipmap = n - 1;
        }
    } else {
        int32_t largestDim = std::max(this->m_Width, this->m_Height);

        if (largestDim == 1) {
            this->m_NumMipmap = 1;
        } else if (largestDim == 2) {
            this->m_NumMipmap = 2;
        } else if (largestDim == 4) {
            this->m_NumMipmap = 3;
        } else if (largestDim == 8) {
            this->m_NumMipmap = 4;
        } else if (largestDim == 16) {
            this->m_NumMipmap = 5;
        } else if (largestDim == 32) {
            this->m_NumMipmap = 6;
        } else if (largestDim == 64) {
            this->m_NumMipmap = 7;
        } else if (largestDim == 128) {
            this->m_NumMipmap = 8;
        } else if (largestDim == 256) {
            this->m_NumMipmap = 9;
        } else if (largestDim == 512) {
            this->m_NumMipmap = 10;
        } else if (largestDim == 1024) {
            this->m_NumMipmap = 11;
        } else if (largestDim == 2048) {
            this->m_NumMipmap = 12;
        } else if (largestDim == 4096) {
            this->m_NumMipmap = 13;
        } else {
            int32_t i = largestDim >> 1;
            int32_t n = 0;

            while (i) {
                i >>= 1;
                n++;
            }

            this->m_NumMipmap = n + 1;
        }
    }

    if (!(this->m_Flags & GLTFLAG_SYSTEM_BUFFER)) {
        BLIZZARD_ASSERT(this->m_RequestedNumMipmaps != 0);

        this->m_NumMipmap = std::min(this->m_NumMipmap, this->m_RequestedNumMipmaps);
    }

    this->var12 = this->GetFormatInfo().m_BytePerPixel * this->m_Width;
    if (this->GetFormatInfo().m_IsCompressed) {
        this->var12 >>= 2;
    }

    this->ResizeMipmaps();

    bool isCubeMap = this->m_TextureType == GL_TEXTURE_CUBE_MAP;

    this->m_Size = 0;

    int32_t numFace = isCubeMap ? 6 : 1;

    for (int32_t face = 0; face < numFace; face++) {
        for (int32_t level = 0; level < this->m_NumMipmap; level++) {
            GLMipmap* mip = this->GetMipmap(level, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face);

            mip->m_Level = level;
            mip->m_Texture = this;
            mip->ResetSize(this->m_Width >> level, this->m_Height >> level, this->m_Depth >> level);

            this->m_Size += mip->m_Size;
        }
    }

    this->var7 = GLSDLDevice::m_Devices[0]->m_TextureList.begin();

    if (this->m_Flags & GLTFLAG_SYSTEM_BUFFER) {
        return;
    }

    BLIZZARD_ASSERT(this->m_Data == nullptr);

    if (!this->IsRenderTarget()) {
        this->m_Data = static_cast<char*>(Blizzard::Memory::Allocate(this->m_Size));
    }

    this->Bind(nullptr, 0);

    if (this->IsRenderTarget()) {
        this->SetAddressModeR(GL_CLAMP_TO_EDGE);
        this->SetAddressModeS(GL_CLAMP_TO_EDGE);
        this->SetAddressModeT(GL_CLAMP_TO_EDGE);
        this->SetMinFilterMode(GL_LINEAR);
        this->SetMagFilterMode(GL_LINEAR);

        // glTexParameteri(this->m_TextureType, GL_TEXTURE_STORAGE_HINT_APPLE, GL_STORAGE_CACHED_APPLE);
    } else {
        this->SetAddressModeR(GL_REPEAT);
        this->SetAddressModeS(GL_REPEAT);
        this->SetAddressModeT(GL_REPEAT);
        this->SetMinFilterMode(GL_NEAREST_MIPMAP_NEAREST);
        this->SetMagFilterMode(GL_NEAREST);
    }

    if (this->GetFormatInfo().m_DataFormat == GL_DEPTH_COMPONENT || this->GetFormatInfo().m_DataFormat == GL_DEPTH_STENCIL) {
        this->SetCompareMode(GLSDLDevice::m_ExtARBShadow >= 1 ? GL_COMPARE_R_TO_TEXTURE : 0);
        glTexParameteri(this->m_TextureType, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(this->m_TextureType, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
    } else {
        this->SetCompareMode(0);
    }

    this->SetMaxAnisotropy(1);

    this->SetBorderColor(GLColor4f::ZERO);

    int32_t autogenMipmap = this->m_Flags & GLTFLAG_AUTOGEN_MIPMAP;
    if (autogenMipmap != this->m_GenerateMipmaps) {
        this->Bind(nullptr, 0);
        glTexParameteri(this->m_TextureType, GL_GENERATE_MIPMAP, autogenMipmap);
        this->m_GenerateMipmaps = autogenMipmap;
    }

    int32_t maxMipmapLevel = this->m_NumMipmap - 1;
    if (maxMipmapLevel != this->m_MaxMipmapLevel) {
        this->Bind(nullptr, 0);
        glTexParameteri(this->m_TextureType, GL_TEXTURE_MAX_LEVEL, maxMipmapLevel);
        this->m_MaxMipmapLevel = maxMipmapLevel;
    }

    if (this->m_TextureType == GL_TEXTURE_RECTANGLE) {
        return;
    }

    if (this->IsRenderTarget()) {
        device->SetUnpackClientStorage(0);
    }

    unsigned char* data = reinterpret_cast<unsigned char*>(this->m_Data);

    for (int32_t face = 0; face < numFace; face++) {
        for (int32_t level = 0; level < this->m_NumMipmap; level++) {
            GLMipmap* mip = this->GetMipmap(level, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face);

            if (this->m_TextureType == GL_TEXTURE_CUBE_MAP) {
                mip->ResetData(face, level, data);
            } else {
                mip->ResetData(this->m_TextureType, level, data);
            }

            if (data) {
                data += mip->m_Size;
            }
        }
    }

    // TODO
    // this->m_TimeStamp = Blizzard::Time::GetTimestamp();

    if (this->IsRenderTarget()) {
        device->SetUnpackClientStorage(1);
    }
}

void GLTexture::Unbind(GLSDLDevice* device, uint32_t stage) {
    auto& bindings = this->GetBindings();

    BLIZZARD_ASSERT(device->GetID() < bindings.size());
    BLIZZARD_ASSERT(bindings[device->GetID()].device == device);
    BLIZZARD_ASSERT(bindings[device->GetID()].boundStages[stage]);

    bindings[device->GetID()].boundStages[stage] = 0;
}

void GLTexture::Unmap(uint32_t level, GLEnum face) {
    auto mipmap = this->GetMipmap(level, face);
    mipmap->Unmap();
}

GLTexture2D* GLTexture2D::Create(uint32_t width, uint32_t height, uint32_t numMipMap, GLTextureFormat format, uint32_t flags) {
    GLTexture2D* tex;

    // TODO
    // tex = GLPool<GLTexture2D>::GLObjectPool::Pop(GLPool<GLTexture2D>::m_pool + 131080);
    tex = nullptr;

    if (!tex) {
        tex = new GLTexture2D();
    }

    // TODO
    // Blizzard::Debug::Assert(tex->m_refCount == 0);

    tex->m_RefCount = 1;

    // TODO
    // Blizzard::Debug::Assert(tex->m_TextureID >= PoolStats<GLTexture2D>::NAME_POOL_FIRST_NAME);

    tex->m_TextureType = GL_TEXTURE_2D;
    tex->m_Width = width;
    tex->m_Depth = 1;
    tex->m_Height = height;
    tex->m_Format = format;
    tex->m_NumMipmap = numMipMap;
    tex->m_RequestedNumMipmaps = numMipMap;
    tex->m_Flags = flags;

    tex->SetupTexture();

    return tex;
}

GLTexture2D::GLTexture2D() : GLTexture() {
    this->m_TextureType = GL_TEXTURE_2D;
    this->m_TextureID = GLPool<GLTexture2D>::Get()->GetNextName();

    // TODO
    // Blizzard::Debug::Assert(this->m_TextureID >= PoolStats<GLTexture2D>::NAME_POOL_FIRST_NAME);
}

void GLTexture2D::ReleaseObject() {
    BLIZZARD_ASSERT(this->m_TextureType == GL_TEXTURE_2D);
    this->FreeTexture();
}

void GLSDLTexSetFlags(CGxTex* texId, GLTexture* a2) {
    static GLEnum convertMagFilterToOgl[] = {
        GL_NEAREST,
        GL_LINEAR,
        GL_NEAREST,
        GL_LINEAR,
        GL_LINEAR,
        GL_LINEAR
    };

    static GLEnum convertMinFilterToOgl[] = {
        GL_NEAREST,
        GL_LINEAR,
        GL_NEAREST_MIPMAP_NEAREST,
        GL_LINEAR_MIPMAP_NEAREST,
        GL_LINEAR_MIPMAP_LINEAR,
        GL_LINEAR_MIPMAP_LINEAR
    };

    a2->SetMagFilterMode(convertMagFilterToOgl[texId->m_flags.m_filter]);
    a2->SetMinFilterMode(convertMinFilterToOgl[texId->m_flags.m_filter]);

    a2->SetAddressModeS(texId->m_flags.m_wrapU ? GL_REPEAT : GL_CLAMP_TO_EDGE);
    a2->SetAddressModeT(texId->m_flags.m_wrapV ? GL_REPEAT : GL_CLAMP_TO_EDGE);

    a2->SetMaxAnisotropy(texId->m_flags.m_maxAnisotropy);
}
