#include "glue/CRealmList.hpp"
#include "client/ClientServices.hpp"
#include "db/Db.hpp"
#include "ui/FrameScript.hpp"
#include <new>
#include <storm/Memory.hpp>
#include <storm/String.hpp>

float CRealmList::s_avgLoad;
TSFixedArray<RealmCategory*> CRealmList::s_categories;
int32_t CRealmList::s_preferredCategory = -1;
int32_t CRealmList::s_selectedCategory = -1;

void CRealmList::Initialize() {
    CRealmList::s_categories.SetCount(g_cfg_CategoriesDB.m_numRecords);

    for (int32_t i = 0; i < g_cfg_CategoriesDB.m_numRecords; i++) {
        auto m = SMemAlloc(sizeof(RealmCategory), __FILE__, __LINE__, 0x0);
        auto category = new (m) RealmCategory();

        CRealmList::s_categories[i] = category;
        CRealmList::s_categories[i]->m_category = g_cfg_CategoriesDB.GetRecordByIndex(i);
        CRealmList::s_categories[i]->uint14 = 0;
    }

    // TODO Initialize CRealmList::s_sortCriteria
}

void CRealmList::SetPreferredInfo(uint32_t categoryIndex, int32_t pvp, int32_t rp) {
    if (CRealmList::s_categories.Count() == 0) {
        return;
    }

    auto selectedCategory = CRealmList::Sub4DE910(categoryIndex - 1);
    if (selectedCategory > CRealmList::s_categories.Count()) {
        selectedCategory = 0;
    }

    CRealmList::s_selectedCategory = static_cast<int32_t>(selectedCategory);
    CRealmList::s_preferredCategory = CRealmList::s_categories[selectedCategory]->m_category->GetID();

    auto realmCategory = CRealmList::s_categories[selectedCategory];
    int32_t realmId = -1;

    // Attempt to find realm that matches selected category and pvp / rp type
    for (int32_t i = 0; i < realmCategory->uint14; i++) {
        auto realmIndex = static_cast<int32_t>(realmCategory->m_realms[i]);
        auto realmInfo = ClientServices::GetRealmInfoByIndex(realmIndex);

        // 0x1 - INVALID, 0x2 - OFFLINE
        if (realmInfo->flags & (0x1 | 0x2)) {
            continue;
        }

        // 0x20 - FORCE_RECOMMENDED
        if (realmInfo->flags & 0x20) {
            realmId = i + 1;
            break;
        }

        // Look up realm pvp / rp type based on realm type
        int32_t realmIsPvp = 0;
        int32_t realmIsRp = 0;
        for (int32_t j = 0; j < g_cfg_ConfigsDB.m_numRecords; j++) {
            auto config = g_cfg_ConfigsDB.GetRecordByIndex(j);

            if (config->m_realmType == realmInfo->type) {
                realmIsPvp = config->m_playerKillingAllowed != 0;
                realmIsRp = config->m_roleplaying != 0;
                break;
            }
        }

        if (pvp == realmIsPvp && rp == realmIsRp) {
            realmId = i + 1;

            // 0x80 - FORCE_FULL
            if (!(realmInfo->flags & 0x80)) {
                break;
            }
        }
    }

    // No match found for selected category and pvp / rp type
    if (realmId == -1) {
        FrameScript_SignalEvent(9, nullptr);
        return;
    }

    FrameScript_SignalEvent(18, "%d%d", categoryIndex, realmId);
}

uint32_t CRealmList::Sub4DE910(uint32_t a1) {
    if (CRealmList::s_categories.Count() == 0) {
        return 0;
    }

    int32_t v1 = -1;

    uint32_t i;
    for (i = 0; i < CRealmList::s_categories.Count(); i++) {
        auto realmCategory = CRealmList::s_categories[i];

        if (realmCategory && realmCategory->uint14 > 0) {
            v1++;

            if (v1 == a1) {
                break;
            }
        }
    }

    return i;
}

void CRealmList::UpdateList() {
    CRealmList::s_avgLoad = 0.0f;
    int32_t category = -1;
    auto realmCount = ClientServices::GetInstance()->m_realmList.Count();

    for (int32_t realmIndex = 0; realmIndex < realmCount; realmIndex++) {
        auto realmInfo = ClientServices::GetRealmInfoByIndex(realmIndex);
        if (!realmInfo) {
            continue;
        }

        auto selectedRealmName = ClientServices::GetSelectedRealmName();

        if (!SStrCmpI(selectedRealmName, realmInfo->name, STORM_MAX_STR) /* TODO SStrCmpUTF8I */) {
            if (CRealmList::s_preferredCategory < 0) {
                CRealmList::s_preferredCategory = realmInfo->category;
            }

            category = realmInfo->category;
        }

        CRealmList::s_avgLoad += realmInfo->population;

        for (int32_t categoryIndex = 0; categoryIndex < CRealmList::s_categories.Count(); categoryIndex++) {
            auto realmCategory = CRealmList::s_categories[categoryIndex];

            if (realmInfo->category != realmCategory->m_category->GetID()) {
                continue;
            }

            if (realmCategory->m_realms.Count() == realmCategory->uint14) {
                realmCategory->m_realms.SetCount(realmCategory->uint14 + 1);
            }

            realmCategory->m_realms[realmCategory->uint14] = realmIndex;
            realmCategory->uint14++;
        }
    }

    if (realmCount <= 1) {
        CRealmList::s_avgLoad = 1.0f;
    } else {
        // TODO
    }

    // TODO CRealmList::m_stdDevLoad

    // TODO sort realm list

    for (int32_t i = 0; i < CRealmList::s_categories.Count(); i++) {
        if (CRealmList::s_categories[i]->m_category->GetID() == category) {
            CRealmList::s_selectedCategory = i;
        }
    }

    if (CRealmList::s_preferredCategory != -1 /* TODO || ClientServices::Connection()->uint2F10 */) {
        FrameScript_SignalEvent(9, nullptr);
    } else {
        uint32_t nonEmptyCategories = 0;

        for (uint32_t i = 0; i < CRealmList::s_categories.Count(); i++) {
            auto realmCategory = CRealmList::s_categories[i];

            if (realmCategory && realmCategory->uint14 != 0) {
                nonEmptyCategories++;
            }
        }

        FrameScript_SignalEvent(10, "%d", nonEmptyCategories);
    }
}
