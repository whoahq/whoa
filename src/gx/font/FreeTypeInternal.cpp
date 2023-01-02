#include "gx/font/FreeTypeInternal.hpp"
#include <storm/Memory.hpp>

void* FreeTypeAllocFunction(FT_Memory memory, long size) {
    return SMemAlloc(size, __FILE__, __LINE__, 0);
};

void FreeTypeFreeFunction(FT_Memory memory, void* block) {
    if (block) {
        SMemFree(block, __FILE__, __LINE__, 0);
    }
};

void* FreeTypeReallocFunction(FT_Memory memory, long currentSize, long newSize, void* block) {
    return SMemReAlloc(block, newSize, __FILE__, __LINE__, 0);
};
