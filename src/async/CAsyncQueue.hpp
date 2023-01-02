#ifndef ASYNC_C_ASYNC_QUEUE_HPP
#define ASYNC_C_ASYNC_QUEUE_HPP

#include "async/CAsyncObject.hpp"
#include <cstdint>
#include <storm/List.hpp>

class CAsyncQueue : public TSLinkedNode<CAsyncQueue> {
    public:
        // Member variables
        STORM_EXPLICIT_LIST(CAsyncObject, link) readList;
        STORM_EXPLICIT_LIST(CAsyncObject, link) list14;
        int32_t int20;
};

#endif
