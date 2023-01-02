#include "gx/buffer/CGxPool.hpp"

void CGxPool::Discard() {
    for (auto buf = this->m_bufList.Head(); buf; buf = this->m_bufList.Link(buf)->Next()) {
        buf->unk1C = 0;
    }

    this->unk1C = 0;
}
