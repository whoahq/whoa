#ifndef GX_TEXTURE_C_TEXTURE_HPP
#define GX_TEXTURE_C_TEXTURE_HPP

#include "async/AsyncFile.hpp"
#include "gx/Types.hpp"
#include "gx/texture/CGxTex.hpp"
#include "util/CStatus.hpp"
#include <cstdint>
#include <common/Handle.hpp>
#include <storm/Hash.hpp>

class HASHKEY_TEXTUREFILE {
    public:
        // Member variables
        char* m_filename;
        CGxTexFlags m_texFlags;

        // Member functions
        bool operator==(const HASHKEY_TEXTUREFILE&);
};

class CTexture : public CHandleObject, public TSHashObject<CTexture, HASHKEY_TEXTUREFILE> {
    public:
        // Static variables
        static EGxTexFilter s_filterMode;
        static int32_t s_maxAnisotropy;

        // Member variables
        uint32_t unk1;
        uint16_t flags = 0;
        uint8_t bestMip = 0;
        uint8_t alphaBits = 0;
        CStatus loadStatus;
        CAsyncObject* asyncObject = nullptr;
        CGxTex* gxTex = nullptr;
        EGxTexTarget gxTexTarget = GxTex_2d;
        uint16_t gxWidth = 0;
        uint16_t gxHeight = 0;
        EGxTexFormat gxTexFormat = GxTex_Unknown;
        EGxTexFormat dataFormat = GxTex_Unknown;
        CGxTexFlags gxTexFlags = CGxTexFlags(GxTex_Linear, 0, 0, 0, 0, 0, 1);
        void* atlas = nullptr; // TODO CTextureAtlas
        int32_t atlasBlockIndex = 0;
        uint32_t unk2[2];
        char filename[260];

        // Member functions
        CTexture();
        ~CTexture();
};

#endif
