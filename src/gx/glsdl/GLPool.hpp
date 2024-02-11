#ifndef GX_GL_SDL_GL_POOL_HPP
#define GX_GL_SDL_GL_POOL_HPP

#include <atomic>
#include <cstdint>

template<class T>
class GLPool {
    public:
        // Static variables
        static GLPool<T>* m_pool;

        // Static functions
        static GLPool<T>* Get(void);
        static void Init(void);

        // Member variables
        std::atomic<uint32_t> m_NextName;

        // Member functions
        uint32_t GetNextName(void);
        T* GetNextObject(void);
        void SetNextName(uint32_t);
};

template<class T>
GLPool<T>* GLPool<T>::m_pool;

template<class T>
GLPool<T>* GLPool<T>::Get() {
    return GLPool<T>::m_pool;
}

template<class T>
void GLPool<T>::Init() {
    GLPool<T>::m_pool = new GLPool<T>();
}

template<class T>
uint32_t GLPool<T>::GetNextName() {
    return this->m_NextName++;
}

template<class T>
T* GLPool<T>::GetNextObject() {
    // TODO
    // - pop off of GLObjectPool

    return new T();
}

template<class T>
void GLPool<T>::SetNextName(uint32_t name) {
    this->m_NextName = name;
}

#endif
