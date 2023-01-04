#ifndef UI_C_RENDER_BATCH_HPP
#define UI_C_RENDER_BATCH_HPP

#include "gx/Types.hpp"
#include <cstdint>
#include <storm/Array.hpp>
#include <storm/List.hpp>
#include <tempest/Vector.hpp>

class CGxShader;
class CGxStringBatch;
class CGxTex;
class CSimpleFontString;
class CSimpleTexture;
class CTexture;

class RENDERCALLBACKNODE : public TSLinkedNode<RENDERCALLBACKNODE> {
    public:
        void (*callback)(void*);
        void* param;
};

class CSimpleBatchedMesh {
    public:
        // Member variables
        CTexture* texture;
        CGxTex* textureID;
        EGxBlend alphaMode;
        CGxShader* shader;
        int32_t posCount;
        C3Vector* position;
        C2Vector* texCoord;
        CImVector* color;
        int32_t colorCount;
        uint16_t* indices;
        int32_t idxCount;
        int32_t onAtlas;
        float atlasScale;
        C2Vector atlasOffset;

        // Member functions
        EGxBlend GetAlphaMode();
};

class CRenderBatch {
    public:
        // Member variables
        uint32_t m_count = 0;
        TSGrowableArray<CSimpleBatchedMesh> m_texturelist;
        CGxStringBatch* m_stringbatch = nullptr;
        TSList<RENDERCALLBACKNODE,TSGetLink<RENDERCALLBACKNODE>> m_callbacks;
        TSLink<CRenderBatch> renderLink;

        // Member functions
        void Clear();
        void Finish();
        void Queue(CTexture* texture, EGxBlend alphaMode, int32_t posCount, const C3Vector* position, const C2Vector* texCoord, int32_t colorCount, const CImVector* color, int32_t idx, const uint16_t* indices, CGxShader* shader);
        void QueueCallback(void (*callback)(void*), void* param);
        void QueueFontString(CSimpleFontString* string);
        void QueueTexture(CSimpleTexture* texture);
};

#endif
