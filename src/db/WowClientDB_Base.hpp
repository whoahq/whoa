#ifndef DB_WOW_CLIENT_DB_BASE_HPP
#define DB_WOW_CLIENT_DB_BASE_HPP

#include "util/SFile.hpp"
#include <cstdint>

class WowClientDB_Base {
    public:
        // Member variables
        int32_t m_loaded = 0;
        int32_t m_numRecords = 0;
        int32_t m_maxID = -1;
        int32_t m_minID = 0xFFFFFFF;
        const char* m_strings = nullptr;

        // Virtual member functions
        virtual void Load(const char* filename, int32_t linenumber) = 0;
        virtual void LoadRecords(SFile* f, const char* filename, int32_t linenumber) = 0;
        virtual int32_t GetRecordByIndex(int32_t index, void* ptr) const = 0;
};

#endif
