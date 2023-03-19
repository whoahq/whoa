#include "gx/buffer/CGxPool.hpp"

void CGxPool::Discard() {
    this->Invalidate();
    this->unk1C = 0;
}

void CGxPool::Invalidate() {
    for (auto buf = this->m_bufList.Head(); buf; buf = this->m_bufList.Next(buf)) {
        buf->unk1C = 0;
    }
}
