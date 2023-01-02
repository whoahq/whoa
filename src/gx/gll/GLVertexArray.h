#ifndef GX_GLL_GL_VERTEX_ARRAY_H
#define GX_GLL_GL_VERTEX_ARRAY_H

#include "gx/gll/GLBuffer.h"
#include "gx/gll/GLObject.h"
#include "gx/gll/GLVertexFormat.h"
#include <cstdint>

class GLDevice;

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
        static void FindVertexArray(GLDevice*, GLVertexArray&);

        // Member variables
        Properties m_Properties;
        GLStates::VertexArrayObject m_GLStates;
        uint32_t m_VertexArrayID = 0;

        // Virtual member functions
        virtual void ReleaseObject();

        // Member functions
        GLVertexArray(bool);
        void ApplyGLStates(GLStates::VertexArrayObject&);
        void ApplyVertexFormat(GLDevice*);
        Properties& GetProperties(void);
};

#endif
