#include "db/rec/MapRec.hpp"
#include "util/SFile.hpp"

const char* MapRec::GetFilename() {
    return "DBFilesClient\\Map.dbc";
}

uint32_t MapRec::GetNumColumns() {
    return 66;
}

uint32_t MapRec::GetRowSize() {
    return 264;
}

bool MapRec::NeedIDAssigned() {
    return false;
}

int32_t MapRec::GetID() {
    return this->m_ID;
}

void MapRec::SetID(int32_t id) {
    this->m_ID = id;
}

bool MapRec::Read(SFile* f, const char* stringBuffer) {
    uint32_t directoryOfs;
    uint32_t mapNameOfs[16];
    uint32_t mapNameMask;
    uint32_t mapDescription0Ofs[16];
    uint32_t mapDescription0Mask;
    uint32_t mapDescription1Ofs[16];
    uint32_t mapDescription1Mask;

    if (
        !SFile::Read(f, &this->m_ID, sizeof(this->m_ID), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &directoryOfs, sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_instanceType, sizeof(this->m_instanceType), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_flags, sizeof(this->m_flags), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_pvp, sizeof(this->m_pvp), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[0], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[1], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[2], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[3], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[4], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[5], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[6], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[7], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[8], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[9], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[10], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[11], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[12], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[13], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[14], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameOfs[15], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapNameMask, sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_areaTableID, sizeof(this->m_areaTableID), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[0], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[1], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[2], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[3], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[4], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[5], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[6], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[7], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[8], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[9], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[10], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[11], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[12], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[13], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[14], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Ofs[15], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription0Mask, sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[0], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[1], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[2], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[3], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[4], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[5], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[6], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[7], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[8], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[9], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[10], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[11], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[12], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[13], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[14], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Ofs[15], sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &mapDescription1Mask, sizeof(uint32_t), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_loadingScreenID, sizeof(this->m_loadingScreenID), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_minimapIconScale, sizeof(this->m_minimapIconScale), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_corpseMapID, sizeof(this->m_corpseMapID), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_corpse[0], sizeof(m_corpse[0]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_corpse[1], sizeof(m_corpse[0]), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_timeOfDayOverride, sizeof(this->m_timeOfDayOverride), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_expansionID, sizeof(this->m_expansionID), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_raidOffset, sizeof(this->m_raidOffset), nullptr, nullptr, nullptr)
        || !SFile::Read(f, &this->m_maxPlayers, sizeof(this->m_maxPlayers), nullptr, nullptr, nullptr)
    ) {
        return false;
    }

    if (stringBuffer) {
        this->m_directory = &stringBuffer[directoryOfs];
        this->m_mapName = &stringBuffer[mapNameOfs[0]];
        this->m_mapDescription0 = &stringBuffer[mapDescription0Ofs[0]];
        this->m_mapDescription1 = &stringBuffer[mapDescription1Ofs[0]];
    } else {
        this->m_directory = "";
        this->m_mapName = "";
        this->m_mapDescription0 = "";
        this->m_mapDescription1 = "";
    }

    return true;
}
