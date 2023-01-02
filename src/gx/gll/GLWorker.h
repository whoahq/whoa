#ifndef GX_GLL_GL_WORKER_H
#define GX_GLL_GL_WORKER_H

#include <list>
#include <pthread.h>

class GLCommand;
class GLDevice;

class GLWorker {
    public:
        // Static functions
        static void* Worker(void*);

        // Member variables
        GLDevice* m_Device;
        std::list<GLCommand*> m_Commands;
        pthread_mutex_t m_Mutex1;
        pthread_mutex_t m_Mutex2;
        pthread_cond_t m_Cond1;
        pthread_cond_t m_Cond2;
        pthread_t m_Thread;
        bool m_UnkA0;
        bool m_UnkA1;

        // Member functions
        GLWorker(GLDevice*);
        void Lock(void);
        void Send(GLCommand*);
        void Signal(void);
        void Unlock(void);
        void WaitOnGLObjects(void);
};

#endif
