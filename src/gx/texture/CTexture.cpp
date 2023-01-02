#include "gx/texture/CTexture.hpp"
#include "gx/Texture.hpp"

EGxTexFilter CTexture::s_filterMode = GxTex_LinearMipNearest;
int32_t CTexture::s_maxAnisotropy = 1;

bool HASHKEY_TEXTUREFILE::operator==(const HASHKEY_TEXTUREFILE& key) {
    if (!SStrCmpI(this->m_filename, key.m_filename, STORM_MAX_PATH) && this->m_texFlags == key.m_texFlags) {
        return true;
    }

    return false;
}

CTexture::CTexture() {
    // TODO
}

CTexture::~CTexture() {
    if (this->gxTex) {
        TextureFreeGxTex(this->gxTex);
    }

    if (this->atlas) {
        // TODO this->atlas->Free(this);
    }

    // TODO
}
