#ifndef DB_REC_MAP_REC_HPP
#define DB_REC_MAP_REC_HPP

#include <cstdint>

class SFile;

class MapRec {
    public:
    int32_t m_ID;
    const char* m_directory;
    int32_t m_instanceType;
    int32_t m_flags; // unconfirmed
    int32_t m_pvp;
    const char* m_mapName;
    int32_t m_areaTableID;
    const char* m_mapDescription0;
    const char* m_mapDescription1;
    int32_t m_loadingScreenID;
    float m_minimapIconScale;
    int32_t m_corpseMapID;
    float m_corpse[2];
    int32_t m_timeOfDayOverride;
    int32_t m_expansionID;
    int32_t m_raidOffset;
    int32_t m_maxPlayers;

    static const char* GetFilename();
    static uint32_t GetNumColumns();
    static uint32_t GetRowSize();
    static bool NeedIDAssigned();
    int32_t GetID();
    void SetID(int32_t id);
    bool Read(SFile* f, const char* stringBuffer);
};

#endif
