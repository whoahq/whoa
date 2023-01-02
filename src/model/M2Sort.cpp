#include "model/M2Sort.hpp"

void M2HeapSort(int32_t (*sortFunc)(uint32_t, uint32_t, const void*), uint32_t* indices, uint32_t count, const void* userArg) {
    if (count <= 1) {
        return;
    }

    auto v5 = count / 2;

    if (v5) {
        auto v6 = 2 * v5 + 1;

        do {
            auto v7 = indices[--v5];
            v6 -= 2;
            auto v15 = v5;
            auto v14 = v6;
            auto v22 = v5;
            auto v18 = v7;

            if (v6 < count) {
                do {
                    auto v20 = indices[v6];

                    if (v6 + 1 < count) {
                        auto v17 = indices[v6 + 1];

                        if (sortFunc(v17, indices[v6], userArg) > 0) {
                            v6++;
                            v20 = v17;
                        }
                    }

                    if (sortFunc(v20, v18, userArg) <= 0) {
                        break;
                    }

                    auto v8 = v22;
                    auto v9 = indices[v6];
                    v22 = v6;
                    v6 = 2 * v6 + 1;
                    indices[v8] = v9;
                } while (v6 < count);

                v5 = v15;
                v6 = v14;
            }

            indices[v22] = v18;
        } while (v5);
    }

    uint32_t j, v21, v12, v13;
    for (auto i = count - 1; i; indices[j] = v21) {
        auto v11 = 1;
        v21 = indices[i];
        indices[i] = *indices;

        for (j = 0; v11 < i; indices[v12] = v13) {
            auto v19 = indices[v11];

            if ( v11 + 1 < i ) {
                auto v16 = indices[v11 + 1];

                if (sortFunc(v16, indices[v11], userArg) > 0) {
                    v11++;
                    v19 = v16;
                }
            }

            if (sortFunc(v19, v21, userArg) <= 0) {
                break;
            }

            v12 = j;
            v13 = indices[v11];
            j = v11;
            v11 = 2 * v11 + 1;
        }

        i--;
    }
}
