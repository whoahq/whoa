#ifndef GX_SDL_GL_PIXEL_SHADER_HPP
#define GX_SDL_GL_PIXEL_SHADER_HPP

#include "gx/glsdl/GLShader.hpp"

class GLPixelShader : public GLShader {
    public:
        // Static functions
        static GLPixelShader* Create(void);
};

#endif
