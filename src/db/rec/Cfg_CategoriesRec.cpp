#include "db/rec/Cfg_CategoriesRec.hpp"
#include "util/SFile.hpp"

const char* Cfg_CategoriesRec::GetFilename() {
    return "DBFilesClient\\Cfg_Categories.dbc";
}

uint32_t Cfg_CategoriesRec::GetNumColumns() {
    return 21;
}

uint32_t Cfg_CategoriesRec::GetRowSize() {
    return 84;
}

int32_t Cfg_CategoriesRec::GetID() {
    return this->m_ID;
}

bool Cfg_CategoriesRec::Read(SFile* f, const char* stringBuffer) {
    uint32_t nameOfs[16];
    uint32_t nameMask;

    if (
        !SFile::Read(f, &this->m_ID, sizeof(this->m_ID), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_localeMask, sizeof(this->m_localeMask), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_createCharsetMask, sizeof(this->m_createCharsetMask), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_flags, sizeof(this->m_flags), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[0], sizeof(nameOfs[0]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[1], sizeof(nameOfs[1]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[2], sizeof(nameOfs[2]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[3], sizeof(nameOfs[3]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[4], sizeof(nameOfs[4]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[5], sizeof(nameOfs[5]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[6], sizeof(nameOfs[6]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[7], sizeof(nameOfs[7]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[8], sizeof(nameOfs[8]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[9], sizeof(nameOfs[9]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[10], sizeof(nameOfs[10]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[11], sizeof(nameOfs[11]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[12], sizeof(nameOfs[12]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[13], sizeof(nameOfs[13]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[14], sizeof(nameOfs[14]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameOfs[15], sizeof(nameOfs[15]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &nameMask, sizeof(nameMask), nullptr, nullptr, nullptr)
    ) {
        return false;
    }

    if (stringBuffer) {
        this->m_name = &stringBuffer[nameOfs[0]];
    } else {
        this->m_name = "";
    }

    return true;
}
