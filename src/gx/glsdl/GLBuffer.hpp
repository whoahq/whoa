#ifndef GX_GL_SDL_GL_BUFFER_HPP
#define GX_GL_SDL_GL_BUFFER_HPP

#include "gx/glsdl/GL.hpp"
#include "gx/glsdl/GLObject.hpp"
#include "gx/glsdl/GLTypes.hpp"

class GLBuffer : public GLObject {
    public:
        // Types
        enum eMapFlag {
            GLMap_NotMapped = -1,
            GLMap_None = 0,
            GLMap_Unk1 = 1,
            GLMap_Unk2 = 2,
            GLMap_Unk3 = 3,
            GLMap_Count = 4
        };

        // Static variables
        static bool m_UsingVBO;
        static GLEnum s_FlagToAccess[];

        // Static functions
        static GLBuffer* Create(GLEnum, uint32_t, const void*, GLEnum, GLEnum);

        // Member variables
        uint32_t m_Size = 0;
        GLEnum m_Type = 0;
        GLEnum m_Usage = 0;
        uint32_t m_BufferID = 0;
        GLEnum m_IndexFormat = 0;
        char* m_Data = nullptr;
        uint32_t m_MapOffset = 0;
        uint32_t m_MapSize = 0;
        uint32_t m_MapFlag = GLMap_NotMapped;

        // Virtual member functions
        virtual void ReleaseObject();

        // Member functions
        GLBuffer();
        char* Map(uint32_t, uint32_t, eMapFlag);
        void Unmap(uint32_t);
};

#endif
