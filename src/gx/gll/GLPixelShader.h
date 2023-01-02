#ifndef GX_GLL_GL_PIXEL_SHADER_H
#define GX_GLL_GL_PIXEL_SHADER_H

#include "gx/gll/GLShader.h"

class GLPixelShader : public GLShader {
    public:
        // Static functions
        static GLPixelShader* Create(void);
};

#endif
