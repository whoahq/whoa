#ifndef DB_I_DATABASE_HPP
#define DB_I_DATABASE_HPP

template <class T>
class IDatabase {
    public:
        // Member variables
        T* m_records = nullptr;
        T** m_recordsById = nullptr;

        // Virtual member functions
        virtual T* GetRecord(int32_t id) = 0;
};

#endif
