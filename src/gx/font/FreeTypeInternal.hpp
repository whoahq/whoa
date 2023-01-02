#ifndef GX_FONT_FREE_TYPE_INTERNAL_HPP
#define GX_FONT_FREE_TYPE_INTERNAL_HPP

#include "freetype/freetype.h"

void* FreeTypeAllocFunction(FT_Memory memory, long size);

void FreeTypeFreeFunction(FT_Memory memory, void* block);

void* FreeTypeReallocFunction(FT_Memory memory, long currentSize, long newSize, void* block);

#endif
