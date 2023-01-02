#include "gx/gll/GLPixelShader.h"
#include "gx/gll/GL.h"

GLPixelShader* GLPixelShader::Create() {
    // TODO
    // GLPool stuff

    GLPixelShader* shader = new GLPixelShader();

    shader->m_ShaderID = 0;
    shader->m_RefCount = 1;
    shader->m_ShaderType = ePixelShader;
    shader->m_UsingGLSL = 0;
    shader->var5 = GL_FRAGMENT_PROGRAM_ARB;

    return shader;
}
