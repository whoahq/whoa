#ifndef GX_GL_SDL_GL_SHADER_INPUT_HPP
#define GX_GL_SDL_GL_SHADER_INPUT_HPP

#include <string>

class GLShader;

class GLShaderInput {
    public:
        // Member variables
        GLShader *m_Shader;
        std::basic_string<char, std::char_traits<char>, std::allocator<char>> m_Name;
        std::basic_string<char, std::char_traits<char>, std::allocator<char>> var2;
        int16_t m_MapIndex;
        int16_t var4;
        int16_t var5;
        int16_t m_UsedSize;
        int16_t var7;
        int16_t var8;
        GLShaderInput *m_Parent;
        int32_t var10;
        int32_t var11;
        int32_t var12;
        int8_t m_ChildrenCount;
        int8_t var14;
        uint16_t var15;
        int8_t var16;
        int8_t m_Variability;
        int8_t var18;
        int8_t var19;
};

#endif
