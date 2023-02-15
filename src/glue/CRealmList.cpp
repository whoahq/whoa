#include "glue/CRealmList.hpp"
#include "db/Db.hpp"
#include <storm/Memory.hpp>

TSFixedArray<RealmCategory*> CRealmList::s_categories;

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
    // TODO
}
