#ifndef COMPONENT_C_CHARACTER_COMPONENT_HPP
#define COMPONENT_C_CHARACTER_COMPONENT_HPP

#include "component/Types.hpp"
#include "gx/Texture.hpp"
#include <tempest/Vector.hpp>

class CACHEENTRY;
class CCharacterComponent;
class CharSectionsRec;

struct BlpPalPixel;
struct MipBits;
struct st_race;
struct TCTEXTUREINFO;

typedef void (PREP_FUNC)(CCharacterComponent* component);

struct CompSectionInfo {
    C2iVector pos;
    C2iVector size;
};

struct ComponentItemDisplay {
    void* texture[7] = {};
    uint32_t displayID[7] = {};
    uint32_t priorityDirty = 0;
};

class CCharacterComponent {
    public:
        // Static variables
        static st_race* s_chrVarArray;
        static uint32_t s_chrVarArrayLength;
        static EGxTexFormat s_gxFormat;
        static uint32_t s_mipLevels;
        static PREP_FUNC* s_prepFunc[NUM_COMPONENT_SECTIONS];
        static CompSectionInfo s_sectionInfo[NUM_COMPONENT_SECTIONS];
        static CompSectionInfo s_sectionInfoRaw[NUM_COMPONENT_SECTIONS];
        static MipBits* s_textureBuffer;
        static MipBits* s_textureBufferCompressed;
        static uint32_t s_textureSize;

        // Static functions
        static CCharacterComponent* AllocComponent();
        static void Initialize();
        static void Initialize(EGxTexFormat textureFormat, uint32_t textureLevel, int32_t thread, int32_t compress);
        static void InitDbData();
        static void RenderPrepAL(CCharacterComponent* component);
        static void RenderPrepAU(CCharacterComponent* component);
        static void RenderPrepFO(CCharacterComponent* component);
        static void RenderPrepHA(CCharacterComponent* component);
        static void RenderPrepHL(CCharacterComponent* component);
        static void RenderPrepHU(CCharacterComponent* component);
        static void RenderPrepLL(CCharacterComponent* component);
        static void RenderPrepLU(CCharacterComponent* component);
        static void RenderPrepTL(CCharacterComponent* component);
        static void RenderPrepTU(CCharacterComponent* component);
        static void Paste(void* srcTexture, MipBits* dstMips, const C2iVector& a3, const C2iVector& a4, const C2iVector& a5, TCTEXTUREINFO& srcInfo, int32_t srcMipLevel);
        static void PasteFromSkin(COMPONENT_SECTIONS section, void* srcTexture, MipBits* dstMips);
        static void PasteOpaque(void* srcTexture, const BlpPalPixel* srcPal, MipBits* dstMips, const C2iVector& dstPos, uint32_t dstWidth, const C2iVector& srcPos, const C2iVector& srcSize, TCTEXTUREINFO& srcInfo, int32_t srcMipLevel, int32_t dstMipLevelOfs);
        static void PasteScale(void* srcTexture, MipBits* dstMips, const C2iVector& a3, const C2iVector& a4, const C2iVector& a5, TCTEXTUREINFO& srcInfo);
        static void PasteToSection(COMPONENT_SECTIONS section, void* srcTexture, MipBits* dstMips);
        static void PasteTransparent1Bit(void* srcTexture, const BlpPalPixel* srcPal, MipBits* dstMips, const C2iVector& dstPos, uint32_t dstWidth, const C2iVector& srcPos, const C2iVector& srcSize, TCTEXTUREINFO& srcInfo, int32_t srcMipLevel, int32_t dstMipLevelOfs);
        static void PasteTransparent4Bit(void* srcTexture, const BlpPalPixel* srcPal, MipBits* dstMips, const C2iVector& dstPos, uint32_t dstWidth, const C2iVector& srcPos, const C2iVector& srcSize, TCTEXTUREINFO& srcInfo, int32_t srcMipLevel, int32_t dstMipLevelOfs);
        static void PasteTransparent8Bit(void* srcTexture, const BlpPalPixel* srcPal, MipBits* dstMips, const C2iVector& dstPos, uint32_t dstWidth, const C2iVector& srcPos, const C2iVector& srcSize, TCTEXTUREINFO& srcInfo, int32_t srcMipLevel, int32_t dstMipLevelOfs);
        static void UpdateBaseTexture(EGxTexCommand cmd, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevel, void* userArg, uint32_t& texelStrideInBytes, const void*& texels);

        // Member variables
        uint32_t m_flags = 0x1 | 0x2 | 0x4;
        uint32_t m_sectionDirty = 0xFFFFFFFF;
        EGxTexFormat m_textureFormat = CCharacterComponent::s_gxFormat;
        ComponentData m_data;
        HTEXTURE m_baseTexture = nullptr;
        void* m_texture[NUM_COMPONENT_VARIATIONS * 3] = {};
        ComponentItemDisplay m_itemDisplays[NUM_COMPONENT_SECTIONS] = {};

        // Member functions
        void CreateBaseTexture();
        void GeosRenderPrep();
        CharSectionsRec* GetSectionsRecord(COMPONENT_VARIATIONS sectionIndex, int32_t variationIndex, int32_t colorIndex, bool* found);
        void Init(ComponentData* data, const char* a3);
        int32_t ItemsLoaded(int32_t a2);
        void LoadBaseVariation(COMPONENT_VARIATIONS sectionIndex, int32_t textureIndex, int32_t variationIndex, int32_t colorIndex, COMPONENT_SECTIONS section, const char* a7);
        void PrepSections();
        int32_t RenderPrep(int32_t a2);
        void RenderPrepAll();
        void RenderPrepSections();
        void ReplaceExtraSkinTexture(const char* a2);
        void ReplaceHairTexture(int32_t hairStyleID, const char* a3);
        void SetFace(int32_t faceID, bool a3, const char* a4);
        void SetHairColor(int32_t hairColorID, bool a3, const char* a4);
        void SetHairStyle(int32_t hairStyleID, const char* a3);
        void SetSkinColor(int32_t skinColorID, bool a3, bool a4, const char* a5);
        int32_t VariationsLoaded(int32_t a2);
};

#endif
