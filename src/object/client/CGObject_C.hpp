#ifndef OBJECT_CLIENT_CG_OBJECT_C_HPP
#define OBJECT_CLIENT_CG_OBJECT_C_HPP

#include "object/Types.hpp"
#include "object/client/CGObject.hpp"
#include "util/GUID.hpp"
#include <storm/Hash.hpp>

class CGObject_C : public CGObject, public TSHashObject<CGObject_C, CHashKeyGUID> {
    public:
        // Public member functions
        void SetBlock(uint32_t block, uint32_t value);
        void SetStorage(uint32_t* storage, uint32_t* saved);
        void SetTypeID(OBJECT_TYPE_ID typeID);
};

#endif
