#ifndef GX_GL_SDL_GL_HPP
#define GX_GL_SDL_GL_HPP

#include <GL/glew.h>
#include <GL/gl.h>

#include "gx/glsdl/GLTypes.hpp"

typedef GLenum GLEnum;

#define kMAX_VERTEX_ATTRIBS 16

struct TextureFormatInfo {
    GLenum m_InternalFormat;
    GLenum m_DataFormat;
    GLenum m_DataType;
    int32_t m_IsCompressed;
    int32_t m_BytePerPixel;
    char m_Name[16];
};

struct VertexTypeInfo {
    GLenum m_Type;
    GLint m_Size;
    GLboolean m_Normalized;
    GLint m_ByteSize;
    const char* m_Name;
};

extern TextureFormatInfo k_TextureFormatInfo[GLTF_NUM_TEXTURE_FORMATS];
extern VertexTypeInfo k_VertexTypeInfo[GLVT_NUM_VERTEX_TYPES];

#endif
