#ifndef GX_GLL_GL_OBJECT_H
#define GX_GLL_GL_OBJECT_H

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
