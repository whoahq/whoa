#ifndef GX_SHADER_C_GX_SHADER_HPP
#define GX_SHADER_C_GX_SHADER_HPP

#include "gx/Types.hpp"
#include <storm/Hash.hpp>

class SFile;

class CGxShader : public TSHashObject<CGxShader, HASHKEY_STRI> {
    public:
        // Member variables
        uint32_t refCount = 0;
        void* apiSpecific = nullptr;
        int32_t target = 0;
        int32_t int28 = 0;
        int32_t valid = 0;
        int32_t loaded = 0;
        int32_t int34 = 0;
        int32_t patched = 0;
        int32_t int3C = 0;
        int32_t int40 = 0;
        int16_t int44 = 0;
        int16_t int46 = 0;
        TSGrowableArray<unsigned char> code;

        // Member functions
        void Load(SFile*);
        int32_t Valid(void);
};

#endif
