#include "util/Autorelease.hpp"

System_Autorelease::ScopedPool::ScopedPool() {
    this->pool = [[NSAutoreleasePool alloc] init];
}

System_Autorelease::ScopedPool::~ScopedPool() {
    [this->pool release];
}
