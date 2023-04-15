#ifndef GX_C_GX_MATRIX_STACK_HPP
#define GX_C_GX_MATRIX_STACK_HPP

#include <cstdint>
#include <tempest/Matrix.hpp>

class CGxMatrixStack {
    public:
        // Member variables
        uint32_t m_level = 0;
        int8_t m_dirty = 0;
        C44Matrix m_mtx[4];
        uint32_t m_flags[4] = {};

        // Member functions
        CGxMatrixStack();
        void Pop(void);
        void Push(void);
        C44Matrix& Top(void);
        const C44Matrix& TopConst();
};

#endif
