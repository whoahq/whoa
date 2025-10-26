#ifndef DB_WOW_CLIENT_DB_HPP
#define DB_WOW_CLIENT_DB_HPP

#include "db/IDatabase.hpp"
#include "db/WowClientDB_Common.hpp"
#include <cstring>
#include <storm/Error.hpp>
#include <storm/Memory.hpp>

template <class T>
class WowClientDB : public WowClientDB_Common<T>, IDatabase<T> {
    public:
        // Virtual member functions
        virtual void Load(const char* filename, int32_t linenumber);
        virtual void LoadRecords(SFile* f, const char* filename, int32_t linenumber);
        virtual int32_t GetRecordByIndex(int32_t index, void* ptr) const;
        virtual T* GetRecord(int32_t id);

        // Member functions
        T* GetRecordByIndex(int32_t index) const;
        int32_t GetNumRecords();
};

template <class T>
int32_t WowClientDB<T>::GetNumRecords() {
    return this->m_numRecords;
}

template <class T>
T* WowClientDB<T>::GetRecordByIndex(int32_t index) const {
        STORM_ASSERT(this->m_numRecords >= 0);

        if (index < 0 || index >= this->m_numRecords) {
            return nullptr;
        }

        return &this->m_records[index];
}

template <class T>
int32_t WowClientDB<T>::GetRecordByIndex(int32_t index, void* ptr) const {
    STORM_ASSERT(this->m_numRecords >= 0);

    if (index < 0 || index >= this->m_numRecords) {
        return 0;
    }

    memcpy(ptr, &this->m_records[index], sizeof(T));

    return 1;
}

template <class T>
T* WowClientDB<T>::GetRecord(int32_t id) {
    STORM_ASSERT(this->m_numRecords >= 0);

    if (id < this->m_minID || id > this->m_maxID) {
        return nullptr;
    }

    return this->m_recordsById[id - this->m_minID];
}

template <class T>
void WowClientDB<T>::Load(const char* filename, int32_t linenumber) {
    if (this->m_loaded) {
        // TODO
        // SErrDisplayAppFatalCustom(0x85100079, "%s already loaded! Aborting to prevent memory leak!", T::GetFilename());
        return;
    }

    SFile* f;
    if (!SFile::OpenEx(nullptr, T::GetFilename(), 0x20000, &f)) {
        // TODO
        // SErrDisplayAppFatalCustom(0x85100079, "Unable to open %s", T::GetFilename());
        return;
    }

    uint32_t signature;
    if (!SFile::Read(f, &signature, sizeof(signature), nullptr, nullptr, nullptr)) {
        // TODO
        // SErrDisplayAppFatalCustom(0x85100079, "Unable to read signature from %s", T::GetFilename());
        return;
    }

    if (signature != 'CBDW') {
        // TODO
        // SErrDisplayAppFatalCustom(0x85100079, "Invalid signature 0x%x from %s", signature, T::GetFilename());
        return;
    }

    if (!SFile::Read(f, &this->m_numRecords, sizeof(this->m_numRecords), nullptr, nullptr, nullptr)) {
        // TODO
        // SErrDisplayAppFatalCustom(0x85100079, "Unable to read record count from %s", T::GetFilename());
        return;
    }

    if (!this->m_numRecords) {
        SFile::Close(f);
        return;
    }

    uint32_t columnCount;
    if (!SFile::Read(f, &columnCount, sizeof(columnCount), nullptr, nullptr, nullptr)) {
        // TODO
        // SErrDisplayAppFatalCustom(0x85100079, "Unable to read column count from %s", T::GetFilename());
        return;
    }

    if (columnCount != T::GetNumColumns()) {
        // TODO
        // SErrDisplayAppFatalCustom(0x85100079, "%s has wrong number of columns (found %i, expected %i)", T::GetFilename(), columnCount, T::GetNumColumns());
        return;
    }

    uint32_t rowSize;
    if (!SFile::Read(f, &rowSize, sizeof(rowSize), nullptr, nullptr, nullptr)) {
        // TODO
        // SErrDisplayAppFatalCustom(0x85100079, "Unable to read row size from %s", T::GetFilename());
        return;
    }

    if (rowSize != T::GetRowSize()) {
        // TODO
        // SErrDisplayAppFatalCustom(0x85100079, "%s has wrong row size (found %i, expected %i)", T::GetFilename(), rowSize, T::GetRowSize());
        return;
    }

    uint32_t stringSize;
    if (!SFile::Read(f, &stringSize, sizeof(stringSize), nullptr, nullptr, nullptr)) {
        // TODO
        // SErrDisplayAppFatalCustom(0x85100079, "Unable to read string size from %s", T::GetFilename());
        return;
    }

    auto stringBuffer = SMemAlloc(stringSize, filename, linenumber, 0x0);
    this->m_strings = static_cast<const char*>(stringBuffer);

    this->m_maxID = 0;
    this->m_minID = 0xFFFFFFF;

    this->LoadRecords(f, filename, linenumber);

    if (!SFile::Read(f, const_cast<char*>(this->m_strings), stringSize, nullptr, nullptr, nullptr)) {
        SErrDisplayAppFatal("%s: Cannot read string table", T::GetFilename());
    }

    SFile::Close(f);

    this->m_loaded = 1;
}

template <class T>
void WowClientDB<T>::LoadRecords(SFile* f, const char* filename, int32_t linenumber) {
    auto records = SMemAlloc(sizeof(T) * this->m_numRecords, filename, linenumber, 0x0);
    this->m_records = static_cast<T*>(records);

    for (uint32_t i = 0; i < this->m_numRecords; i++) {
        auto record = &this->m_records[i];
        record->Read(f, this->m_strings);

        if (T::NeedIDAssigned()) {
            record->SetID(i);
        }

        this->m_maxID = record->GetID() > this->m_maxID ? record->GetID() : this->m_maxID;
        this->m_minID = record->GetID() < this->m_minID ? record->GetID() : this->m_minID;
    }

    auto recordsById = SMemAlloc(sizeof(void*) * (this->m_maxID - this->m_minID + 1), __FILE__, __LINE__, 0x0);
    memset(recordsById, 0, sizeof(void*) * (this->m_maxID - this->m_minID + 1));
    this->m_recordsById = static_cast<T**>(recordsById);

    for (uint32_t i = 0; i < this->m_numRecords; i++) {
        auto record = &this->m_records[i];
        auto id = record->GetID() - this->m_minID;
        this->m_recordsById[id] = record;
    }
}

#endif
