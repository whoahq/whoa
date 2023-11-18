#ifndef GX_GL_SDL_GL_VERTEX_ARRAY_HPP
#define GX_GL_SDL_GL_VERTEX_ARRAY_HPP

#include "gx/glsdl/GLBuffer.hpp"
#include "gx/glsdl/GLObject.hpp"
#include "gx/glsdl/GLVertexFormat.hpp"
#include <cstdint>

class GLSDLDevice;

class GLVertexArray : public GLObject {
    public:
        // Types
        struct Properties {
            GLVertexFormat* m_VertexBufferFormat = nullptr;
            GLBuffer* m_IndexBuffer = nullptr;
            GLBuffer* m_PixelPackBuffer = nullptr;
            GLBuffer* m_PixelUnpackBuffer = nullptr;
            GLBuffer* m_VertexBuffer[4] = {};
            uint32_t m_VertexBase = 0;
            uint32_t m_VertexBufferOffset[4] = {};
            uint32_t m_VertexBufferStride[4] = {};
        };

        // Static variables
        static bool s_VertexArrayEnable;

        // Static functions
        static void FindVertexArray(GLSDLDevice*, GLVertexArray&);

        // Member variables
        Properties m_Properties;
        GLStates::VertexArrayObject m_GLStates;
        uint32_t m_VertexArrayID = 0;

        // Virtual member functions
        virtual void ReleaseObject();

        // Member functions
        GLVertexArray(bool);
        void ApplyGLStates(GLStates::VertexArrayObject&);
        void ApplyVertexFormat(GLSDLDevice*);
        Properties& GetProperties(void);
};

#endif
