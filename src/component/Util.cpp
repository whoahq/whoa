#include "component/Util.hpp"
#include "component/Types.hpp"
#include "db/Db.hpp"
#include "object/Types.hpp"
#include <storm/Memory.hpp>

int32_t BuildComponentArray(uint32_t varArrayLength, st_race** varArrayPtr) {
    if (!varArrayLength) {
        return 0;
    }

    auto varArray = new (STORM_ALLOC(sizeof(st_race) * varArrayLength)) st_race[varArrayLength];

    int32_t prevRaceID = g_charSectionsDB.GetNumRecords() > 0 ? g_charSectionsDB.GetRecordByIndex(0)->m_raceID : 4;
    int32_t prevSexID = g_charSectionsDB.GetNumRecords() > 0 ? g_charSectionsDB.GetRecordByIndex(0)->m_sexID : 8;
    int32_t prevBaseSection = g_charSectionsDB.GetNumRecords() > 0 ? g_charSectionsDB.GetRecordByIndex(0)->m_baseSection : 12;
    int32_t prevVariationIndex = 0;

    // Build sections

    int32_t variationIndex = 0;

    for (int32_t i = 0; i < g_charSectionsDB.GetNumRecords(); i++) {
        auto sectionsRec = g_charSectionsDB.GetRecordByIndex(i);

        if (sectionsRec->m_baseSection >= NUM_COMPONENT_VARIATIONS) {
            continue;
        }

        auto sectionChange = prevRaceID != sectionsRec->m_raceID
            || prevSexID != sectionsRec->m_sexID
            || prevBaseSection != sectionsRec->m_baseSection;

        auto lastRecord = i == g_charSectionsDB.GetNumRecords() - 1;

        if (sectionChange || lastRecord) {
            auto& section = varArray[(prevRaceID * UNITSEX_NUM_SEXES + prevSexID)].sections[prevBaseSection];

            section.variationCount = variationIndex + 1;

            if (section.variationCount > 0) {
                section.variationArray = new (STORM_ALLOC(sizeof(st_variation) * section.variationCount)) st_variation[section.variationCount];
            }

            variationIndex = 0;
        }

        if (variationIndex <= sectionsRec->m_variationIndex) {
            variationIndex = sectionsRec->m_variationIndex;
        }

        prevRaceID = sectionsRec->m_raceID;
        prevSexID = sectionsRec->m_sexID;
        prevBaseSection = sectionsRec->m_baseSection;
    }

    // Build variations

    prevRaceID = 1;
    prevSexID = 0;
    prevBaseSection = 0;
    prevVariationIndex = 0;

    int32_t colorIndex = 0;

    for (int32_t i = 0; i < g_charSectionsDB.GetNumRecords(); i++) {
        auto sectionsRec = g_charSectionsDB.GetRecordByIndex(i);

        if (sectionsRec->m_baseSection >= NUM_COMPONENT_VARIATIONS) {
            continue;
        }

        auto sectionChange = prevRaceID != sectionsRec->m_raceID
            || prevSexID != sectionsRec->m_sexID
            || prevBaseSection != sectionsRec->m_baseSection
            || prevVariationIndex != sectionsRec->m_variationIndex;

        auto lastRecord = i == g_charSectionsDB.GetNumRecords() - 1;

        if (sectionChange || lastRecord) {
            auto& section = varArray[(prevRaceID * UNITSEX_NUM_SEXES + prevSexID)].sections[prevBaseSection];

            if (section.variationCount > 0) {
                auto& variation = section.variationArray[prevVariationIndex];

                variation.colorCount = colorIndex + 1;

                if (variation.colorCount > 0) {
                    variation.colorArray = new (STORM_ALLOC(sizeof(st_color) * variation.colorCount)) st_color[variation.colorCount];
                }
            }

            colorIndex = 0;
        }

        if (colorIndex <= sectionsRec->m_colorIndex) {
            colorIndex = sectionsRec->m_colorIndex;
        }

        prevRaceID = sectionsRec->m_raceID;
        prevSexID = sectionsRec->m_sexID;
        prevBaseSection = sectionsRec->m_baseSection;
        prevVariationIndex = sectionsRec->m_variationIndex;
    }

    // Build colors

    for (int32_t i = 0; i < g_charSectionsDB.GetNumRecords(); i++) {
        auto sectionsRec = g_charSectionsDB.GetRecordByIndex(i);

        if (sectionsRec->m_baseSection >= NUM_COMPONENT_VARIATIONS) {
            continue;
        }

        auto& section = varArray[(sectionsRec->m_raceID * UNITSEX_NUM_SEXES + sectionsRec->m_sexID)].sections[sectionsRec->m_baseSection];

        if (section.variationCount > 0 && sectionsRec->m_variationIndex < section.variationCount) {
            auto& variation = section.variationArray[sectionsRec->m_variationIndex];

            if (variation.colorCount > 0 && sectionsRec->m_colorIndex < variation.colorCount) {
                auto& color = variation.colorArray[sectionsRec->m_colorIndex];
                color.rec = sectionsRec;
            }
        }
    }

    *varArrayPtr = varArray;

    return 1;
}

int32_t ComponentGetHairGeoset(ComponentData* data) {
    for (int32_t i = 0; i < g_charHairGeosetsDB.GetNumRecords(); i++) {
        auto hairGeosetRec = g_charHairGeosetsDB.GetRecordByIndex(i);

        if (hairGeosetRec->m_raceID == data->raceID && hairGeosetRec->m_sexID == data->sexID && hairGeosetRec->m_variationID == data->hairStyleID) {
            auto geosetId = hairGeosetRec->m_geosetID;
            return geosetId > 0 ? geosetId : 1;
        }
    }

    return 1;
}

int32_t ComponentGetNumColors(st_race* varArray, int32_t raceId, int32_t sexId, COMPONENT_VARIATIONS sectionIndex, int32_t variationIndex) {
    auto& section = varArray[(raceId * UNITSEX_NUM_SEXES + sexId)].sections[sectionIndex];

    if (variationIndex >= section.variationCount || section.variationCount == 0) {
        return 0;
    }

    if (!section.variationArray || !section.variationArray[variationIndex].colorArray) {
        return 0;
    }

    return section.variationArray[variationIndex].colorCount;
}

CharSectionsRec* ComponentGetSectionsRecord(st_race* varArray, int32_t raceId, int32_t sexId, COMPONENT_VARIATIONS sectionIndex, int32_t variationIndex, int32_t colorIndex, bool* found) {
    if (!ComponentValidateBase(varArray, raceId, sexId, sectionIndex, variationIndex, colorIndex)) {
        if (found) {
            *found = false;
        }

        return nullptr;
    }

    auto& section = varArray[(raceId * UNITSEX_NUM_SEXES + sexId)].sections[sectionIndex];

    if (found) {
        *found = true;
    }

    return section.variationArray[variationIndex].colorArray[colorIndex].rec;
}

int32_t ComponentValidateBase(st_race* varArray, int32_t raceId, int32_t sexId, COMPONENT_VARIATIONS sectionIndex, int32_t variationIndex, int32_t colorIndex) {
    if (sectionIndex >= NUM_COMPONENT_VARIATIONS || variationIndex < 0) {
        return 0;
    }

    auto& section = varArray[(raceId * UNITSEX_NUM_SEXES + sexId)].sections[sectionIndex];

    if (variationIndex >= section.variationCount || section.variationCount == 0) {
        return 0;
    }

    if (colorIndex < 0) {
        return 0;
    }

    auto& variation = section.variationArray[variationIndex];

    if (colorIndex >= variation.colorCount || variation.colorCount == 0) {
        return 0;
    }

    return 1;
}
