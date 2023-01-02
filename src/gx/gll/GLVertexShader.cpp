#include "gx/gll/GLVertexShader.h"
#include "gx/gll/GL.h"

GLVertexShader* GLVertexShader::Create() {
    // TODO
    // GLPool stuff

    GLVertexShader* shader = new GLVertexShader();

    shader->m_ShaderID = 0;
    shader->m_RefCount = 1;
    shader->m_ShaderType = eVertexShader;
    shader->m_UsingGLSL = 0;
    shader->var5 = GL_VERTEX_PROGRAM_ARB;

    return shader;
}
