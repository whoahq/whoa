#include "ui/LuaMemory.hpp"
#include "ui/FrameScript.hpp"
#include <cstring>
#include <new>
#include <common/MemPool.hpp>
#include <storm/Memory.hpp>

void* luaM_initPool() {
    void* v0 = SMemAlloc(sizeof(MemPool*) * 9, ".\\src\\lmemPool.cpp", 243, 0);
    MemPool** pools = (MemPool**)v0;

    uint32_t v2 = 0;

    void* v3;
    MemPool* v4;
    size_t v5;
    size_t v6;

    do {
        v3 = SMemAlloc(sizeof(MemPool), ".\\src\\lmemPool.cpp", 245, 0);

        if (v3) {
            v4 = new (v3) MemPool();
        } else {
            v4 = 0;
        }

        pools[v2] = v4;

        v5 = FrameScript::s_blocksizes[v2];
        v6 = FrameScript::s_blocksizes[v2] << 10;

        v4->Init(v5, v6);

        ++v2;
    } while (v2 < 9);

    return pools;
}

void* luaM_reallocPool(void* ud, void *ptr, size_t osize, size_t nsize) {
    void* result; // eax
    signed int v5; // esi
    signed int v6; // edx
    signed int v7; // ecx
    signed int v8; // ecx
    void *v9; // eax
    void *v10; // edi
    size_t v11; // eax
    signed int v12; // [esp+Ch] [ebp-4h]

    MemPool** mempool = (MemPool**)ud;

    result = ptr;

    v5 = -1;
    v6 = -1;
    v12 = -1;

    if (ptr) {
        v7 = 0;

        while (osize > FrameScript::s_blocksizes[v7]) {
            if (++v7 >= 9) {
                goto LABEL_7;
            }
        }

        v12 = v7;
        v5 = v7;
    }

LABEL_7:
    if (nsize) {
        v8 = 0;

        while (nsize > FrameScript::s_blocksizes[v8]) {
            if (++v8 >= 9) {
                goto LABEL_18;
            }
        }

        v6 = v8;

LABEL_18:
        if (v5 == v6) {
            if (v5 < 0) {
                result = SMemReAlloc(ptr, nsize, ".\\src\\lmemPool.cpp", 311, 0);
            }
        } else {
            if (v6 < 0) {
                v9 = SMemAlloc(nsize, ".\\src\\lmemPool.cpp", 326, 0);
            } else {
                // Pretty sure: MemPool::MemAlloc
                // v9 = sub_855820(*((_DWORD *)a1 + v6));
                v9 = mempool[v6]->MemAlloc();
            }

            v10 = v9;

            if (ptr) {
                v11 = osize;

                if (osize >= nsize) {
                    v11 = nsize;
                }

                memcpy(v10, ptr, v11);

                if (v12 >= 0) {
                    // Pretty sure: MemPool::MemFree
                    // sub_855670(*((_DWORD *)a1 + v12), ptr);
                    mempool[v12]->MemFree(ptr);
                    return v10;
                }

                SMemFree(ptr, ".\\src\\lmemPool.cpp", 338, 0);
            }

            result = v10;
        }
    } else {
        if (ptr) {
            if (v5 >= 0) {
                // Pretty sure: MemPool::MemFree
                // sub_855670(*((_DWORD *)a1 + v5), ptr);
                mempool[v5]->MemFree(ptr);
                return 0;
            }

            SMemFree(ptr, ".\\src\\lmemPool.cpp", 276, 0);
        }

        result = 0;
    }

    return result;
}
