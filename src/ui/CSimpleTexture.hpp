#ifndef UI_C_SIMPLE_TEXTURE_HPP
#define UI_C_SIMPLE_TEXTURE_HPP

#include "gx/Texture.hpp"
#include "ui/CSimpleRegion.hpp"
#include "ui/Types.hpp"
#include <tempest/Vector.hpp>

class CGxShader;
class CRect;
class CRenderBatch;
class CSimpleFrame;

class CSimpleTexture : public CSimpleRegion {
    public:
        // Static variables
        static CGxShader* s_imageModePixelShaders[];
        static uint16_t s_indices[];
        static int32_t s_metatable;
        static int32_t s_objectType;
        static EGxTexFilter s_textureFilterMode;

        // Static functions
        static void CreateScriptMetaTable(void);
        static CGxShader* GetImageModePixelShader(TextureImageMode);
        static int32_t GetObjectType(void);
        static void Init(void);
        static void RegisterScriptMethods(lua_State*);

        // Member variables
        HTEXTURE m_texture = nullptr;
        EGxBlend m_alphaMode = GxBlend_Alpha;
        CGxShader* m_shader = s_imageModePixelShaders[0];
        C3Vector m_position[4];
        C2Vector m_texCoord[4];
        uint32_t m_nonBlocking : 1;
        uint32_t m_updateTexCoord : 1;
        uint32_t m_horizTile : 1;
        uint32_t m_vertTile : 1;

        // Virtual member functions
        virtual ~CSimpleTexture();
        virtual int32_t GetScriptMetaTable(void);
        virtual bool IsA(int32_t);
        virtual void LoadXML(XMLNode*, CStatus*);
        virtual float GetWidth(void);
        virtual float GetHeight(void);
        virtual void Draw(CRenderBatch*);
        virtual void OnFrameSizeChanged(const CRect&);

        // Member functions
        CSimpleTexture(CSimpleFrame*, uint32_t, int32_t);
        void GetTexCoord(C2Vector*);
        void PostLoadXML(XMLNode*, CStatus*);
        void SetAlpha(float alpha);
        void SetBlendMode(EGxBlend);
        void SetPosition(const CRect&, C3Vector*);
        void SetTexCoord(const CRect& texRect);
        void SetTexCoord(const C2Vector*);
        int32_t SetTexture(const char*, bool, bool, EGxTexFilter, TextureImageMode);
        int32_t SetTexture(const CImVector&);
};

#endif
