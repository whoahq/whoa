#ifndef GX_GL_SDL_GL_MIPMAP_HPP
#define GX_GL_SDL_GL_MIPMAP_HPP

#include "gx/glsdl/GL.hpp"
#include "gx/glsdl/GLTypes.hpp"
#include <cstdint>
#include <vector>

class GLFramebuffer;
class GLTexture;

class GLMipmap {
    public:
        // Types
        struct MapParams {
            GLBox m_MapArea;
            int32_t m_MapMode = 0;
            int32_t m_Unk7 = 0;
            unsigned char* m_Unk8 = nullptr;
            int32_t m_Size = 0;
        };

        // Member variables
        GLTexture* m_Texture = nullptr;
        uint16_t m_Width = 0;
        uint16_t m_Height = 0;
        uint16_t m_Depth = 0;
        uint8_t m_Level = 0;
        uint8_t m_DepthBits = 0;
        uint32_t m_Size = 0;
        int32_t m_Target = 0;
        unsigned char* m_Data = nullptr; // TODO proper type
        MapParams* m_MapParams = nullptr;
        std::vector<GLAttachPoint>* m_AttachPoints = nullptr;
        uint32_t m_Unk20 = 0;
        uint32_t m_Unk24 = 0;

        // Member functions
        void Attach(GLFramebuffer*, GLenum, int32_t);
        void Detach(GLFramebuffer*, GLenum, bool);
        void DetachAll();
        int32_t GetDepthBits(void);
        GLTextureFormat GetFormat(void);
        TextureFormatInfo& GetFormatInfo(void);
        uint16_t GetHeight(void);
        int32_t GetPitch(void);
        GLTexture* GetTexture();
        uint32_t GetTextureID(void);
        uint16_t GetWidth(void);
        void* Map(GLEnum, const GLBox*);
        void* Map(GLEnum, const GLRect*);
        void ReleaseObject();
        void RemoveDebugMipmap();
        void ResetData(GLEnum, int32_t, unsigned char*);
        void ResetSize(uint32_t, uint32_t, uint32_t);
        void TexImage(const void*);
        void TexSubImage(const GLBox&, int32_t, const void*);
        void Unmap(void);
        void Unmap(MapParams*);
};

#endif
