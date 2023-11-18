#ifndef GX_GL_SDL_GL_VERTEX_SHADER_HPP
#define GX_GL_SDL_GL_VERTEX_SHADER_HPP

#include "gx/glsdl/GLShader.hpp"

class GLVertexShader : public GLShader {
    public:
        // Static functions
        static GLVertexShader* Create(void);
};

#endif
