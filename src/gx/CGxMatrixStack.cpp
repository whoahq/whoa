#include "gx/CGxMatrixStack.hpp"

CGxMatrixStack::CGxMatrixStack() {
    this->m_flags[0] = F_Identity;
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
    this->m_flags[this->m_level] &= ~F_Identity;
    return this->m_mtx[this->m_level];
}

const C44Matrix& CGxMatrixStack::TopConst() {
    return this->m_mtx[this->m_level];
}
