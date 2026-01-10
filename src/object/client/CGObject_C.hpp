#ifndef OBJECT_CLIENT_CG_OBJECT_C_HPP
#define OBJECT_CLIENT_CG_OBJECT_C_HPP

#include "object/client/CGObject.hpp"
#include "object/Types.hpp"
#include "util/CHashKeyGUID.hpp"
#include <storm/Hash.hpp>

class CGObject_C : public CGObject, public TSHashObject<CGObject_C, CHashKeyGUID> {
    public:
        // Public member functions
        void SetTypeID(OBJECT_TYPE_ID typeID);
};

#endif
