#ifndef ASYNC_ASYNC_FILE_HPP
#define ASYNC_ASYNC_FILE_HPP

#include "async/CAsyncObject.hpp"

CAsyncObject* AsyncFileReadAllocObject();

void AsyncFileReadDestroyObject(CAsyncObject* object);

void AsyncFileReadInitialize(uint32_t threadSleep, uint32_t handlerTimeout);

void AsyncFileReadObject(CAsyncObject* object, int32_t a2);

#endif
