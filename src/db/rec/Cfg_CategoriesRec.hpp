#ifndef DB_REC_CFG_CATEGORIES_REC_HPP
#define DB_REC_CFG_CATEGORIES_REC_HPP

#include <cstdint>

class SFile;

class Cfg_CategoriesRec {
    public:
    static constexpr uint32_t columnCount = 21;
    static constexpr uint32_t rowSize = 84;

    uint32_t m_ID;
    uint32_t m_localeMask;
    uint32_t m_createCharsetMask;
    uint32_t m_flags;
    const char *m_name;

    static const char* GetFilename();
    bool Read(SFile* f, const char* stringBuffer);
};

#endif
