#ifndef GX_FONT_FACE_DATA_HPP
#define GX_FONT_FACE_DATA_HPP

#include "gx/font/FreeType.hpp"
#include "gx/font/Types.hpp"
#include <common/Handle.hpp>
#include <storm/Hash.hpp>

class FACEDATA : public CHandleObject, public TSHashObject<FACEDATA, HASHKEY_STRI> {
    public:
        void* data = nullptr;
        FT_Face face = nullptr;
        HFACE selfReference;
        virtual ~FACEDATA();
};

#endif
