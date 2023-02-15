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

            if (realmInfo->category != realmCategory->m_category->m_ID) {
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
        if (CRealmList::s_categories[i]->m_category->m_ID == category) {
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
