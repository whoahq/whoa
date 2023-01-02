#include "async/AsyncFile.hpp"
#include "async/AsyncFileRead.hpp"
#include "async/CAsyncQueue.hpp"
#include "event/Event.hpp"
#include "util/SFile.hpp"
#include <common/Prop.hpp>
#include <common/Time.hpp>

CAsyncObject* AsyncFileReadAllocObject() {
    AsyncFileRead::s_queueLock.Enter();

    CAsyncObject* object = AsyncFileRead::s_asyncFileReadFreeList.Head();

    if (!object) {
        object = AsyncFileRead::s_asyncFileReadFreeList.NewNode(1, 0, 0x0);
    }

    object->link.Unlink();

    AsyncFileRead::s_queueLock.Leave();

    object->file = nullptr;
    object->buffer = nullptr;
    object->size = 0;
    object->userArg = nullptr;
    object->userPostloadCallback = nullptr;
    object->userFailedCallback = nullptr;
    object->queue = nullptr;
    object->isProcessed = 0;
    object->isRead = 0;
    object->isCurrent = 0;
    object->char24 = 0;
    object->char25 = 0;
    object->ptr1C = 0;
    object->priority = 126;

    return object;
}

void AsyncFileReadDestroyObject(CAsyncObject* object) {
    AsyncFileRead::s_queueLock.Enter();

    if (object->isCurrent) {
        // TODO
        // nullsub_3();

        AsyncFileRead::s_queueLock.Leave();

        while (object->isCurrent) {
            OsSleep(1);
        }

        AsyncFileRead::s_queueLock.Enter();
    }

    object->link.Unlink();

    if (object->file) {
        SFile::Close(object->file);
    }

    AsyncFileRead::s_asyncFileReadFreeList.LinkToHead(object);

    AsyncFileRead::s_queueLock.Leave();
}

void AsyncFileReadInitialize(uint32_t threadSleep, uint32_t handlerTimeout) {
    AsyncFileRead::s_threadSleep = std::min(threadSleep, 100u);
    AsyncFileRead::s_handlerTimeout = std::max(handlerTimeout, 20u);

    EventRegisterEx(EVENT_ID_POLL, &AsyncFileReadPollHandler, nullptr, 0.0f);
    if (SFile::IsStreamingMode()) {
        // TODO
        // EventRegisterEx(EVENT_ID_IDLE, &Sub4B9F40, nullptr, -1.0f);
    }

    AsyncFileRead::s_asyncWaitObject = nullptr;
    AsyncFileRead::s_progressCallback = nullptr;
    AsyncFileRead::s_progressParam = nullptr;
    AsyncFileRead::s_ingameProgressCallback = nullptr;
    AsyncFileRead::s_ingameStartCallback = nullptr;
    AsyncFileRead::s_propContext = PropGetSelectedContext();

    AsyncFileRead::s_shutdownEvent.Reset();

    int32_t numQueues = SFile::IsStreamingMode() != 0 ? 3 : 1;
    for (int32_t i = 0; i < numQueues; i++) {
        CAsyncQueue* queue = AsyncFileReadCreateQueue();
        const char* queueName = AsyncFileRead::s_asyncQueueNames[i];

        AsyncFileRead::s_asyncQueues[i] = queue;
        AsyncFileReadCreateThread(queue, queueName);
    }

    if (SFile::IsStreamingMode()) {
        AsyncFileRead::s_asyncQueues[2]->int20 = 1;
    }

    // TODO
    // - Something related to AsyncFileRead::s_userQueueLock
}

void AsyncFileReadObject(CAsyncObject* object, int32_t a2) {
    CAsyncQueue* queue = AsyncFileRead::s_asyncQueues[0];

    if (SFile::IsStreamingMode()) {
        // TODO
        // int32_t v3 = SFile::FileIsLocal(object->file, 6);
        //
        // if (!v3 || v3 == 2) {
        //     int32_t v4 = object->priority <= 127;
        //     object->char24 = 1;
        //     queue = AsyncFileRead::s_asyncQueues[1];
        //
        //     if (!v4) {
        //         queue = AsyncFileRead::s_asyncQueues[2];
        //     }
        // }
    }

    AsyncFileRead::s_queueLock.Enter();

    object->queue = queue;

    if (AsyncFileRead::s_asyncWaitObject == object) {
        object->priority = object->priority > 127 ? 128 : 0;
        // TODO
        // object->ptr1C = g_theGxDevicePtr + 3944;
        object->char25 = 0;
    } else if (queue->int20) {
        // TODO
        // Sub4BA530(object, a2);
    } else {
        AsyncFileReadLinkObject(object, a2);
    }

    AsyncFileRead::s_queueLock.Leave();

    if (SFile::IsStreamingMode()) {
        // TODO
        // SFile::LogFileAccess(object->file, 0, 0);
    }
}
