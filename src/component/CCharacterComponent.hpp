#ifndef COMPONENT_C_CHARACTER_COMPONENT_HPP
#define COMPONENT_C_CHARACTER_COMPONENT_HPP

#include "component/Types.hpp"
#include "gx/Texture.hpp"
#include "object/Types.hpp"
#include <tempest/Vector.hpp>

class CACHEENTRY;
class CCharacterComponent;
class CharSectionsRec;
class ItemDisplayInfoRec;

struct BlpPalPixel;
struct MipBits;
struct st_race;
struct TCTEXTUREINFO;

typedef void (CCharacterComponent::*ITEM_FUNC)(ITEM_SLOT itemSlot, const ItemDisplayInfoRec* displayRec, bool update);
typedef void (CCharacterComponent::*PREP_FUNC)();

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
        static uint32_t* s_characterFacialHairStylesList;
        static st_race* s_chrVarArray;
        static uint32_t s_chrVarArrayLength;
        static EGxTexFormat s_gxFormat;
        static ITEM_FUNC s_itemFunc[NUM_COMPONENT_SECTIONS];
        static uint32_t s_mipLevels;
        static PREP_FUNC s_prepFunc[NUM_COMPONENT_SECTIONS];
        static CompSectionInfo s_sectionInfo[NUM_COMPONENT_SECTIONS];
        static CompSectionInfo s_sectionInfoRaw[NUM_COMPONENT_SECTIONS];
        static MipBits* s_textureBuffer;
        static MipBits* s_textureBufferCompressed;
        static uint32_t s_textureSize;

        // Static functions
        static void AddHandItem(CM2Model* model, const ItemDisplayInfoRec* displayRec, INVENTORY_SLOTS invSlot, SHEATHE_TYPE sheatheType, bool a5, bool a6, bool a7, int32_t a8);
        static CCharacterComponent* AllocComponent();
        static HTEXTURE CreateTexture(const char* fileName, CStatus* status);
        static void Initialize();
        static void Initialize(EGxTexFormat textureFormat, uint32_t textureLevel, int32_t thread, int32_t compress);
        static void InitDbData();
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
        uint32_t m_memHandle;
        EGxTexFormat m_textureFormat = CCharacterComponent::s_gxFormat;
        ComponentData m_data;
        HTEXTURE m_baseTexture = nullptr;
        void* m_texture[NUM_COMPONENT_VARIATIONS * 3] = {};
        ComponentItemDisplay m_itemDisplays[NUM_COMPONENT_SECTIONS] = {};
        int32_t m_items[NUM_ITEM_SLOT] = {};

        // Member functions
        void AddItem(ITEM_SLOT itemSlot, int32_t displayID, int32_t a4);
        void AddItem(ITEM_SLOT itemSlot, const ItemDisplayInfoRec* displayRec, int32_t a4);
        void AddItemBySlot(INVENTORY_SLOTS invSlot, int32_t displayID, int32_t a4);
        void ClearItemDisplay(COMPONENT_SECTIONS section, int32_t priority);
        void CreateBaseTexture();
        void* CreateTexture(const ItemDisplayInfoRec* displayRec, int32_t section);
        void GeosRenderPrep();
        CharSectionsRec* GetSectionsRecord(COMPONENT_VARIATIONS sectionIndex, int32_t variationIndex, int32_t colorIndex, bool* found);
        int32_t Init(ComponentData* data, const char* a3);
        int32_t ItemsLoaded(int32_t a2);
        void LoadBaseVariation(COMPONENT_VARIATIONS sectionIndex, int32_t textureIndex, int32_t variationIndex, int32_t colorIndex, COMPONENT_SECTIONS section, const char* a7);
        void PrepSections();
        int32_t RenderPrep(int32_t a2);
        void RenderPrepAL();
        void RenderPrepAU();
        void RenderPrepFO();
        void RenderPrepHA();
        void RenderPrepHL();
        void RenderPrepHU();
        void RenderPrepLL();
        void RenderPrepLU();
        void RenderPrepTL();
        void RenderPrepTU();
        void RenderPrepAll();
        void RenderPrepSections();
        void ReplaceExtraSkinTexture(const char* a2);
        void ReplaceHairTexture(int32_t hairStyleID, const char* a3);
        void SetBeardStyle(int32_t facialHairStyleID, bool a3, const char* a4);
        void SetFace(int32_t faceID, bool a3, const char* a4);
        void SetHairColor(int32_t hairColorID, bool a3, const char* a4);
        void SetHairStyle(int32_t hairStyleID, const char* a3);
        void SetSkinColor(int32_t skinColorID, bool a3, bool a4, const char* a5);
        void UpdateItemAL(ITEM_SLOT itemSlot, const ItemDisplayInfoRec* displayRec, bool update);
        void UpdateItemAU(ITEM_SLOT itemSlot, const ItemDisplayInfoRec* displayRec, bool update);
        void UpdateItemFO(ITEM_SLOT itemSlot, const ItemDisplayInfoRec* displayRec, bool update);
        void UpdateItemHA(ITEM_SLOT itemSlot, const ItemDisplayInfoRec* displayRec, bool update);
        void UpdateItemHL(ITEM_SLOT itemSlot, const ItemDisplayInfoRec* displayRec, bool update);
        void UpdateItemHU(ITEM_SLOT itemSlot, const ItemDisplayInfoRec* displayRec, bool update);
        void UpdateItemLL(ITEM_SLOT itemSlot, const ItemDisplayInfoRec* displayRec, bool update);
        void UpdateItemLU(ITEM_SLOT itemSlot, const ItemDisplayInfoRec* displayRec, bool update);
        void UpdateItemTL(ITEM_SLOT itemSlot, const ItemDisplayInfoRec* displayRec, bool update);
        void UpdateItemTU(ITEM_SLOT itemSlot, const ItemDisplayInfoRec* displayRec, bool update);
        int32_t UpdateItemDisplay(COMPONENT_SECTIONS section, const ItemDisplayInfoRec* newDisplayRec, int32_t priority);
        int32_t VariationsLoaded(int32_t a2);
};

#endif
