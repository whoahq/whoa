#ifndef UI_LUA_MEMORY_HPP
#define UI_LUA_MEMORY_HPP

#include <cstdlib>

void* luaM_initPool(void);
void* luaM_reallocPool(void*, void*, size_t, size_t);

#endif
