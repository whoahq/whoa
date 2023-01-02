#ifndef UTIL_UNIMPLEMENTED_HPP
#define UTIL_UNIMPLEMENTED_HPP

#include <cstdio>

#define WHOA_UNIMPLEMENTED()                                                                                    \
    fprintf(stderr, "Function not yet implemented: %s in %s (line %i)\n", __FUNCTION__, __FILE__, __LINE__);    \
    return 0;                                                                                                   \
    void(0)

#define WHOA_UNIMPLEMENTED_VOID()                                                                               \
    fprintf(stderr, "Function not yet implemented: %s in %s (line %i)\n", __FUNCTION__, __FILE__, __LINE__);    \
    return;                                                                                                     \
    void(0)

#endif
