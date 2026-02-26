#ifndef OBJECT_CLIENT_CG_CONTAINER_HPP
#define OBJECT_CLIENT_CG_CONTAINER_HPP

#include "util/GUID.hpp"
#include <cstdint>

struct CGContainerData {
    uint32_t numSlots;
    uint32_t pad;
    WOWGUID slots[36];
};

class CGContainer {
    public:
        // Public static functions
        static uint32_t GetBaseOffset();
        static uint32_t GetBaseOffsetSaved();
        static uint32_t GetDataSize();
        static uint32_t GetDataSizeSaved();
        static uint32_t TotalFields();
        static uint32_t TotalFieldsSaved();

    protected:
        // Protected member variables
        CGContainerData* m_cont;
        uint32_t* m_contSaved;

        // Protected member functions
        CGContainerData* Container() const;
};

#endif
