#ifndef GX_GL_SDL_GL_VERTEX_FORMAT_HPP
#define GX_GL_SDL_GL_VERTEX_FORMAT_HPP

#include "gx/glsdl/GLTypes.hpp"
#include <cstdint>

class GLVertexFormat {
    public:
        uint32_t m_Size;
        GLVertexAttrib m_Attribs[16];
};

#endif
