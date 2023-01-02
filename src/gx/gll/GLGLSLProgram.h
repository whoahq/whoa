#ifndef GX_GLL_GL_GLSL_PROGRAM_H
#define GX_GLL_GL_GLSL_PROGRAM_H

#include "gx/gll/GLObject.h"

class GLShader;

class GLGLSLProgram : public GLObject {
    public:
        // Static functions
        static GLGLSLProgram* Find(GLShader*, GLShader*);
};

#endif
