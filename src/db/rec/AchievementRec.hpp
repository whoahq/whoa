#ifndef DB_REC_ACHIEVEMENT_REC_HPP
#define DB_REC_ACHIEVEMENT_REC_HPP

#include <cstdint>

class SFile;

class AchievementRec {
    public:
        static constexpr uint32_t columnCount = 62;
        static constexpr uint32_t rowSize = 248;

        int32_t m_ID;
        int32_t m_faction;
        int32_t m_instanceID;
        int32_t m_supercedes;
        const char* m_title;
        const char* m_description;
        int32_t m_category;
        int32_t m_points;
        int32_t m_uiOrder;
        int32_t m_flags;
        uint32_t m_iconID;
        const char* m_reward;
        int32_t m_minimumCriteria;
        int32_t m_sharesCriteria;

        static const char* GetFilename();
        bool Read(SFile* f, const char* stringBuffer);
};

#endif
