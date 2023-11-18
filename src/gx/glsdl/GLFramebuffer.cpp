#include "gx/glsdl/GLFramebuffer.hpp"
#include "gx/glsdl/GLSDLDevice.hpp"
#include "gx/glsdl/GLMipmap.hpp"
#include "gx/glsdl/GLPool.hpp"
#include "gx/glsdl/GLTypes.hpp"
#include <bc/Debug.hpp>

GLFramebuffer* GLFramebuffer::Create(bool a1) {
    GLFramebuffer* framebuffer = new GLFramebuffer(a1);

    if (!a1) {
        // TODO
        // BLIZZARD_ASSERT(framebuffer->m_FramebufferID >= PoolStats<GLFramebuffer>::NAME_POOL_FIRST_NAME);
    }

    BLIZZARD_ASSERT(framebuffer->m_NumAttach == 0);

    framebuffer->m_Width = 0;
    framebuffer->m_Height = 0;
    framebuffer->m_Device = GLSDLDevice::Get();

    return framebuffer;
}

GLFramebuffer::GLFramebuffer(bool a1) : GLObject() {
    if (!a1) {
        this->m_FramebufferID = GLPool<GLFramebuffer>::Get()->GetNextName();
    }
}

void GLFramebuffer::Attach(GLMipmap* image, GLenum a3, int32_t a4) {
    BLIZZARD_ASSERT(this->m_Device == GLSDLDevice::Get());

    if (!image) {
        this->Detach(a3);
        return;
    }

    if (a3 == GL_DEPTH_STENCIL) {
        BLIZZARD_ASSERT(image->GetFormat() == GLTF_D24S8);

        this->Attach(image, GL_DEPTH_ATTACHMENT, 0);
        this->Attach(image, GL_STENCIL_ATTACHMENT, 0);

        (*image->m_AttachPoints)[this->m_FramebufferID].point = GL_DEPTH_STENCIL;

        return;
    }

    int32_t index;

    if (a3 == GL_DEPTH_ATTACHMENT) {
        index = 4;
    } else if (a3 == GL_STENCIL_ATTACHMENT) {
        index = 5;
    } else {
        index = a3 - GL_COLOR_ATTACHMENT0;
    }

    BLIZZARD_ASSERT(index < MAX_ATTACHMENT);

    GLMipmap* oldImage = this->m_Attachments[index];

    if (image != oldImage) {
        if (oldImage) {
            oldImage->Detach(this, a3, true);
        } else {
            ++this->m_NumAttach;
        }

        this->m_Attachments[index] = image;

        this->m_Width = image->m_Width;
        this->m_Height = image->m_Height;

        image->Attach(this, a3, a4);
        this->m_Device->Sub38460(0);
    }

    BLIZZARD_ASSERT((*image->m_AttachPoints)[m_FramebufferID].framebuffer == this);
}

void GLFramebuffer::Detach(GLenum a2) {
    int32_t v2 = a2;
    int32_t index;

    if (a2 == GL_DEPTH_STENCIL) {
        index = 5;
        v2 = GL_STENCIL_ATTACHMENT;
        this->Detach(GL_DEPTH_ATTACHMENT);
    } else if (a2 == GL_DEPTH_ATTACHMENT) {
        index = 4;
    } else if (a2 == GL_STENCIL_ATTACHMENT) {
        index = 5;
    } else {
        index = a2 - GL_COLOR_ATTACHMENT0;
    }

    BLIZZARD_ASSERT(index < MAX_ATTACHMENT);

    GLMipmap* oldImage = this->m_Attachments[index];

    if (oldImage) {
        oldImage->Detach(this, v2, 0);

        --this->m_NumAttach;

        this->m_Attachments[index] = 0;

        if (this->m_Device == GLSDLDevice::Get()) {
            this->m_Device->Sub38460(0);
        }

        if (this->m_NumAttach == 0) {
            this->m_Width = 0;
            this->m_Height = 0;
        }
    }
}

GLMipmap* GLFramebuffer::GetAttachment(GLEnum a2) {
    int32_t index;

    if (a2 == GL_DEPTH_ATTACHMENT) {
        index = 4;
    } else if (a2 == GL_STENCIL_ATTACHMENT) {
        index = 5;
    } else {
        index = a2 - GL_COLOR_ATTACHMENT0;
    }

    BLIZZARD_ASSERT(index < MAX_ATTACHMENT);

    return this->m_Attachments[index];
}

int32_t GLFramebuffer::GetSampleCount() {
    return this->m_FramebufferID
        ? 1
        : this->m_Device->m_Context.GetSampleCount();
}

bool GLFramebuffer::IsValid() {
    auto status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
    return status == GL_FRAMEBUFFER_COMPLETE;
}

void GLFramebuffer::ReleaseObject() {
    // TODO
}
