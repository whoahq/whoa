#ifndef OBJECT_CLIENT_CG_OBJECT_HPP
#define OBJECT_CLIENT_CG_OBJECT_HPP

#include "object/Types.hpp"
#include "util/GUID.hpp"
#include <cstdint>

struct CGObjectData {
    WOWGUID m_guid;
    OBJECT_TYPE m_type;
    int32_t m_entryID;
    float m_scale;
    uint32_t pad;
};

class CGObject {
    public:
        // Public static functions
        static uint32_t GetBaseOffset();
        static uint32_t GetBaseOffsetSaved();
        static uint32_t GetDataSize();
        static uint32_t GetDataSizeSaved();
        static uint32_t TotalFields();
        static uint32_t TotalFieldsSaved();

        // Public member variables
        uint32_t uint0; // TODO what is this?
        uint32_t m_memHandle;

        // Public member functions
        WOWGUID GetGUID() const;
        OBJECT_TYPE GetType() const;
        OBJECT_TYPE_ID GetTypeID() const;
        int32_t IsA(OBJECT_TYPE type) const;

    protected:
        // Protected member variables
        CGObjectData* m_obj;
        uint32_t* m_objSaved;
        OBJECT_TYPE_ID m_typeID;

        // Protected member functions
        CGObjectData* Obj() const;
};

#endif
