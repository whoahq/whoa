#include "gx/glsdl/GLShader.hpp"
#include "gx/glsdl/GLSDLDevice.hpp"
#include "gx/glsdl/GLPixelShader.hpp"
#include "gx/glsdl/GLPool.hpp"
#include "gx/glsdl/GLVertexShader.hpp"
#include "gx/glsdl/GL.hpp"
#include <bc/Debug.hpp>

// TODO
// - threaded compiler support
// - glsl support
// - hybrid support
GLShader* GLShader::Create(ShaderType shaderType, bool hybrid, bool usingCG, const char* a4, const void* buf, int32_t codeLen, const char* a7, const char* name, GLShaderLogInfo* logInfo) {
    const char* shaderCode = reinterpret_cast<const char*>(buf);

    if (*reinterpret_cast<const int32_t*>(buf) == 'GSL1') {
        BLIZZARD_ASSERT(!usingCG);

        const ShaderDataHeader header = *reinterpret_cast<const ShaderDataHeader*>(buf);

        BLIZZARD_ASSERT(header.shaderType == shaderType);
        BLIZZARD_ASSERT(header.size == codeLen);
        BLIZZARD_ASSERT(header.codePos >= sizeof(ShaderDataHeader));
        BLIZZARD_ASSERT(header.codeSize > 0);

        shaderCode = &reinterpret_cast<const char*>(buf)[header.codePos];
    }

    GLShader* shader = nullptr;

    if (shaderType == ePixelShader) {
        shader = GLPixelShader::Create();
    } else if (shaderType == eVertexShader) {
        shader = GLVertexShader::Create();
    } else {
        // TODO
        // sub_1C5E0(&v38, "Unknown shader type %d!", shaderType);
    }

    shader->m_UsingCG = usingCG;

    if (usingCG) {
        shader->CompileCG(a4, shaderCode, codeLen, a7, logInfo);
    } else {
        shader->m_Code.assign(shaderCode, codeLen);

        // TODO
        // sub_5CD10(shader);
    }

    return shader;
}

bool GLShader::CheckErrorsARB(GLShaderLogInfo* logInfo) {
    GLint errorPos;

    glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorPos);
    const GLubyte* errorStr = glGetString(GL_PROGRAM_ERROR_STRING_ARB);

    // TODO
    // Blizzard::Debug::VAssert(logInfo != 0 || errorPos == -1, errorStr);

    return errorPos == -1;
}

void GLShader::Compile(GLShaderLogInfo* logInfo) {
    this->ImmediateCompile(logInfo);
}

void GLShader::CompileCG(const char* a2, const void* shaderCode, int32_t codeLen, const char* a5, GLShaderLogInfo* logInfo) {
    // TODO
}

void GLShader::FlushUniforms(GLGLSLProgram* program) {
    // TODO
}

std::string& GLShader::GetCode() {
    return this->m_Code;
}

int32_t GLShader::GetShaderType() {
    return this->m_ShaderType;
}

void GLShader::ImmediateCompile(GLShaderLogInfo* logInfo) {
    BLIZZARD_ASSERT(!this->GetCode().empty());

    this->m_Device = GLSDLDevice::Get();

    if (!this->m_UsingGLSL) {
        if (!this->m_ShaderID) {
            if (this->m_ShaderType == eVertexShader) {
                this->m_ShaderID = GLPool<GLVertexShader>::Get()->GetNextName();
            } else {
                this->m_ShaderID = GLPool<GLPixelShader>::Get()->GetNextName();
            }
        }

        this->m_Device->BindShader(this);

        const char* arbCode = this->GetCode().c_str();
        size_t arbLen = strlen(arbCode);

        glProgramStringARB(this->var5, GL_PROGRAM_FORMAT_ASCII_ARB, arbLen, arbCode);

        BLIZZARD_ASSERT(this->CheckErrorsARB(logInfo));
    } else {
        // TODO
        // - handle GLSL shaders
        // - handle hybrid shaders
    }

    if (logInfo) {
        // TODO
        // this->var20 = logInfo[0];
    } else {
        this->var20 = 1;
    }

    // TODO
    // this->m_TimeStamp = Blizzard::Time::GetTimestamp();
}

bool GLShader::IsEnabled() {
    return this->m_Enabled;
}

void GLShader::ReleaseObject() {
    // TODO
}

void GLShader::SetShaderConstants(ShaderType shaderType, uint32_t index, const float* constants, uint32_t count) {
    // TODO
}
