#ifndef GX_GL_SDL_GL_FRAMEBUFFER_HPP
#define GX_GL_SDL_GL_FRAMEBUFFER_HPP

#include "gx/glsdl/GL.hpp"
#include "gx/glsdl/GLObject.hpp"
#include <cstdint>

#define MAX_ATTACHMENT 6

class GLSDLDevice;
class GLMipmap;

class GLFramebuffer : public GLObject {
    public:
        // Static functions
        static GLFramebuffer* Create(bool);

        // Member variables
        int32_t m_Width = 0;
        int32_t m_Height = 0;
        uint32_t m_FramebufferID = 0;
        GLSDLDevice* m_Device;
        GLMipmap* m_Attachments[6] = {};
        uint32_t m_NumAttach = 0;

        // Virtual member functions
        virtual void ReleaseObject();

        // Member functions
        GLFramebuffer(bool);
        void Attach(GLMipmap*, GLenum, int32_t);
        void Detach(GLenum);
        GLMipmap* GetAttachment(GLEnum);
        int32_t GetSampleCount(void);
        bool IsValid();
};

#endif
