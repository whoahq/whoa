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
        EGxBlend GetAlphaMode(void);
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
        void Clear(void);
        void Finish(void);
        void Queue(CTexture*, EGxBlend, int32_t, const C3Vector*, const C2Vector*, int32_t, const CImVector*, int32_t, const uint16_t*, CGxShader*);
        void QueueCallback(void (*)(void*), void*);
        void QueueFontString(CSimpleFontString*);
        void QueueTexture(CSimpleTexture*);
};

#endif
