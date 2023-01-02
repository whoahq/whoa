#ifndef GX_BUFFER_C_GX_BUF_HPP
#define GX_BUFFER_C_GX_BUF_HPP

#include <cstdint>
#include <storm/List.hpp>

class CGxPool;

class CGxBuf : public TSLinkedNode<CGxBuf> {
    public:
        // Member variables
        CGxPool* m_pool;
        uint32_t m_itemSize;
        uint32_t m_itemCount;
        uint32_t m_size;
        uint32_t m_index;
        uint8_t unk1C; // TODO
        uint8_t unk1D; // TODO
        uint8_t unk1E; // TODO
        uint8_t unk1F; // TODO

        // Member functions
        CGxBuf() = default;
        CGxBuf(CGxPool* pool, uint32_t itemSize, uint32_t itemCount, uint32_t index)
            : m_pool(pool)
            , m_itemSize(itemSize)
            , m_itemCount(itemCount)
            , m_size(itemSize * itemCount)
            , m_index(index)
            , unk1C(0)
            , unk1D(1)
            , unk1E(0)
            , unk1F(0)
            {};
};

#endif
