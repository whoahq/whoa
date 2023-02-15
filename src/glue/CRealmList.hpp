#ifndef GLUE_C_REALM_LIST_HPP
#define GLUE_C_REALM_LIST_HPP

#include <cstdint>
#include <storm/Array.hpp>

class Cfg_CategoriesRec;

struct RealmCategory {
    Cfg_CategoriesRec* m_category;
    TSGrowableArray<uint32_t> m_realms;
    uint32_t uint14;
};

class CRealmList {
    public:
        // Static variables
        static TSFixedArray<RealmCategory*> s_categories;

        // Static functions
        static void Initialize();
        static void UpdateList();
};

#endif
