#ifndef UI_C_SIMPLE_FONT_STRING_HPP
#define UI_C_SIMPLE_FONT_STRING_HPP

#include "gx/Font.hpp"
#include "ui/CSimpleFontable.hpp"
#include "ui/CSimpleRegion.hpp"

class CRenderBatch;
class CSimpleFrame;

class CSimpleFontString : public CSimpleRegion, public CSimpleFontable {
    public:
        // Static variables
        static int32_t s_count;
        static int32_t s_metatable;
        static int32_t s_objectType;

        // Static functions
        static void CreateScriptMetaTable(void);
        static int32_t GetObjectType(void);
        static void RegisterScriptMethods(lua_State*);

        // Member variables
        HTEXTFONT m_font = nullptr;
        float m_fontHeight = 0.0f;
        uint16_t m_textMaxSize = 0;
        uint16_t m_textCurSize = 0;
        char* m_text = nullptr;
        float m_spacing = 0.0f;
        HTEXTBLOCK m_string = nullptr;
        float m_cachedWidth = 0.0f;
        float m_cachedHeight = 0.0f;
        CImVector m_shadowColor;
        C2Vector m_shadowOffset;
        C2Vector m_justificationOffset;
        int16_t m_alphaGradientStart = -1;
        int16_t m_alphaGradientLength;
        uint32_t m_styleFlags = 0x2 | 0x10 | 0x200;
        uint32_t m_settableStyleFlags = 0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x1000 | 0x20000;
        uint32_t m_maxLines : 24;
        uint32_t m_unk12b : 1;

        // Virtual member functions
        virtual ~CSimpleFontString();
        virtual bool IsA(int32_t);
        virtual int32_t GetScriptMetaTable(void);
        virtual void LoadXML(XMLNode*, CStatus*);
        virtual void OnColorChanged(bool);
        virtual void OnScreenSizeChanged();
        virtual void Draw(CRenderBatch*);
        virtual float GetWidth(void);
        virtual float GetHeight(void);
        virtual void OnFrameSizeChanged(const CRect&);
        virtual void FontObjectUpdated(CSimpleFontStringAttributes&);

        // Member functions
        CSimpleFontString(CSimpleFrame*, uint32_t, int32_t);
        void AddShadow(const CImVector&, const C2Vector&);
        void ClearString(void);
        void DrawEmbeddedTextures(CRenderBatch*);
        void FreeEmbeddedTextures(void);
        const char* GetDisplayText(float, float);
        uint32_t GetFontFlags(void);
        float GetFontHeight(bool);
        const char* GetFontName(void);
        uint32_t GetNumCharsWithinWidth(const char*, uint32_t, float);
        float GetStringHeight(void);
        float GetStringWidth(void);
        const char* GetText(void);
        float GetTextWidth(const char*, uint32_t);
        void PostLoadXML(XMLNode*, CStatus*);
        void RefreshEmbeddedTextures(void);
        void RemoveShadow(void);
        int32_t SetFont(const char*, float, uint32_t, bool);
        void SetIndentedWordWrap(bool);
        void SetJustificationOffset(float, float);
        void SetJustifyV(uint8_t);
        void SetJustifyH(uint8_t);
        void SetNonSpaceWrap(int32_t);
        void SetNonWordWrap(int32_t);
        void SetSpacing(float);
        void SetStyleFlags(uint32_t);
        void SetText(const char*, int32_t);
        void SetTextLength(uint32_t);
        int32_t Sub482AC0();
        void UpdateString(void);
};

#endif
