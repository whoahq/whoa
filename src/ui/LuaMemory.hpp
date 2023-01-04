#ifndef UI_LUA_MEMORY_HPP
#define UI_LUA_MEMORY_HPP

#include <cstdlib>

void* luaM_initPool();
void* luaM_reallocPool(void* ud, void* ptr, size_t osize, size_t nsize);

#endif
