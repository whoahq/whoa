#ifndef GX_C_GX_FONT_HPP
#define GX_C_GX_FONT_HPP

#include "gx/font/CGxString.hpp"
#include "gx/Font.hpp"
#include "gx/Texture.hpp"
#include <cstdint>
#include <storm/Hash.hpp>
#include <storm/List.hpp>

class CTexture;

class GLYPHBITMAPDATA {
    public:
        // Member variables
        void* m_data = nullptr;
        uint32_t m_dataSize = 0;
        uint32_t m_glyphWidth;
        uint32_t m_glyphHeight;
        uint32_t m_glyphCellWidth;
        float m_glyphAdvance;
        float m_glyphBearing;
        uint32_t m_glyphPitch;
        int32_t m_yOffset;
        int32_t m_yStart;
        CRect m_textureCoords;

        // Member functions
        ~GLYPHBITMAPDATA();
        void CopyFrom(GLYPHBITMAPDATA*);
};

class CHARCODEDESC : public TSHashObject<CHARCODEDESC, HASHKEY_NONE> {
    public:
        // Member variables
        TSLink<CHARCODEDESC> textureRowLink;
        TSLink<CHARCODEDESC> fontGlyphLink;
        uint32_t textureNumber = -1;
        uint32_t rowNumber = -1;
        uint32_t glyphStartPixel = -1;
        uint32_t glyphEndPixel = 0;
        GLYPHBITMAPDATA bitmapData;

        // Member functions
        uint32_t GapToNextTexture(void);
        uint32_t GapToPreviousTexture(void);
        void GenerateTextureCoords(uint32_t, uint32_t);
};

class KERNINGHASHKEY {
    public:
        // Member variables
        uint32_t code;

        // Member functions
        KERNINGHASHKEY& operator=(const KERNINGHASHKEY&);
        bool operator==(const KERNINGHASHKEY&);
};

class KERNNODE : public TSHashObject<KERNNODE, KERNINGHASHKEY> {
    public:
        // Member variables
        uint32_t flags = 0x0;
        float proporportionalSpacing = 0.0f;
        float fixedWidthSpacing = 0.0f;
};

class TEXTURECACHEROW {
    public:
        // Member variables
        uint32_t widestFreeSlot = 0;
        STORM_EXPLICIT_LIST(CHARCODEDESC, textureRowLink) glyphList;

        // Member functions
        CHARCODEDESC* CreateNewDesc(GLYPHBITMAPDATA*, uint32_t, uint32_t);
        void EvictGlyph(CHARCODEDESC*);
};

class TEXTURECACHE {
    public:
        // Static variables
        static uint16_t s_textureData[256 * 256];

        // Static functions
        static void TextureCallback(EGxTexCommand, uint32_t, uint32_t, uint32_t, uint32_t, void*, uint32_t&, const void*&);

        // Member variables
        HTEXTURE m_texture = nullptr;
        CGxFont* m_theFace = nullptr;
        int8_t m_anyDirtyGlyphs = 0;
        int8_t pad[3];
        TSFixedArray<TEXTURECACHEROW> m_textureRows;

        // Member functions
        CHARCODEDESC* AllocateNewGlyph(GLYPHBITMAPDATA*);
        void CreateTexture(int32_t);
        void Initialize(CGxFont*, uint32_t);
        void PasteGlyph(const GLYPHBITMAPDATA&, uint16_t*);
        void PasteGlyphNonOutlinedAA(const GLYPHBITMAPDATA&, uint16_t*);
        void PasteGlyphNonOutlinedMonochrome(const GLYPHBITMAPDATA&, uint16_t*);
        void PasteGlyphOutlinedAA(const GLYPHBITMAPDATA&, uint16_t*);
        void PasteGlyphOutlinedMonochrome(const GLYPHBITMAPDATA&, uint16_t*);
        void UpdateDirty(void);
        void WriteGlyphToTexture(CHARCODEDESC*);
};

class CGxFont : public TSLinkedNode<CGxFont> {
    public:
        // Member variables
        STORM_EXPLICIT_LIST(CGxString, m_fontStringLink) m_strings;
        TSHashTable<CHARCODEDESC, HASHKEY_NONE> m_activeCharacters;
        TSHashTable<KERNNODE, KERNINGHASHKEY> m_kernInfo;
        STORM_EXPLICIT_LIST(CHARCODEDESC, fontGlyphLink) m_activeCharacterCache;
        HFACE m_faceHandle;
        char m_fontName[260];
        uint32_t m_cellHeight = 0;
        uint32_t m_baseline;
        uint32_t m_flags;
        float m_requestedFontHeight;
        float m_pixelsPerUnit = 0.0f;
        TEXTURECACHE m_textureCache[8];
        uint32_t m_pixelSize;

        // Member functions
        ~CGxFont();
        int32_t CheckStringGlyphs(const char*);
        void Clear(void);
        void ClearGlyphs(void);
        float ComputeStep(uint32_t, uint32_t);
        float ComputeStepFixedWidth(uint32_t, uint32_t);
        float GetGlyphBearing(const CHARCODEDESC*, bool, float);
        int32_t GetGlyphData(GLYPHBITMAPDATA*, uint32_t);
        const char* GetName(void) const;
        uint32_t GetPixelSize(void);
        int32_t Initialize(const char*, uint32_t, float);
        const CHARCODEDESC* NewCodeDesc(uint32_t);
        void RegisterEvictNotice(uint32_t);
        int32_t UpdateDimensions(void);
};

#endif
