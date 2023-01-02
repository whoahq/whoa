#include "gx/CGxMatrixStack.hpp"

CGxMatrixStack::CGxMatrixStack() {
    this->m_flags[0] = 0x1;
}

void CGxMatrixStack::Pop() {
    if (this->m_level > 0) {
        this->m_level--;
    }

    this->m_dirty = 1;
}

void CGxMatrixStack::Push() {
    if (this->m_level < 3) {
        this->m_level++;
    }

    this->m_mtx[this->m_level] = this->m_mtx[this->m_level - 1];
    this->m_flags[this->m_level] = this->m_flags[this->m_level - 1];

    this->m_dirty = 1;
}

C44Matrix& CGxMatrixStack::Top() {
    this->m_dirty = 1;
    this->m_flags[this->m_level] &= 0xFFFFFFFE;
    return this->m_mtx[this->m_level];
}
