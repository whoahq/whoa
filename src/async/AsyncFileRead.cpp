#include "async/AsyncFileRead.hpp"
#include "util/SFile.hpp"
#include <common/Prop.hpp>
#include <common/Time.hpp>
#include <storm/Error.hpp>

uint32_t AsyncFileRead::s_threadSleep;
uint32_t AsyncFileRead::s_handlerTimeout = 100;
CAsyncObject* AsyncFileRead::s_asyncWaitObject;
void* AsyncFileRead::s_progressCallback;
void* AsyncFileRead::s_progressParam;
int32_t AsyncFileRead::s_progressCount;
void* AsyncFileRead::s_ingameProgressCallback;
void* AsyncFileRead::s_ingameStartCallback;
void* AsyncFileRead::s_propContext;
SEvent AsyncFileRead::s_shutdownEvent = SEvent(1, 0);
const char* AsyncFileRead::s_asyncQueueNames[NUM_ASYNC_QUEUES] = {
    "Disk Queue",
    "Net Geometry Queue",
    "Net Texture Queue"
};
CAsyncQueue* AsyncFileRead::s_asyncQueues[NUM_ASYNC_QUEUES];
SCritSect AsyncFileRead::s_queueLock;
SCritSect AsyncFileRead::s_userQueueLock;
TSList<CAsyncQueue, TSGetLink<CAsyncQueue>> AsyncFileRead::s_asyncQueueList;
TSList<CAsyncThread, TSGetLink<CAsyncThread>> AsyncFileRead::s_asyncThreadList;
STORM_EXPLICIT_LIST(CAsyncObject, link) AsyncFileRead::s_asyncFileReadPostList;
STORM_EXPLICIT_LIST(CAsyncObject, link) AsyncFileRead::s_asyncFileReadFreeList;
int32_t AsyncFileRead::s_waiting;

CAsyncQueue* AsyncFileReadCreateQueue() {
    CAsyncQueue* queue = AsyncFileRead::s_asyncQueueList.NewNode(0, 2, 0x8);
    return queue;
}

void AsyncFileReadCreateThread(CAsyncQueue* queue, const char* queueName) {
    CAsyncThread* thread = AsyncFileRead::s_asyncThreadList.NewNode(0, 2, 0x8);

    thread->queue = queue;
    thread->currentObject = nullptr;

    SThread::Create(AsyncFileReadThread, thread, thread->thread, const_cast<char*>(queueName), 0);
}

void AsyncFileReadLinkObject(CAsyncObject* object, int32_t a2) {
    if (!object->queue) {
        return;
    }

    object->link.Unlink();

    auto& readList = object->queue->readList;

    for (auto currentObject = readList.Head(); currentObject; currentObject = readList.Link(currentObject)->Next()) {
        uint8_t priority = object->priority;
        uint8_t currentPriority = currentObject->priority;

        if (priority <= currentPriority && (a2 || priority != currentPriority)) {
            readList.LinkNode(object, 2, currentObject);
            object->char25 = 0;

            return;
        }
    }

    readList.LinkToTail(object);
    object->char25 = 0;
}

int32_t AsyncFileReadPollHandler(const void* a1, void* a2) {
    uint32_t start = OsGetAsyncTimeMsPrecise();

    while (1) {
        AsyncFileRead::s_queueLock.Enter();

        CAsyncObject* object = AsyncFileRead::s_asyncFileReadPostList.Head();

        if (!object) {
            AsyncFileRead::s_queueLock.Leave();
            break;
        }

        AsyncFileRead::s_asyncFileReadPostList.UnlinkNode(object);

        if (AsyncFileRead::s_asyncWaitObject == object) {
            AsyncFileRead::s_asyncWaitObject = nullptr;
        }

        object->isProcessed = 1;

        AsyncFileRead::s_queueLock.Leave();

        object->userPostloadCallback(object->userArg);

        AsyncFileRead::s_progressCount--;

        // Check if we're exceeded the allowed running time
        if (OsGetAsyncTimeMsPrecise() - start > AsyncFileRead::s_handlerTimeout) {
            break;
        }
    }

    // TODO
    // for (int32_t i = 0; i < DwordB4A224; i++) {
    //     DwordB4A228[i]();
    // }

    return 1;
}

uint32_t AsyncFileReadThread(void* param) {
    CAsyncThread* thread = static_cast<CAsyncThread*>(param);

    PropSelectContext(AsyncFileRead::s_propContext);

    while (AsyncFileRead::s_shutdownEvent.Wait(0)) {
        uint32_t sleep = 0;
        CAsyncObject* object;

        while (1) {
            AsyncFileRead::s_queueLock.Enter();

            object = thread->queue->readList.Head();

            if (object && thread->queue->int20 && /* TODO */ true) {
                // TODO
                // Sub4BA530(object, 1);

                AsyncFileRead::s_queueLock.Leave();
                continue;
            }

            if (!object) {
                object = thread->queue->list14.Head();
            }

            if (!object) {
                AsyncFileRead::s_queueLock.Leave();
                break;
            }

            object->link.Unlink();
            object->queue = nullptr;
            object->isCurrent = 1;
            thread->currentObject = object;

            AsyncFileRead::s_queueLock.Leave();

            int32_t tries = 10;
            while (1) {
                if (SFile::IsStreamingMode() && object->file) {
                    // TODO
                    // Sub421820(object->file, (object->priority > 127) + 1, 1);
                }

                if (SFile::Read(object->file, object->buffer, object->size, nullptr, nullptr, nullptr)) {
                    break;
                }

                tries--;

                // Handle failure
                if (tries == 0) {
                    // TODO
                    // Sub421850((object->file, v17, 512);
                    // v10 = Sub7717E0();
                    // Sub771A80(v10, v18, 512);
                    // nullsub_3(v17);

                    break;
                }
            }

            AsyncFileRead::s_queueLock.Enter();

            AsyncFileRead::s_asyncFileReadPostList.LinkToTail(object);

            thread->currentObject = nullptr;
            object->isCurrent = 0;
            object->isRead = 1;

            AsyncFileRead::s_queueLock.Leave();

            if (AsyncFileRead::s_threadSleep) {
                sleep++;

                if (sleep == AsyncFileRead::s_threadSleep) {
                    OsSleep(1);
                    sleep = 0;
                }
            }
        }

        OsSleep(1);
    }

    return 0;
}

void AsyncFileReadWait(CAsyncObject* object) {
    STORM_ASSERT(object);

    AsyncFileRead::s_waiting++;

    AsyncFileRead::s_queueLock.Enter();

    if (object->isProcessed) {
        AsyncFileRead::s_queueLock.Leave();
        return;
    }

    AsyncFileRead::s_asyncWaitObject = object;

    if (!object->isCurrent && !object->isRead) {
        object->link.Unlink();
        object->queue->readList.LinkToHead(object);
    }

    AsyncFileRead::s_queueLock.Leave();

    if (SFile::IsStreamingMode()) {
        // TODO
    }

    // TODO

    if (AsyncFileRead::s_ingameStartCallback) {
        // TODO AsyncFileRead::s_ingameStartCallback();
    }

    while (true) {
        if (AsyncFileRead::s_ingameProgressCallback) {
            // TODO AsyncFileRead::s_ingameProgressCallback(0.0, 0);
        }

        AsyncFileReadPollHandler(nullptr, nullptr);

        if (!AsyncFileRead::s_asyncWaitObject) {
            break;
        }

        OsSleep(1);
    }

    AsyncFileRead::s_waiting--;
}
