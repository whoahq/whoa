#include "ui/CRenderBatch.hpp"
#include "ui/CSimpleFontString.hpp"
#include "ui/CSimpleTexture.hpp"
#include "gx/Coordinate.hpp"
#include "gx/Font.hpp"
#include "gx/Shader.hpp"
#include "gx/font/CGxStringBatch.hpp"
#include <cstdlib>
#include <storm/Error.hpp>

int32_t SortByTexture(const void* a, const void* b) {
    auto meshA = reinterpret_cast<const CSimpleBatchedMesh*>(a);
    auto meshB = reinterpret_cast<const CSimpleBatchedMesh*>(b);

    if (meshA->textureID != meshB->textureID) {
        return meshA->textureID - meshB->textureID;
    }

    if (meshA->alphaMode != meshB->alphaMode) {
        if ((meshA->alphaMode >= 2 || !meshA->color) && (meshB->alphaMode >= 2 || !meshB->color)) {
            return meshA->alphaMode - meshB->alphaMode;
        } else if (meshA->alphaMode >= 2 || !meshA->color) {
            return meshA->alphaMode - 2;
        } else if (meshB->alphaMode >= 2 || !meshB->color) {
            return 2 - meshB->alphaMode;
        }
    }

    if (meshA->shader != meshB->shader) {
        return meshA->shader - meshB->shader;
    }

    return 0;
}

EGxBlend CSimpleBatchedMesh::GetAlphaMode() {
    if (this->color && this->alphaMode < GxBlend_Alpha) {
        return GxBlend_Alpha;
    } else {
        return this->alphaMode;
    }
}

CRenderBatch::~CRenderBatch() {
    this->Clear();
}

void CRenderBatch::Clear() {
    this->m_texturelist.SetCount(0);

    if (this->m_stringbatch) {
        GxuFontDestroyBatch(this->m_stringbatch);
        this->m_stringbatch = nullptr;
    }

    this->m_callbacks.Clear();

    this->m_count = 0;
}

void CRenderBatch::Finish() {
    if (this->m_texturelist.Count() > 1) {
        qsort(this->m_texturelist.m_data, this->m_texturelist.Count(), sizeof(CSimpleBatchedMesh), SortByTexture);
    }
}

void CRenderBatch::Queue(CTexture* texture, EGxBlend alphaMode, int32_t posCount, const C3Vector* position, const C2Vector* texCoord, int32_t colorCount, const CImVector* color, int32_t idxCount, const uint16_t* indices, CGxShader* shader) {
    CGxTex* textureID = TextureGetGxTex(texture, 1, nullptr);

    if (!textureID) {
        return;
    }

    CSimpleBatchedMesh* mesh = this->m_texturelist.New();

    mesh->texture = texture;
    mesh->textureID = textureID;
    mesh->alphaMode = alphaMode;
    mesh->shader = shader;
    mesh->posCount = posCount;
    mesh->position = const_cast<C3Vector*>(position);
    mesh->texCoord = const_cast<C2Vector*>(texCoord);
    mesh->color = const_cast<CImVector*>(color);
    mesh->colorCount = colorCount;
    mesh->indices = const_cast<uint16_t*>(indices);
    mesh->idxCount = idxCount;

    // TODO
    // - implement atlas stuff
    mesh->onAtlas = 0;
    // mesh->onAtlas = TextureOnAtlas(texture);
    // if (onAtlas) {
    //     TextureGetAtlasOffsetAndScale(mesh->texture, mesh->offset, mesh->scale);
    // }

    this->m_count++;
}

void CRenderBatch::QueueCallback(void (*callback)(void*), void* param) {
    auto node = this->m_callbacks.NewNode(2, 0, 0);
    node->callback = callback;
    node->param = param;

    this->m_count++;
}

void CRenderBatch::QueueFontString(CSimpleFontString* string) {
    CRect rect = { 0.0f, 0.0f, 0.0f, 0.0f };

    if (!string->GetRect(&rect)) {
        return;
    }

    if (rect.minX > 1.0f || rect.maxX < 0.0f || rect.minY > 1.0f || rect.maxY < 0.0f) {
        return;
    }

    if (!string->m_string) {
        return;
    }

    auto gxString = TextBlockGetStringPtr(string->m_string);

    if (gxString) {
        if (!this->m_stringbatch) {
            this->m_stringbatch = GxuFontCreateBatch(false, false);
            STORM_ASSERT(this->m_stringbatch);
        }

        GxuFontAddToBatch(this->m_stringbatch, gxString);
        this->m_count++;
    }
}

void CRenderBatch::QueueTexture(CSimpleTexture* texture) {
    CGxTex* gxTex = TextureGetGxTex(texture->m_texture, texture->m_nonBlocking ? 0 : 2, 0);

    if (!gxTex) {
        // TODO
        // this->OnDelayedLoad();

        return;
    }

    if (texture->IsResizePending()) {
        texture->Resize(1);
    }

    if (texture->m_updateTexCoord) {
        C2Vector texCoord[4];
        texture->GetTexCoord(texCoord);

        CRect rect = {
            texture->m_position[1].y,
            texture->m_position[0].x,
            texture->m_position[0].y,
            texture->m_position[2].x
        };

        if (texture->m_horizTile) {
            float width = gxTex->GetWidth();
            float ddcWidth = CoordinateGetAspectCompensation() * 1024.0f * (rect.maxX - rect.minX);
            float ndcWidth = DDCToNDCWidth(ddcWidth);

            if (width && ndcWidth > 0.0f) {
                if (texCoord[0].x != 0.0f) {
                    texCoord[0].x = ndcWidth / width;
                }

                if (texCoord[1].x != 0.0f) {
                    texCoord[1].x = ndcWidth / width;
                }

                if (texCoord[2].x != 0.0f) {
                    texCoord[2].x = ndcWidth / width;
                }

                if (texCoord[3].x != 0.0f) {
                    texCoord[3].x = ndcWidth / width;
                }
            }
        }

        if (texture->m_vertTile) {
            float height = gxTex->GetHeight();
            float ddcHeight = CoordinateGetAspectCompensation() * 1024.0f * (rect.maxY - rect.minY);
            float ndcHeight = DDCToNDCWidth(ddcHeight);

            if (height && ndcHeight > 0.0f) {
                if (texCoord[0].y != 0.0f) {
                    texCoord[0].y = ndcHeight / height;
                }

                if (texCoord[1].y != 0.0f) {
                    texCoord[1].y = ndcHeight / height;
                }

                if (texCoord[2].y != 0.0f) {
                    texCoord[2].y = ndcHeight / height;
                }

                if (texCoord[3].y != 0.0f) {
                    texCoord[3].y = ndcHeight / height;
                }
            }
        }

        texture->m_updateTexCoord = 0;
        texture->SetTexCoord(texCoord);
    }

    this->Queue(
        TextureGetTexturePtr(texture->m_texture),
        texture->m_alphaMode,
        4,
        texture->m_position,
        texture->m_texCoord,
        texture->m_colorCount > 1 ? 4 : 0,
        texture->m_colorCount ? texture->m_color : nullptr,
        6,
        CSimpleTexture::s_indices,
        texture->m_shader
    );
}
