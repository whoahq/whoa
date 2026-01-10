#ifndef OBJECT_CLIENT_CG_OBJECT_HPP
#define OBJECT_CLIENT_CG_OBJECT_HPP

#include "object/Types.hpp"
#include <cstdint>

struct CGObjectData {
    // TODO
};

class CGObject {
    public:
        // Public static functions
        static uint32_t TotalFieldsSaved();

        // Public member variables
        CGObjectData* m_obj;
        uint32_t* m_objSaved;
        uint32_t m_memHandle;
        OBJECT_TYPE_ID m_typeID;
};

#endif
