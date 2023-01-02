#ifndef MODEL_M2_SORT_HPP
#define MODEL_M2_SORT_HPP

#include <cstdint>

void M2HeapSort(int32_t (*sortFunc)(uint32_t, uint32_t, const void*), uint32_t* indices, uint32_t count, const void* userArg);

#endif
