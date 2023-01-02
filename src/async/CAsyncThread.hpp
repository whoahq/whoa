#ifndef ASYNC_C_ASYNC_THREAD_HPP
#define ASYNC_C_ASYNC_THREAD_HPP

#include <cstdint>
#include <storm/List.hpp>
#include <storm/Thread.hpp>

class CAsyncObject;
class CAsyncQueue;

class CAsyncThread : public TSLinkedNode<CAsyncThread> {
    public:
        // Member variables
        SThread thread;
        CAsyncQueue* queue;
        CAsyncObject* currentObject;
};

#endif
