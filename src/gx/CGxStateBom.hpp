#ifndef GX_C_GX_STATE_BOM_HPP
#define GX_C_GX_STATE_BOM_HPP

#include <cstdint>

class C3Vector;
class CImVector;

class CGxStateBom {
    public:
        // Member variables
        union {
            int32_t i[3];
            uint32_t u[3];
            float f[3];
            void* p;
        } m_data;

        int32_t filler;

        // Member functions
        const CGxStateBom& operator=(float);
        const CGxStateBom& operator=(int32_t);
        const CGxStateBom& operator=(uint32_t);
        const CGxStateBom& operator=(void*);
        const CGxStateBom& operator=(C3Vector&);
        bool operator==(float);
        bool operator==(int32_t);
        bool operator==(uint32_t);
        bool operator==(void*);
        bool operator==(C3Vector&);
        bool operator!=(float);
        bool operator!=(int32_t);
        bool operator!=(uint32_t);
        bool operator!=(void*);
        bool operator!=(C3Vector&);
        bool operator!=(CGxStateBom&);
        explicit operator CImVector() const;
        explicit operator float() const;
        explicit operator int32_t() const;
        explicit operator uint32_t() const;
        explicit operator void*() const;
};

#endif
