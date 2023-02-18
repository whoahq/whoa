#include "db/rec/Cfg_ConfigsRec.hpp"
#include "util/SFile.hpp"

const char* Cfg_ConfigsRec::GetFilename() {
    return "DBFilesClient\\Cfg_Configs.dbc";
}

uint32_t Cfg_ConfigsRec::GetNumColumns() {
    return 4;
}

uint32_t Cfg_ConfigsRec::GetRowSize() {
    return 16;
}

int32_t Cfg_ConfigsRec::GetID() {
    return this->m_ID;
}

bool Cfg_ConfigsRec::Read(SFile* f, const char* stringBuffer) {
    if (
        !SFile::Read(f, &this->m_ID, sizeof(this->m_ID), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_realmType, sizeof(this->m_realmType), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_playerKillingAllowed, sizeof(this->m_playerKillingAllowed), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_roleplaying, sizeof(this->m_roleplaying), nullptr, nullptr, nullptr)
    ) {
        return false;
    }

    return true;
}
