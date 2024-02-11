#ifndef GX_GL_SDL_GL_SHADER_HPP
#define GX_GL_SDL_GL_SHADER_HPP

#include "gx/glsdl/GLObject.hpp"
#include "gx/glsdl/GLShaderInput.hpp"
#include <cstdint>
#include <string>
#include <vector>

class GLSDLDevice;
class GLGLSLProgram;
class GLShaderLogInfo;

class GLShader : public GLObject {
    public:
        // Types
        enum ShaderType {
            eVertexShader = 1,
            ePixelShader = 2,
            eShaderTypeCount = 3
        };

        struct ShaderDataHeader {
            uint32_t signature;
            uint32_t size;
            ShaderType shaderType;
            uint32_t codePos;
            uint32_t codeSize;
            uint32_t unk1;
            uint32_t unk2;
            uint32_t unk3;
        };

        // Static functions
        static GLShader* Create(ShaderType, bool, bool, const char*, const void*, int32_t, const char*, const char*, GLShaderLogInfo*);

        // Member variables
        int32_t m_ShaderType = 0;
        int32_t var5 = 0;
        uint32_t m_ShaderID = 0;
        bool m_UsingCG = false;
        bool m_UsingGLSL = false;
        uint32_t m_UniformRegisterCount = 0;
        GLShaderInput** var10 = nullptr;
        float* var11 = nullptr;
        bool var12 = false;
        uint32_t var13 = 0;
        uint32_t var14 = 0;
        std::vector<GLShaderInput*, std::allocator<GLShaderInput*>> var15;
        std::vector<GLShaderInput*, std::allocator<GLShaderInput*>> var16;
        std::vector<GLShaderInput*, std::allocator<GLShaderInput*>> var17;
        GLShader* var18 = nullptr;
        GLSDLDevice* m_Device = nullptr;
        bool var20 = 0;
        bool m_Enabled = true;
        std::basic_string<char, std::char_traits<char>, std::allocator<char>> m_Code;
        std::basic_string<char, std::char_traits<char>, std::allocator<char>> var23;

        // Virtual member functions
        virtual void ReleaseObject();

        // Member functions
        bool CheckErrorsARB(GLShaderLogInfo*);
        bool CheckErrorsGLSL(GLShaderLogInfo*);
        void Compile(GLShaderLogInfo*);
        void CompileCG(const char*, const void*, int32_t, const char*, GLShaderLogInfo*);
        void FlushUniforms(GLGLSLProgram*);
        std::string& GetCode(void);
        int32_t GetShaderType(void);
        void ImmediateCompile(GLShaderLogInfo*);
        bool IsEnabled(void);
        void SetShaderConstants(ShaderType, uint32_t, const float*, uint32_t);
};

#endif
