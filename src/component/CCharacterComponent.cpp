#include "component/CCharacterComponent.hpp"
#include "component/Texture.hpp"
#include "component/Util.hpp"
#include "db/Db.hpp"
#include "gx/Blp.hpp"
#include "gx/Device.hpp"
#include "gx/Texture.hpp"
#include "model/CM2Model.hpp"
#include "object/Types.hpp"
#include "util/CStatus.hpp"
#include <storm/Memory.hpp>
#include <storm/String.hpp>

uint32_t* CCharacterComponent::s_characterFacialHairStylesList;
st_race* CCharacterComponent::s_chrVarArray;
uint32_t CCharacterComponent::s_chrVarArrayLength;
EGxTexFormat CCharacterComponent::s_gxFormat;
uint32_t CCharacterComponent::s_mipLevels;
PREP_FUNC* CCharacterComponent::s_prepFunc[];
CompSectionInfo CCharacterComponent::s_sectionInfo[];
MipBits* CCharacterComponent::s_textureBuffer;
MipBits* CCharacterComponent::s_textureBufferCompressed;
uint32_t CCharacterComponent::s_textureSize;

CompSectionInfo CCharacterComponent::s_sectionInfoRaw[] = {
    { 0,    0,      256,    128 },  // SECTION_ARM_UPPER
    { 0,    128,    256,    128 },  // SECTION_ARM_LOWER
    { 0,    256,    256,    64  },  // SECTION_HAND
    { 256,  0,      256,    128 },  // SECTION_TORSO_UPPER
    { 256,  128,    256,    64  },  // SECTION_TORSO_LOWER
    { 256,  192,    256,    128 },  // SECTION_LEG_UPPER
    { 256,  320,    256,    128 },  // SECTION_LEG_LOWER
    { 256,  448,    256,    64  },  // SECTION_FOOT
    { 0,    320,    256,    64  },  // SECTION_HEAD_UPPER
    { 0,    384,    256,    128 },  // SECTION_HEAD_LOWER
};

/**
 * Texture priorities for each item slot and component section. Determines order for pasting
 * textures in RenderPrep functions. Priority start and end for leg component sections is
 * adjusted in the corresponding RenderPrep functions.
 */
int32_t s_itemPriority[NUM_ITEM_SLOT][NUM_COMPONENT_SECTIONS] = {
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, // ITEMSLOT_0
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, // ITEMSLOT_1
    {  0,  0, -1,  0,  0, -1, -1, -1, -1, -1 }, // ITEMSLOT_2
    {  1,  1, -1,  1,  1,  1,  1, -1, -1, -1 }, // ITEMSLOT_3
    { -1, -1, -1, -1,  5,  2, -1, -1, -1, -1 }, // ITEMSLOT_4
    { -1, -1, -1, -1, -1,  0,  0, -1, -1, -1 }, // ITEMSLOT_5
    { -1, -1, -1, -1, -1, -1,  2,  0, -1, -1 }, // ITEMSLOT_6
    { -1,  2, -1, -1, -1, -1, -1, -1, -1, -1 }, // ITEMSLOT_7
    { -1,  3,  0, -1, -1, -1, -1, -1, -1, -1 }, // ITEMSLOT_8
    { -1, -1, -1,  4,  4, -1, -1, -1, -1, -1 }, // ITEMSLOT_9
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // ITEMSLOT_10
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // ITEMSLOT_11
};

/**
 * Total item texture priorities for each component section.
 */
#define SECTION_AU_ITEM_PRIORITIES 2
#define SECTION_AL_ITEM_PRIORITIES 7
#define SECTION_HA_ITEM_PRIORITIES 1
#define SECTION_TU_ITEM_PRIORITIES 5
#define SECTION_TL_ITEM_PRIORITIES 7
#define SECTION_LU_ITEM_PRIORITIES 3
#define SECTION_LL_ITEM_PRIORITIES 6
#define SECTION_FO_ITEM_PRIORITIES 1
#define SECTION_HU_ITEM_PRIORITIES 0
#define SECTION_HL_ITEM_PRIORITIES 0

int32_t s_bInRenderPrep = 0;
char* s_pathEnd;
char s_path[STORM_MAX_PATH];
CStatus s_status;

#define TEXTURE_INDEX(section, texture) (3 * section + texture)

CCharacterComponent* CCharacterComponent::AllocComponent() {
    // TODO ObjectAlloc
    return STORM_NEW(CCharacterComponent);
}

HTEXTURE CCharacterComponent::CreateTexture(const char* fileName, CStatus* status) {
    auto texFlags = CGxTexFlags(GxTex_LinearMipNearest, 0, 0, 0, 0, 0, 1);
    return TextureCreate(fileName, texFlags, status, 0);
}

void CCharacterComponent::Initialize() {
    // TODO

    // TODO proper implementation
    CCharacterComponent::Initialize(GxTex_Rgb565, 9, 0, 0);
}

void CCharacterComponent::Initialize(EGxTexFormat textureFormat, uint32_t textureLevel, int32_t thread, int32_t compress) {
    // TODO

    s_pathEnd = s_path;

    CCharacterComponent::s_prepFunc[SECTION_ARM_UPPER]      = &CCharacterComponent::RenderPrepAU;
    CCharacterComponent::s_prepFunc[SECTION_ARM_LOWER]      = &CCharacterComponent::RenderPrepAL;
    CCharacterComponent::s_prepFunc[SECTION_HAND]           = &CCharacterComponent::RenderPrepHA;
    CCharacterComponent::s_prepFunc[SECTION_TORSO_UPPER]    = &CCharacterComponent::RenderPrepTU;
    CCharacterComponent::s_prepFunc[SECTION_TORSO_LOWER]    = &CCharacterComponent::RenderPrepTL;
    CCharacterComponent::s_prepFunc[SECTION_LEG_UPPER]      = &CCharacterComponent::RenderPrepLU;
    CCharacterComponent::s_prepFunc[SECTION_LEG_LOWER]      = &CCharacterComponent::RenderPrepLL;
    CCharacterComponent::s_prepFunc[SECTION_FOOT]           = &CCharacterComponent::RenderPrepFO;
    CCharacterComponent::s_prepFunc[SECTION_HEAD_UPPER]     = &CCharacterComponent::RenderPrepHU;
    CCharacterComponent::s_prepFunc[SECTION_HEAD_LOWER]     = &CCharacterComponent::RenderPrepHL;

    // TODO

    // Clamp mip levels between 6 and 9
    uint32_t mipLevels = std::min(std::max(textureLevel, 6u), 9u);

    // Cap mip levels to 8 if compression isn't enabled
    if (!compress && mipLevels > 8) {
        mipLevels = 8;
    }

    CCharacterComponent::s_mipLevels = mipLevels;
    CCharacterComponent::s_textureSize = 1 << mipLevels;

    // Scale section info to match mip levels
    for (int32_t i = 0; i < NUM_COMPONENT_SECTIONS; i++) {
        auto& info = CCharacterComponent::s_sectionInfo[i];
        auto& infoRaw = CCharacterComponent::s_sectionInfoRaw[i];

        info.pos.x = infoRaw.pos.x >> (9 - mipLevels);
        info.pos.y = infoRaw.pos.y >> (9 - mipLevels);
        info.size.x = infoRaw.size.x >> (9 - mipLevels);
        info.size.y = infoRaw.size.y >> (9 - mipLevels);
    }

    // TODO

    CCharacterComponent::s_gxFormat = textureFormat;

    // TODO

    CCharacterComponent::InitDbData();

    // TODO

    CCharacterComponent::s_textureBuffer = TextureAllocMippedImg(
        PIXEL_ARGB8888,
        CCharacterComponent::s_textureSize,
        CCharacterComponent::s_textureSize
    );

    // TODO
}

void CCharacterComponent::InitDbData() {
    uint32_t varArrayLength = (g_chrRacesDB.m_maxID + 1) * UNITSEX_NUM_SEXES;
    CCharacterComponent::s_chrVarArrayLength = varArrayLength;

    BuildComponentArray(varArrayLength, &CCharacterComponent::s_chrVarArray);
    CountFacialFeatures(varArrayLength, &CCharacterComponent::s_characterFacialHairStylesList);
}

void CCharacterComponent::Paste(void* srcTexture, MipBits* dstMips, const C2iVector& dstPos, const C2iVector& srcPos, const C2iVector& srcSize, TCTEXTUREINFO& srcInfo, int32_t srcMipLevel) {
    uint32_t dstWidth = CCharacterComponent::s_textureSize * 4;

    auto srcPal = TextureCacheGetPal(srcTexture);

    if (!srcPal) {
        // TODO fill in with crappy green (0x00FF00FF)

        return;
    }

    switch (srcInfo.alphaSize) {
    case 0:
        CCharacterComponent::PasteOpaque(
            srcTexture,
            srcPal,
            dstMips,
            dstPos,
            dstWidth,
            srcPos,
            srcSize,
            srcInfo,
            srcMipLevel,
            -srcMipLevel
        );

        break;

    case 1:
        CCharacterComponent::PasteTransparent1Bit(
            srcTexture,
            srcPal,
            dstMips,
            dstPos,
            dstWidth,
            srcPos,
            srcSize,
            srcInfo,
            srcMipLevel,
            -srcMipLevel
        );

        break;

    case 4:
        CCharacterComponent::PasteTransparent4Bit(
            srcTexture,
            srcPal,
            dstMips,
            dstPos,
            dstWidth,
            srcPos,
            srcSize,
            srcInfo,
            srcMipLevel,
            -srcMipLevel
        );

        break;

    case 8:
        CCharacterComponent::PasteTransparent8Bit(
            srcTexture,
            srcPal,
            dstMips,
            dstPos,
            dstWidth,
            srcPos,
            srcSize,
            srcInfo,
            srcMipLevel,
            -srcMipLevel
        );

        break;

    default:
        // Do nothing
        break;
    }
}

void CCharacterComponent::PasteFromSkin(COMPONENT_SECTIONS section, void* srcTexture, MipBits* dstMips) {
    if (!TextureCacheHasMips(srcTexture)) {
        return;
    }

    auto& sectionInfo = CCharacterComponent::s_sectionInfo[section];

    TCTEXTUREINFO srcInfo;
    TextureCacheGetInfo(srcTexture, srcInfo, 1);

    // Skin is always opaque
    srcInfo.alphaSize = 0;

    if (srcInfo.width >= CCharacterComponent::s_textureSize || srcInfo.height >= CCharacterComponent::s_textureSize ) {
        // Calculate source mip level appropriate for CCharacterComponent::s_textureSize
        int32_t srcMipLevel = 0;
        int32_t srcWidth = srcInfo.width;
        while (srcWidth > CCharacterComponent::s_textureSize) {
            srcWidth /= 2;
            srcMipLevel++;
        }

        CCharacterComponent::Paste(srcTexture, dstMips, sectionInfo.pos, sectionInfo.pos, sectionInfo.size, srcInfo, srcMipLevel);
    } else {
        CCharacterComponent::PasteScale(srcTexture, dstMips, sectionInfo.pos, sectionInfo.pos, sectionInfo.size, srcInfo);
    }
}

void CCharacterComponent::PasteOpaque(void* srcTexture, const BlpPalPixel* srcPal, MipBits* dstMips, const C2iVector& dstPos, uint32_t dstWidth, const C2iVector& srcPos, const C2iVector& srcSize, TCTEXTUREINFO& srcInfo, int32_t srcMipLevel, int32_t dstMipLevelOfs) {
    // Prepare first mip level

    C2iVector curSrcSize = srcSize;

    C2iVector curSrcPos = srcPos;
    C2iVector curDstPos = dstPos;

    uint32_t curSrcWidth = srcInfo.width >> srcMipLevel;
    uint32_t curDstWidth = dstWidth;

    // Paste texture for each mip level

    for (int32_t curMipLevel = srcMipLevel; curMipLevel < srcInfo.mipCount; curMipLevel++) {
        auto srcMip = TextureCacheGetMip(srcTexture, curMipLevel);
        auto srcRow = &srcMip[(curSrcPos.y * curSrcWidth) + curSrcPos.x];

        auto dstMip = reinterpret_cast<uint8_t*>(dstMips[curMipLevel + dstMipLevelOfs].mip[0]);
        auto dstRow = &dstMip[(curDstPos.y * curDstWidth) + (curDstPos.x * 4)];

        // Calculate the end of the source region
        auto srcEnd = srcRow + curSrcWidth * curSrcSize.y;

        // Copy each row
        while (srcRow < srcEnd) {
            auto dst = reinterpret_cast<C4Pixel*>(dstRow);

            for (uint32_t x = 0; x < curSrcSize.x; x++) {
                // Get palette entry
                auto& src = srcPal[srcRow[x]];

                // Copy color from palette entry
                dst->b = src.b;
                dst->g = src.g;
                dst->r = src.r;
                dst->a = 0xFF;

                dst++;
            }

            // Move to next row
            srcRow += curSrcWidth;
            dstRow += curDstWidth;
        }

        // Prepare next mip level

        curSrcSize.x = std::max(1, curSrcSize.x / 2);
        curSrcSize.y = std::max(1, curSrcSize.y / 2);

        curSrcPos.x /= 2;
        curSrcPos.y /= 2;
        curDstPos.x /= 2;
        curDstPos.y /= 2;

        curDstWidth /= 2;
        curSrcWidth /= 2;
    }
}

void CCharacterComponent::PasteScale(void* srcTexture, MipBits* dstMips, const C2iVector& a3, const C2iVector& a4, const C2iVector& a5, TCTEXTUREINFO& srcInfo) {
    // TODO
}

void CCharacterComponent::PasteToSection(COMPONENT_SECTIONS section, void* srcTexture, MipBits* dstMips) {
    if (!TextureCacheHasMips(srcTexture)) {
        return;
    }

    auto& sectionInfo = CCharacterComponent::s_sectionInfo[section];

    TCTEXTUREINFO srcInfo;
    TextureCacheGetInfo(srcTexture, srcInfo, 1);

    if (srcInfo.width >= sectionInfo.size.x || srcInfo.height >= sectionInfo.size.y) {
        // Calculate source mip level appropriate for section size
        int32_t srcMipLevel = 0;
        int32_t srcWidth = srcInfo.width;
        while (srcWidth > sectionInfo.size.x) {
            srcWidth /= 2;
            srcMipLevel++;
        }

        C2iVector srcPos = { 0, 0 };

        CCharacterComponent::Paste(srcTexture, dstMips, sectionInfo.pos, srcPos, sectionInfo.size, srcInfo, srcMipLevel);
    } else {
        C2iVector srcPos = { 0, 0 };

        CCharacterComponent::PasteScale(srcTexture, dstMips, sectionInfo.pos, srcPos, sectionInfo.size, srcInfo);
    }
}

void CCharacterComponent::PasteTransparent1Bit(void* srcTexture, const BlpPalPixel* srcPal, MipBits* dstMips, const C2iVector& dstPos, uint32_t dstWidth, const C2iVector& srcPos, const C2iVector& srcSize, TCTEXTUREINFO& srcInfo, int32_t srcMipLevel, int32_t dstMipLevelOfs) {
    // TODO
}

void CCharacterComponent::PasteTransparent4Bit(void* srcTexture, const BlpPalPixel* srcPal, MipBits* dstMips, const C2iVector& dstPos, uint32_t dstWidth, const C2iVector& srcPos, const C2iVector& srcSize, TCTEXTUREINFO& srcInfo, int32_t srcMipLevel, int32_t dstMipLevelOfs) {
    // TODO
}

void CCharacterComponent::PasteTransparent8Bit(void* srcTexture, const BlpPalPixel* srcPal, MipBits* dstMips, const C2iVector& dstPos, uint32_t dstWidth, const C2iVector& srcPos, const C2iVector& srcSize, TCTEXTUREINFO& srcInfo, int32_t srcMipLevel, int32_t dstMipLevelOfs) {
    // TODO
}

void CCharacterComponent::RenderPrepAL(CCharacterComponent* component) {
    auto& itemDisplay = component->m_itemDisplays[SECTION_ARM_LOWER];

    // Skin texture

    auto skinTexture = component->m_texture[TEXTURE_INDEX(VARIATION_SKIN, 0)];
    CCharacterComponent::PasteFromSkin(SECTION_ARM_LOWER, skinTexture, CCharacterComponent::s_textureBuffer);

    // Item textures

    for (int32_t priority = 0; priority < SECTION_AL_ITEM_PRIORITIES; priority++) {
        if (itemDisplay.priorityDirty & (1 << priority)) {
            CCharacterComponent::PasteToSection(SECTION_ARM_LOWER, itemDisplay.texture[priority], CCharacterComponent::s_textureBuffer);
        }
    }
}

void CCharacterComponent::RenderPrepAU(CCharacterComponent* component) {
    auto& itemDisplay = component->m_itemDisplays[SECTION_ARM_UPPER];

    // Skin texture

    auto skinTexture = component->m_texture[TEXTURE_INDEX(VARIATION_SKIN, 0)];
    CCharacterComponent::PasteFromSkin(SECTION_ARM_UPPER, skinTexture, CCharacterComponent::s_textureBuffer);

    // Item textures

    for (int32_t priority = 0; priority < SECTION_AU_ITEM_PRIORITIES; priority++) {
        if (itemDisplay.priorityDirty & (1 << priority)) {
            CCharacterComponent::PasteToSection(SECTION_ARM_UPPER, itemDisplay.texture[priority], CCharacterComponent::s_textureBuffer);
        }
    }
}

void CCharacterComponent::RenderPrepFO(CCharacterComponent* component) {
    auto& itemDisplay = component->m_itemDisplays[SECTION_FOOT];

    // Skin texture

    auto skinTexture = component->m_texture[TEXTURE_INDEX(VARIATION_SKIN, 0)];
    CCharacterComponent::PasteFromSkin(SECTION_FOOT, skinTexture, CCharacterComponent::s_textureBuffer);

    // Item textures

    for (int32_t priority = 0; priority < SECTION_FO_ITEM_PRIORITIES; priority++) {
        if (itemDisplay.priorityDirty & (1 << priority)) {
            CCharacterComponent::PasteToSection(SECTION_FOOT, itemDisplay.texture[priority], CCharacterComponent::s_textureBuffer);
        }
    }
}

void CCharacterComponent::RenderPrepHA(CCharacterComponent* component) {
    auto& itemDisplay = component->m_itemDisplays[SECTION_HAND];

    // Skin texture

    auto skinTexture = component->m_texture[TEXTURE_INDEX(VARIATION_SKIN, 0)];
    CCharacterComponent::PasteFromSkin(SECTION_HAND, skinTexture, CCharacterComponent::s_textureBuffer);

    // Item textures

    for (int32_t priority = 0; priority < SECTION_HA_ITEM_PRIORITIES; priority++) {
        if (itemDisplay.priorityDirty & (1 << priority)) {
            CCharacterComponent::PasteToSection(SECTION_HAND, itemDisplay.texture[priority], CCharacterComponent::s_textureBuffer);
        }
    }
}

void CCharacterComponent::RenderPrepHL(CCharacterComponent* component) {
    auto sectionsRec = ComponentGetSectionsRecord(
        CCharacterComponent::s_chrVarArray,
        component->m_data.raceID,
        component->m_data.sexID,
        VARIATION_SKIN,
        0,
        component->m_data.skinColorID,
        nullptr
    );

    // Skin texture

    if (sectionsRec && sectionsRec->m_flags & 0x8) {
        auto skinTexture = component->m_texture[TEXTURE_INDEX(VARIATION_SKIN, 0)];
        CCharacterComponent::PasteFromSkin(SECTION_HEAD_LOWER, skinTexture, CCharacterComponent::s_textureBuffer);
    }

    // Face texture

    auto faceLowerTexture = component->m_texture[TEXTURE_INDEX(VARIATION_FACE, 0)];
    if (faceLowerTexture) {
        CCharacterComponent::PasteToSection(SECTION_HEAD_LOWER, faceLowerTexture, CCharacterComponent::s_textureBuffer);
    }

    // Hair textures

    auto facialHairLowerTexture = component->m_texture[TEXTURE_INDEX(VARIATION_FACIAL_HAIR, 0)];
    if (facialHairLowerTexture) {
        CCharacterComponent::PasteToSection(SECTION_HEAD_LOWER, facialHairLowerTexture, CCharacterComponent::s_textureBuffer);
    }

    auto hairLowerTexture = component->m_texture[TEXTURE_INDEX(VARIATION_HAIR, 1)];
    if (hairLowerTexture) {
        CCharacterComponent::PasteToSection(SECTION_HEAD_LOWER, hairLowerTexture, CCharacterComponent::s_textureBuffer);
    }
}

void CCharacterComponent::RenderPrepHU(CCharacterComponent* component) {
    auto sectionsRec = ComponentGetSectionsRecord(
        CCharacterComponent::s_chrVarArray,
        component->m_data.raceID,
        component->m_data.sexID,
        VARIATION_SKIN,
        0,
        component->m_data.skinColorID,
        nullptr
    );

    // Skin texture

    if (sectionsRec && sectionsRec->m_flags & 0x8) {
        auto skinTexture = component->m_texture[TEXTURE_INDEX(VARIATION_SKIN, 0)];
        CCharacterComponent::PasteFromSkin(SECTION_HEAD_UPPER, skinTexture, CCharacterComponent::s_textureBuffer);
    }

    // Face texture

    auto faceUpperTexture = component->m_texture[TEXTURE_INDEX(VARIATION_FACE, 1)];
    if (faceUpperTexture) {
        CCharacterComponent::PasteToSection(SECTION_HEAD_UPPER, faceUpperTexture, CCharacterComponent::s_textureBuffer);
    }

    // Hair textures

    auto facialHairUpperTexture = component->m_texture[TEXTURE_INDEX(VARIATION_FACIAL_HAIR, 1)];
    if (facialHairUpperTexture) {
        CCharacterComponent::PasteToSection(SECTION_HEAD_UPPER, facialHairUpperTexture, CCharacterComponent::s_textureBuffer);
    }

    auto hairUpperTexture = component->m_texture[TEXTURE_INDEX(VARIATION_HAIR, 2)];
    if (hairUpperTexture) {
        CCharacterComponent::PasteToSection(SECTION_HEAD_UPPER, hairUpperTexture, CCharacterComponent::s_textureBuffer);
    }
}

void CCharacterComponent::RenderPrepLL(CCharacterComponent* component) {
    auto& itemDisplay = component->m_itemDisplays[SECTION_LEG_LOWER];

    // Skin texture

    auto skinTexture = component->m_texture[TEXTURE_INDEX(VARIATION_SKIN, 0)];
    CCharacterComponent::PasteFromSkin(SECTION_LEG_LOWER, skinTexture, CCharacterComponent::s_textureBuffer);

    // Item textures

    auto firstPriority = 0;
    auto itemPriorities = SECTION_LL_ITEM_PRIORITIES;

    if (component->m_flags & 0x20) {
        firstPriority = 1;
        itemPriorities = SECTION_LL_ITEM_PRIORITIES - 2;
    }

    for (int32_t priority = firstPriority; priority < itemPriorities; priority++) {
        if (itemDisplay.priorityDirty & (1 << priority)) {
            CCharacterComponent::PasteToSection(SECTION_LEG_LOWER, itemDisplay.texture[priority], CCharacterComponent::s_textureBuffer);
        }
    }
}

void CCharacterComponent::RenderPrepLU(CCharacterComponent* component) {
    auto& itemDisplay = component->m_itemDisplays[SECTION_LEG_UPPER];

    // Skin texture

    auto skinTexture = component->m_texture[TEXTURE_INDEX(VARIATION_SKIN, 0)];
    CCharacterComponent::PasteFromSkin(SECTION_LEG_UPPER, skinTexture, CCharacterComponent::s_textureBuffer);

    // Underwear texture

    if ((component->m_flags & 0x20) || !(itemDisplay.priorityDirty & ((1 << 0) | (1 << 1)))) {
        auto bottomUnderwearTexture = component->m_texture[TEXTURE_INDEX(VARIATION_UNDERWEAR, 0)];
        if (bottomUnderwearTexture) {
            CCharacterComponent::PasteToSection(SECTION_LEG_UPPER, bottomUnderwearTexture, CCharacterComponent::s_textureBuffer);
        }
    }

    // Item textures

    auto firstPriority = (component->m_flags & 0x20) ? 1 : 0;

    for (int32_t priority = firstPriority; priority < SECTION_LU_ITEM_PRIORITIES; priority++) {
        if (itemDisplay.priorityDirty & (1 << priority)) {
            CCharacterComponent::PasteToSection(SECTION_LEG_UPPER, itemDisplay.texture[priority], CCharacterComponent::s_textureBuffer);
        }
    }
}

void CCharacterComponent::RenderPrepTL(CCharacterComponent* component) {
    auto& itemDisplay = component->m_itemDisplays[SECTION_TORSO_LOWER];

    // Skin texture

    auto skinTexture = component->m_texture[TEXTURE_INDEX(VARIATION_SKIN, 0)];
    CCharacterComponent::PasteFromSkin(SECTION_TORSO_LOWER, skinTexture, CCharacterComponent::s_textureBuffer);

    // Item textures

    for (int32_t priority = 0; priority < SECTION_TL_ITEM_PRIORITIES; priority++) {
        if (itemDisplay.priorityDirty & (1 << priority)) {
            CCharacterComponent::PasteToSection(SECTION_TORSO_LOWER, itemDisplay.texture[priority], CCharacterComponent::s_textureBuffer);
        }
    }
}

void CCharacterComponent::RenderPrepTU(CCharacterComponent* component) {
    auto& itemDisplay = component->m_itemDisplays[SECTION_TORSO_UPPER];

    // Skin texture

    auto skinTexture = component->m_texture[TEXTURE_INDEX(VARIATION_SKIN, 0)];
    CCharacterComponent::PasteFromSkin(SECTION_TORSO_UPPER, skinTexture, CCharacterComponent::s_textureBuffer);

    // Underwear texture

    if (!(itemDisplay.priorityDirty & ((1 << 0) | (1 << 1) | (1 << 2)))) {
        auto topUnderwearTexture = component->m_texture[TEXTURE_INDEX(VARIATION_UNDERWEAR, 1)];
        if (topUnderwearTexture) {
            CCharacterComponent::PasteToSection(SECTION_TORSO_UPPER, topUnderwearTexture, CCharacterComponent::s_textureBuffer);
        }
    }

    // Item textures

    for (int32_t priority = 0; priority < SECTION_TU_ITEM_PRIORITIES; priority++) {
        if (itemDisplay.priorityDirty & (1 << priority)) {
            CCharacterComponent::PasteToSection(SECTION_TORSO_UPPER, itemDisplay.texture[priority], CCharacterComponent::s_textureBuffer);
        }
    }
}

void CCharacterComponent::UpdateBaseTexture(EGxTexCommand cmd, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipLevel, void* userArg, uint32_t& texelStrideInBytes, const void*& texels) {
    auto component = static_cast<CCharacterComponent*>(userArg);

    switch(cmd) {
    case GxTex_Lock: {
        if (!s_bInRenderPrep) {
            component->RenderPrepAll();
        }

        break;
    }

    case GxTex_Latch: {
        if (component->m_textureFormat == GxTex_Dxt1) {
            // TODO
            STORM_ASSERT(false);
        } else {
            texelStrideInBytes = 4 * width;
        }

        // TODO conditional check on some member of component

        auto buffer = component->m_textureFormat == GxTex_Dxt1
            ? CCharacterComponent::s_textureBufferCompressed
            : CCharacterComponent::s_textureBuffer;

        texels = buffer[mipLevel].mip[0];

        break;
    }

    // TODO unknown command
    case 3: {
        // TODO
        break;
    }
    }
}

void CCharacterComponent::CreateBaseTexture() {
    auto dataFormat = this->m_textureFormat == GxTex_Dxt1
        ? GxTex_Dxt1
        : GxTex_Argb8888;

    CGxTexFlags flags = CGxTexFlags(GxTex_LinearMipLinear, 0, 0, 0, 0, 0, 1);
    if (GxDevApi() == GxApi_GLL) {
        flags.m_bit15 = 1;
    }

    auto baseTexture = TextureCreate(
       CCharacterComponent::s_textureSize,
       CCharacterComponent::s_textureSize,
       this->m_textureFormat,
       dataFormat,
       flags,
       this,
       &CCharacterComponent::UpdateBaseTexture,
       "CharacterBaseSkin",
       1
    );

    this->m_baseTexture = baseTexture;

    this->m_data.model->ReplaceTexture(1, this->m_baseTexture);
}

void CCharacterComponent::GeosRenderPrep() {
    // Check for eye glow

    bool eyeGlow = false;

    if (this->m_data.classID == 6) {
        eyeGlow = true;
    } else {
        auto sectionsRec = this->GetSectionsRecord(VARIATION_FACE, this->m_data.faceID, this->m_data.skinColorID, nullptr);

        if (sectionsRec && sectionsRec->m_flags & 0x4) {
            eyeGlow = true;
        }
    }

    // Hide all geosets (0 - 2000)

    this->m_data.model->SetGeometryVisible(0, 2000, 0);

    // Show base skin geoset (0)

    this->m_data.model->SetGeometryVisible(0, 0, 1);

    // Show all enabled geosets

    for (int32_t geoset = 0; geoset < NUM_GEOSET; geoset++) {
        if (geoset == GEOSET_EYE_EFFECTS && eyeGlow) {
            this->m_data.model->SetGeometryVisible(1703, 1703, 1);
        } else {
            this->m_data.model->SetGeometryVisible(this->m_data.geosets[geoset], this->m_data.geosets[geoset], 1);
        }
    }

    // TODO

    this->m_flags &= ~0x4;
}

CharSectionsRec* CCharacterComponent::GetSectionsRecord(COMPONENT_VARIATIONS sectionIndex, int32_t variationIndex, int32_t colorIndex, bool* found) {
    return ComponentGetSectionsRecord(
        CCharacterComponent::s_chrVarArray,
        this->m_data.raceID,
        this->m_data.sexID,
        sectionIndex,
        variationIndex,
        colorIndex,
        found
    );
}

int32_t CCharacterComponent::Init(ComponentData* data, const char* a3) {
    // If existing model is present, release it before copying in new data
    if (this->m_data.model) {
        this->m_data.model->Release();
    }

    this->m_data = *data;

    // TODO

    this->SetSkinColor(this->m_data.skinColorID, false, true, a3);
    this->SetHairStyle(this->m_data.hairStyleID, a3);
    this->SetBeardStyle(this->m_data.facialHairStyleID, false, a3);

    return 1;
}

int32_t CCharacterComponent::ItemsLoaded(int32_t a2) {
    // TODO

    return 1;
}

void CCharacterComponent::LoadBaseVariation(COMPONENT_VARIATIONS sectionIndex, int32_t textureIndex, int32_t variationIndex, int32_t colorIndex, COMPONENT_SECTIONS section, const char* a7) {
    int32_t index = TEXTURE_INDEX(sectionIndex, textureIndex);

    if (this->m_texture[index]) {
        TextureCacheDestroyTexture(this->m_texture[index]);
    }

    auto valid = ComponentValidateBase(
        CCharacterComponent::s_chrVarArray,
        this->m_data.raceID,
        this->m_data.sexID,
        sectionIndex,
        variationIndex,
        colorIndex
    );
    if (!valid) {
        return;
    }

    auto sectionsRec = this->GetSectionsRecord(sectionIndex, variationIndex, colorIndex, nullptr);

    auto textureName = sectionsRec->m_textureName[textureIndex];

    if (*textureName) {
        SStrCopy(s_pathEnd, textureName);
        this->m_texture[index] = TextureCacheCreateTexture(s_path);
    }

    this->m_sectionDirty |= 1 << section;

    // TODO

    this->m_flags &= ~0x8;
}

void CCharacterComponent::PrepSections() {
    // TODO
}

int32_t CCharacterComponent::RenderPrep(int32_t a2) {
    if (this->m_data.flags & 0x1) {
        if (this->m_flags & 0x4) {
            this->GeosRenderPrep();
        }

        return 1;
    }

    // TODO

    if (a2) {
        // TODO

        this->VariationsLoaded(1);
        this->ItemsLoaded(1);

        this->m_flags |= 8u;

        this->RenderPrepSections();
        // TODO this->Sub79F820();

        return 1;
    }

    // TODO
    return 1;
}

void CCharacterComponent::RenderPrepAll() {
    // TODO

    this->m_flags &= ~0x8;

    this->VariationsLoaded(1);
    this->ItemsLoaded(1);

    for (uint32_t i = 0; i < NUM_COMPONENT_SECTIONS; i++) {
        CCharacterComponent::s_prepFunc[i](this);
    }

    // TODO

    this->m_flags &= ~0x1;

    this->m_sectionDirty = 0;

    // TODO
}

void CCharacterComponent::RenderPrepSections() {
    s_bInRenderPrep = 1;

    if (this->m_flags & 0x4) {
        this->GeosRenderPrep();
    }

    if (!this->m_baseTexture) {
        this->CreateBaseTexture();
    }

    this->PrepSections();

    this->m_flags &= ~0x1;

    this->m_sectionDirty = 0;

    // TODO

    s_bInRenderPrep = 0;
}

void CCharacterComponent::ReplaceExtraSkinTexture(const char* a2) {
    // TODO
}

void CCharacterComponent::ReplaceHairTexture(int32_t hairStyleID, const char* a3) {
    if (!ComponentValidateBase(
       CCharacterComponent::s_chrVarArray,
       this->m_data.raceID,
       this->m_data.sexID,
       VARIATION_HAIR,
       hairStyleID,
       this->m_data.hairColorID
    )) {
        return;
    }

    auto sectionsRec = this->GetSectionsRecord(VARIATION_HAIR, hairStyleID, this->m_data.hairColorID, nullptr);
    if (!*sectionsRec->m_textureName[0]) {
        return;
    }

    SStrCopy(s_pathEnd, sectionsRec->m_textureName[0]);

    auto hairTexture = CCharacterComponent::CreateTexture(s_path, &s_status);
    if (hairTexture) {
        this->m_data.model->ReplaceTexture(6, hairTexture);
        HandleClose(hairTexture);
    }
}

void CCharacterComponent::SetBeardStyle(int32_t facialHairStyleID, bool a3, const char* a4) {
    auto listIndex = this->m_data.raceID * 2 + this->m_data.sexID;
    if (facialHairStyleID < 0 || facialHairStyleID > CCharacterComponent::s_characterFacialHairStylesList[listIndex]) {
        return;
    }

    this->m_data.facialHairStyleID = facialHairStyleID;

    auto facialHairStyleRec = ComponentGetFacialHairStyleRecord(&this->m_data);

    if (facialHairStyleRec) {
        this->m_data.geosets[1] = 100 + facialHairStyleRec->m_geoset[0];
        this->m_data.geosets[2] = 200 + facialHairStyleRec->m_geoset[2];
        this->m_data.geosets[3] = 300 + facialHairStyleRec->m_geoset[1];
        this->m_data.geosets[16] = 1600 + facialHairStyleRec->m_geoset[3];
        this->m_data.geosets[17] = 1700 + facialHairStyleRec->m_geoset[4];

        this->m_flags |= 0x4;

        if (
            (facialHairStyleRec->m_geoset[0] || facialHairStyleRec->m_geoset[1] || facialHairStyleRec->m_geoset[2])
            && (!this->m_texture[TEXTURE_INDEX(VARIATION_HAIR, 1)] || !this->m_texture[TEXTURE_INDEX(VARIATION_HAIR, 2)])
        ) {
            this->ReplaceHairTexture(1, a4);
        }
    }

    bool isNPC = this->m_data.flags & 0x1;

    if (!isNPC) {
        if (a3) {
            this->LoadBaseVariation(VARIATION_FACIAL_HAIR, 0, this->m_data.facialHairStyleID, this->m_data.hairColorID, SECTION_HEAD_LOWER, a4);
            this->LoadBaseVariation(VARIATION_FACIAL_HAIR, 1, this->m_data.facialHairStyleID, this->m_data.hairColorID, SECTION_HEAD_UPPER, a4);
        }

        this->m_sectionDirty |= (1 << SECTION_HEAD_LOWER) | (1 << SECTION_HEAD_UPPER);

        // TODO component request logic

        this->m_flags &= ~0x8;
    }
}

void CCharacterComponent::SetFace(int32_t faceID, bool a3, const char* a4) {
    bool isNPC = this->m_data.flags & 0x1;

    if (isNPC) {
        return;
    }

    auto skinSectionsRec = this->GetSectionsRecord(VARIATION_SKIN, 0, this->m_data.skinColorID, nullptr);

    if (skinSectionsRec && skinSectionsRec->m_flags & 0x8) {
        return;
    }

    if (!ComponentValidateBase(CCharacterComponent::s_chrVarArray, this->m_data.raceID, this->m_data.sexID, VARIATION_FACE, faceID, this->m_data.skinColorID)) {
        return;
    }

    this->m_data.faceID = faceID;

    this->LoadBaseVariation(VARIATION_FACE, 0, this->m_data.faceID, this->m_data.skinColorID, SECTION_HEAD_LOWER, a4);
    this->LoadBaseVariation(VARIATION_FACE, 1, this->m_data.faceID, this->m_data.skinColorID, SECTION_HEAD_UPPER, a4);

    if (a3) {
        this->LoadBaseVariation(VARIATION_HAIR, 1, this->m_data.hairStyleID, this->m_data.hairColorID, SECTION_HEAD_LOWER, a4);
        this->LoadBaseVariation(VARIATION_HAIR, 2, this->m_data.hairStyleID, this->m_data.hairColorID, SECTION_HEAD_UPPER, a4);
    }

    this->m_flags |= 0x4;
    this->m_sectionDirty |= (1 << SECTION_HEAD_LOWER) | (1 << SECTION_HEAD_UPPER);

    // TODO

    this->m_flags &= ~0x8;
}

void CCharacterComponent::SetHairColor(int32_t hairColorID, bool a3, const char* a4) {
    if (!ComponentValidateBase(
        CCharacterComponent::s_chrVarArray,
        this->m_data.raceID,
        this->m_data.sexID,
        VARIATION_HAIR,
        this->m_data.hairStyleID,
        hairColorID
    )) {
        return;
    }

    this->m_data.hairColorID = hairColorID;

    auto raceRec = g_chrRacesDB.GetRecord(this->m_data.raceID);

    if (this->m_data.sexID == UNITSEX_MALE && this->m_data.hairStyleID == 0 && raceRec->m_flags & 0x8) {
        this->ReplaceHairTexture(1, a4);
    }

    this->ReplaceHairTexture(this->m_data.hairStyleID, a4);

    bool isNPC = this->m_data.flags & 0x1;

    if (!isNPC) {
        if (a3) {
            this->LoadBaseVariation(VARIATION_HAIR, 1, this->m_data.hairStyleID, this->m_data.hairColorID, SECTION_HEAD_LOWER, a4);
            this->LoadBaseVariation(VARIATION_HAIR, 2, this->m_data.hairStyleID, this->m_data.hairColorID, SECTION_HEAD_UPPER, a4);
        }

        this->LoadBaseVariation(VARIATION_FACIAL_HAIR, 0, this->m_data.facialHairStyleID, this->m_data.hairColorID, SECTION_HEAD_LOWER, a4);
        this->LoadBaseVariation(VARIATION_FACIAL_HAIR, 1, this->m_data.facialHairStyleID, this->m_data.hairColorID, SECTION_HEAD_UPPER, a4);
    }
}

void CCharacterComponent::SetHairStyle(int32_t hairStyleID, const char* a3) {
    if (!ComponentValidateBase(
        CCharacterComponent::s_chrVarArray,
        this->m_data.raceID,
        this->m_data.sexID,
        VARIATION_HAIR,
        hairStyleID,
        this->m_data.hairColorID
    )) {
        return;
    }

    this->m_data.hairStyleID = hairStyleID;

    auto hairGeoset = ComponentGetHairGeoset(&this->m_data);
    this->m_data.geosets[0] = hairGeoset;

    bool isNPC = this->m_data.flags & 0x1;

    if (!isNPC) {
        this->LoadBaseVariation(VARIATION_HAIR, 1, this->m_data.hairStyleID, this->m_data.hairColorID, SECTION_HEAD_LOWER, a3);
        this->LoadBaseVariation(VARIATION_HAIR, 2, this->m_data.hairStyleID, this->m_data.hairColorID, SECTION_HEAD_UPPER, a3);
    }

    this->SetHairColor(this->m_data.hairColorID, false, a3);

    this->m_flags |= 0x4;
    this->m_sectionDirty |= (1 << SECTION_HEAD_LOWER) | (1 << SECTION_HEAD_UPPER);

    // TODO component request logic

    this->m_flags &= ~0x8;
}

void CCharacterComponent::SetSkinColor(int32_t skinColorID, bool a3, bool a4, const char* a5) {
    bool isNPC = this->m_data.flags & 0x1;

    this->m_data.skinColorID = skinColorID;

    if (isNPC) {
        return;
    }

    if (!ComponentValidateBase(CCharacterComponent::s_chrVarArray, this->m_data.raceID, this->m_data.sexID, VARIATION_SKIN, 0, skinColorID)) {
        return;
    }

    auto numColors = ComponentGetNumColors(
        CCharacterComponent::s_chrVarArray,
        this->m_data.raceID,
        this->m_data.sexID,
        VARIATION_SKIN,
        0
    );

    auto sectionsRec = ComponentGetSectionsRecord(
        CCharacterComponent::s_chrVarArray,
        this->m_data.raceID,
        this->m_data.sexID,
        VARIATION_SKIN,
        0,
        skinColorID,
        nullptr
    );

    if (skinColorID < numColors && sectionsRec && !(sectionsRec->m_flags & 0x8)) {
        auto underwearRec = this->GetSectionsRecord(VARIATION_UNDERWEAR, 0, skinColorID, nullptr);

        auto t0 = TEXTURE_INDEX(VARIATION_UNDERWEAR, 0);
        auto t1 = TEXTURE_INDEX(VARIATION_UNDERWEAR, 1);

        if (this->m_texture[t0]) {
            TextureCacheDestroyTexture(this->m_texture[t0]);
            this->m_texture[t0] = nullptr;
        }

        if (this->m_texture[t1]) {
            TextureCacheDestroyTexture(this->m_texture[t1]);
            this->m_texture[t1] = nullptr;
        }

        if (*underwearRec->m_textureName[0]) {
            SStrCopy(s_pathEnd, underwearRec->m_textureName[0]);
            this->m_texture[t0] = TextureCacheCreateTexture(s_path);
            STORM_ASSERT(this->m_texture[t0]);
        }

        if (*underwearRec->m_textureName[1]) {
            SStrCopy(s_pathEnd, underwearRec->m_textureName[1]);
            this->m_texture[t1] = TextureCacheCreateTexture(s_path);
            STORM_ASSERT(this->m_texture[t1]);
        }
    }

    this->ReplaceExtraSkinTexture(a5);

    this->LoadBaseVariation(
        VARIATION_SKIN,
        0,
        0,
        this->m_data.skinColorID,
        SECTION_TORSO_UPPER,
        a5
    );

    this->SetFace(this->m_data.faceID, a3, a5);

    if (a4) {
        this->m_flags |= 0x1;

        // TODO component request logic

        this->m_flags &= ~0x8;
    }

    this->m_sectionDirty = -1;

    // TODO component request logic

    this->m_flags &= ~0x8;
}

int32_t CCharacterComponent::VariationsLoaded(int32_t a2) {
    TCTEXTUREINFO info;

    for (int32_t v = 0; v < NUM_COMPONENT_VARIATIONS; v++) {
        for (int32_t t = 0; t < 3; t++) {
            auto texture = this->m_texture[TEXTURE_INDEX(v, t)];

            if (texture && !TextureCacheGetInfo(texture, info, a2)) {
                return 0;
            }
        }
    }

    this->m_flags &= ~0x2;

    return 1;
}
