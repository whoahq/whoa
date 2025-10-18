#include "component/CCharacterComponent.hpp"
#include "component/Texture.hpp"
#include "component/Util.hpp"
#include "db/Db.hpp"
#include "model/CM2Model.hpp"
#include "object/Types.hpp"
#include <storm/Memory.hpp>
#include <storm/String.hpp>

st_race* CCharacterComponent::s_chrVarArray;
uint32_t CCharacterComponent::s_chrVarArrayLength;

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

    // TODO

    CCharacterComponent::InitDbData();

    // TODO
}

void CCharacterComponent::InitDbData() {
    uint32_t varArrayLength = (g_chrRacesDB.m_maxID + 1) * UNITSEX_NUM_SEXES;
    CCharacterComponent::s_chrVarArrayLength = varArrayLength;

    BuildComponentArray(varArrayLength, &CCharacterComponent::s_chrVarArray);
    // TODO CountFacialFeatures(varArrayLength, &CCharacterComponent::s_characterFacialHairStylesList);
}

void CCharacterComponent::CreateBaseTexture() {
    // TODO
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
    // TODO

    return 1;
}
