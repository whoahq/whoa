#ifndef ASYNC_ASYNC_FILE_READ_HPP
#define ASYNC_ASYNC_FILE_READ_HPP

#include "async/CAsyncQueue.hpp"
#include "async/CAsyncThread.hpp"
#include <common/Prop.hpp>
#include <storm/Thread.hpp>

#define NUM_ASYNC_QUEUES 3

class CAsyncObject;

class AsyncFileRead {
    public:
        // Static variables
        static uint32_t s_threadSleep;
        static uint32_t s_handlerTimeout;
        static CAsyncObject* s_asyncWaitObject;
        static void* s_progressCallback;
        static void* s_progressParam;
        static int32_t s_progressCount;
        static void* s_ingameProgressCallback;
        static void* s_ingameStartCallback;
        static HPROPCONTEXT s_propContext;
        static SEvent s_shutdownEvent;
        static const char* s_asyncQueueNames[];
        static CAsyncQueue* s_asyncQueues[];
        static SCritSect s_queueLock;
        static SCritSect s_userQueueLock;
        static TSList<CAsyncQueue, TSGetLink<CAsyncQueue>> s_asyncQueueList;
        static TSList<CAsyncThread, TSGetLink<CAsyncThread>> s_asyncThreadList;
        static STORM_EXPLICIT_LIST(CAsyncObject, link) s_asyncFileReadPostList;
        static STORM_EXPLICIT_LIST(CAsyncObject, link) s_asyncFileReadFreeList;
};

CAsyncQueue* AsyncFileReadCreateQueue(void);

void AsyncFileReadCreateThread(CAsyncQueue* queue, const char* queueName);

void AsyncFileReadLinkObject(CAsyncObject* object, int32_t a2);

int32_t AsyncFileReadPollHandler(const void*, void*);

uint32_t AsyncFileReadThread(void* thread);

#endif
