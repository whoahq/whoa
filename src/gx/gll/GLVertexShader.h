#ifndef GX_GLL_GL_VERTEX_SHADER_H
#define GX_GLL_GL_VERTEX_SHADER_H

#include "gx/gll/GLShader.h"

class GLVertexShader : public GLShader {
    public:
        // Static functions
        static GLVertexShader* Create(void);
};

#endif
