#ifndef OBJECT_CLIENT_CG_DYNAMIC_OBJECT_HPP
#define OBJECT_CLIENT_CG_DYNAMIC_OBJECT_HPP

#include <cstdint>

struct CGDynamicObjectData {
    // TODO
};

class CGDynamicObject {
    public:
        // Public static functions
        static uint32_t GetBaseOffset();
        static uint32_t GetBaseOffsetSaved();
        static uint32_t GetDataSize();
        static uint32_t GetDataSizeSaved();
        static uint32_t TotalFields();
        static uint32_t TotalFieldsSaved();

        // Public member variables
        CGDynamicObjectData* m_dynamicObj;
        uint32_t* m_dynamicObjSaved;
};

#endif
