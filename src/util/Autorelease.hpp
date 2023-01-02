#ifndef UTIL_AUTORELEASE_HPP
#define UTIL_AUTORELEASE_HPP

#include <Foundation/Foundation.h>

namespace System_Autorelease {
    class ScopedPool {
        public:
            // Member variables
            NSAutoreleasePool* pool;

            // Member functions
            ScopedPool();
            ~ScopedPool();
    };
}

#endif
