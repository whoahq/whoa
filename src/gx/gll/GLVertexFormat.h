#ifndef GX_GLL_GL_VERTEX_FORMAT_H
#define GX_GLL_GL_VERTEX_FORMAT_H

#include "gx/gll/GLTypes.h"
#include <cstdint>

class GLVertexFormat {
    public:
        uint32_t m_Size;
        GLVertexAttrib m_Attribs[16];
};

#endif
