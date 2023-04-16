#ifndef GX_C_GX_MATRIX_STACK_HPP
#define GX_C_GX_MATRIX_STACK_HPP

#include <cstdint>
#include <tempest/Matrix.hpp>

class CGxMatrixStack {
    public:
        // Types
        enum EMatrixFlags {
            F_Identity = 0x1,
        };

        // Member variables
        uint32_t m_level = 0;
        int8_t m_dirty = 0;
        C44Matrix m_mtx[4];
        uint32_t m_flags[4] = {};

        // Member functions
        CGxMatrixStack();
        void Pop();
        void Push();
        C44Matrix& Top();
        const C44Matrix& TopConst();
};

#endif
