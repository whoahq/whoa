#include "db/rec/AchievementRec.hpp"
#include "util/SFile.hpp"

const char* AchievementRec::GetFilename() {
    return "DBFilesClient\\Achievement.dbc";
}

uint32_t AchievementRec::GetNumColumns() {
    return 62;
}

uint32_t AchievementRec::GetRowSize() {
    return 248;
}

int32_t AchievementRec::GetID() {
    return this->m_ID;
}

bool AchievementRec::Read(SFile* f, const char* stringBuffer) {
    uint32_t titleOfs[16];
    uint32_t titleMask;
    uint32_t descriptionOfs[16];
    uint32_t descriptionMask;
    uint32_t rewardOfs[16];
    uint32_t rewardMask;

    if (
        !SFile::Read(f, &this->m_ID, sizeof(this->m_ID), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_faction, sizeof(this->m_faction), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_instanceID, sizeof(this->m_instanceID), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_supercedes, sizeof(this->m_supercedes), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[0], sizeof(titleOfs[0]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[1], sizeof(titleOfs[1]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[2], sizeof(titleOfs[2]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[3], sizeof(titleOfs[3]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[4], sizeof(titleOfs[4]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[5], sizeof(titleOfs[5]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[6], sizeof(titleOfs[6]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[7], sizeof(titleOfs[7]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[8], sizeof(titleOfs[8]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[9], sizeof(titleOfs[9]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[10], sizeof(titleOfs[10]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[11], sizeof(titleOfs[11]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[12], sizeof(titleOfs[12]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[13], sizeof(titleOfs[13]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[14], sizeof(titleOfs[14]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleOfs[15], sizeof(titleOfs[15]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &titleMask, sizeof(titleMask), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[0], sizeof(descriptionOfs[0]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[1], sizeof(descriptionOfs[1]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[2], sizeof(descriptionOfs[2]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[3], sizeof(descriptionOfs[3]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[4], sizeof(descriptionOfs[4]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[5], sizeof(descriptionOfs[5]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[6], sizeof(descriptionOfs[6]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[7], sizeof(descriptionOfs[7]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[8], sizeof(descriptionOfs[8]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[9], sizeof(descriptionOfs[9]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[10], sizeof(descriptionOfs[10]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[11], sizeof(descriptionOfs[11]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[12], sizeof(descriptionOfs[12]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[13], sizeof(descriptionOfs[13]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[14], sizeof(descriptionOfs[14]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionOfs[15], sizeof(descriptionOfs[15]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &descriptionMask, sizeof(descriptionMask), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_category, sizeof(this->m_category), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_points, sizeof(this->m_points), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_uiOrder, sizeof(this->m_uiOrder), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_flags, sizeof(this->m_flags), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_iconID, sizeof(this->m_iconID), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[0], sizeof(rewardOfs[0]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[1], sizeof(rewardOfs[1]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[2], sizeof(rewardOfs[2]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[3], sizeof(rewardOfs[3]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[4], sizeof(rewardOfs[4]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[5], sizeof(rewardOfs[5]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[6], sizeof(rewardOfs[6]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[7], sizeof(rewardOfs[7]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[8], sizeof(rewardOfs[8]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[9], sizeof(rewardOfs[9]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[10], sizeof(rewardOfs[10]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[11], sizeof(rewardOfs[11]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[12], sizeof(rewardOfs[12]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[13], sizeof(rewardOfs[13]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[14], sizeof(rewardOfs[14]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardOfs[15], sizeof(rewardOfs[15]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &rewardMask, sizeof(rewardMask), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_minimumCriteria, sizeof(this->m_minimumCriteria), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_sharesCriteria, sizeof(this->m_sharesCriteria), nullptr, nullptr, nullptr)
    ) {
        return false;
    }

    if (stringBuffer) {
        this->m_title = &stringBuffer[titleOfs[0]];
        this->m_description = &stringBuffer[descriptionOfs[0]];
        this->m_reward = &stringBuffer[rewardOfs[0]];
    } else {
        this->m_title = "";
        this->m_description = "";
        this->m_reward = "";
    }

    return true;
}
