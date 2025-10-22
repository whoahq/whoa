#include "component/CCharacterComponent.hpp"
#include "component/Texture.hpp"
#include "component/Util.hpp"
#include "db/Db.hpp"
#include "gx/Device.hpp"
#include "gx/Texture.hpp"
#include "model/CM2Model.hpp"
#include "object/Types.hpp"
#include <storm/Memory.hpp>
#include <storm/String.hpp>

st_race* CCharacterComponent::s_chrVarArray;
uint32_t CCharacterComponent::s_chrVarArrayLength;
EGxTexFormat CCharacterComponent::s_gxFormat;
uint32_t CCharacterComponent::s_mipLevels;
PREP_FUNC* CCharacterComponent::s_prepFunc[];
MipBits* CCharacterComponent::s_textureBuffer;
MipBits* CCharacterComponent::s_textureBufferCompressed;
uint32_t CCharacterComponent::s_textureSize;

int32_t s_bInRenderPrep = 0;
char* s_pathEnd;
char s_path[STORM_MAX_PATH];

#define TEXTURE_INDEX(section, texture) (3 * section + texture)

CCharacterComponent* CCharacterComponent::AllocComponent() {
    // TODO ObjectAlloc
    return STORM_NEW(CCharacterComponent);
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
    // TODO CountFacialFeatures(varArrayLength, &CCharacterComponent::s_characterFacialHairStylesList);
}

void CCharacterComponent::RenderPrepAL(CCharacterComponent* component) {
    // TODO
}

void CCharacterComponent::RenderPrepAU(CCharacterComponent* component) {
    // TODO
}

void CCharacterComponent::RenderPrepFO(CCharacterComponent* component) {
    // TODO
}

void CCharacterComponent::RenderPrepHA(CCharacterComponent* component) {
    // TODO
}

void CCharacterComponent::RenderPrepHL(CCharacterComponent* component) {
    // TODO
}

void CCharacterComponent::RenderPrepHU(CCharacterComponent* component) {
    // TODO
}

void CCharacterComponent::RenderPrepLL(CCharacterComponent* component) {
    // TODO
}

void CCharacterComponent::RenderPrepLU(CCharacterComponent* component) {
    // TODO
}

void CCharacterComponent::RenderPrepTL(CCharacterComponent* component) {
    // TODO
}

void CCharacterComponent::RenderPrepTU(CCharacterComponent* component) {
    // TODO
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
    // TODO

    // Default all sections (0 - 2000) to hidden
    this->m_data.model->SetGeometryVisible(0, 2000, 0);

    // Show base "skin" section (0)
    this->m_data.model->SetGeometryVisible(0, 0, 1);

    // Show selected geosets
    for (int32_t i = 0; i < NUM_GEOSET; i++) {
        // TODO handle 1703

        this->m_data.model->SetGeometryVisible(this->m_data.geosets[i], this->m_data.geosets[i], 1);
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

void CCharacterComponent::Init(ComponentData* data, const char* a3) {
    if (data->model) {
        data->model->Release();
    }

    this->m_data = *data;

    // TODO

    this->SetSkinColor(this->m_data.skinColorID, false, true, a3);

    // TODO
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

    this->m_sections |= 1 << section;

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
}

void CCharacterComponent::RenderPrepAll() {
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

    // TODO

    s_bInRenderPrep = 0;
}

void CCharacterComponent::ReplaceExtraSkinTexture(const char* a2) {
    // TODO
}

void CCharacterComponent::SetFace(int32_t faceID, bool a3, const char* a4) {
    // TODO
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
        // TODO underwear
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

    // TODO
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
