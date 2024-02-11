#ifndef GX_GL_SDL_GL_OBJECT_HPP
#define GX_GL_SDL_GL_OBJECT_HPP

#include <cstdint>

class GLObject {
    public:
        // Member variables
        GLObject* m_Next;
        uint32_t m_RefCount;
        int64_t m_TimeStamp;

        // Virtual member functions
        virtual void ReleaseObject() = 0;
        virtual void AddRefTwin();
        virtual void ReleaseTwin();

        // Member functions
        uint32_t Release();
};

#endif
