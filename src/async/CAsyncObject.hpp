#ifndef ASYNC_C_ASYNC_OBJECT_HPP
#define ASYNC_C_ASYNC_OBJECT_HPP

#include <storm/List.hpp>

class SFile;
class CAsyncQueue;

typedef void (*ASYNC_CALLBACK)(void*);

class CAsyncObject {
    public:
        // Member variables
        SFile* file;
        void* buffer;
        uint32_t size;
        void* userArg;
        ASYNC_CALLBACK userPostloadCallback;
        ASYNC_CALLBACK userFailedCallback;
        CAsyncQueue* queue;
        void* ptr1C;
        uint8_t priority;
        uint8_t isProcessed;
        uint8_t isRead;
        uint8_t isCurrent;
        uint8_t char24;
        uint8_t char25;
        uint8_t padding[2];
        TSLink<CAsyncObject> link;
};

#endif
