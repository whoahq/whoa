#ifndef DB_REC_CFG_CATEGORIES_REC_HPP
#define DB_REC_CFG_CATEGORIES_REC_HPP

#include <cstdint>

class SFile;

class Cfg_CategoriesRec {
    public:
        int32_t m_ID;
        int32_t m_localeMask;
        int32_t m_createCharsetMask;
        int32_t m_flags;
        const char *m_name;

        static const char* GetFilename();
        static uint32_t GetNumColumns();
        static uint32_t GetRowSize();

        int32_t GetID();
        bool Read(SFile* f, const char* stringBuffer);
};

#endif
