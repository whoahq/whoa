#ifndef GX_GL_SDL_GL_GLSL_PROGRAM_HPP
#define GX_GL_SDL_GL_GLSL_PROGRAM_HPP

#include "gx/glsdl/GLObject.hpp"

class GLShader;

class GLGLSLProgram : public GLObject {
    public:
        // Static functions
        static GLGLSLProgram* Find(GLShader*, GLShader*);
};

#endif
