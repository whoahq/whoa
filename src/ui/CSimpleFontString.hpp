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
        static void CreateScriptMetaTable();
        static int32_t GetObjectType();
        static void RegisterScriptMethods(lua_State* L);

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
        virtual bool IsA(int32_t type);
        virtual int32_t GetScriptMetaTable();
        virtual void LoadXML(XMLNode* node, CStatus* status);
        virtual void OnColorChanged(bool a2);
        virtual void OnScreenSizeChanged();
        virtual void Draw(CRenderBatch* batch);
        virtual void SetWidth(float width);
        virtual void SetHeight(float height);
        virtual float GetWidth();
        virtual float GetHeight();
        virtual void OnFrameSizeChanged(const CRect& rect);
        virtual void FontObjectUpdated(CSimpleFontStringAttributes& attributes);

        // Member functions
        CSimpleFontString(CSimpleFrame* frame, uint32_t drawlayer, int32_t show);
        void AddShadow(const CImVector& shadowColor, const C2Vector& shadowOffset);
        void ClearString();
        void DrawEmbeddedTextures(CRenderBatch* batch);
        void FreeEmbeddedTextures();
        const char* GetDisplayText(float width, float height);
        uint32_t GetFontFlags();
        float GetFontHeight(bool a2);
        const char* GetFontName();
        uint32_t GetNumCharsWithinWidth(const char* text, uint32_t textBytes, float maxWidth);
        float GetStringHeight();
        float GetStringWidth();
        const char* GetText();
        float GetTextWidth(const char* text, uint32_t textBytes);
        void PostLoadXML(XMLNode* node, CStatus* status);
        void RefreshEmbeddedTextures();
        void RemoveShadow();
        int32_t SetFont(const char* fontName, float fontHeight, uint32_t fontFlags, bool force);
        void SetIndentedWordWrap(bool a2);
        void SetJustificationOffset(float x, float y);
        void SetJustifyV(uint8_t justify);
        void SetJustifyH(uint8_t justify);
        void SetNonSpaceWrap(int32_t a2);
        void SetNonWordWrap(int32_t a2);
        void SetSpacing(float spacing);
        void SetStyleFlags(uint32_t styleFlags);
        void SetText(const char* text, int32_t a3);
        void SetTextLength(uint32_t a2);
        void UpdateString();
};

#endif
