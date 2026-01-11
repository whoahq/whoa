#ifndef OBJECT_CLIENT_CG_ITEM_C_HPP
#define OBJECT_CLIENT_CG_ITEM_C_HPP

#include "object/client/CGObject_C.hpp"
#include "object/client/CGItem.hpp"

class CGItem_C : public CGObject_C, public CGItem {
    public:
        // Public member functions
        void SetStorage(uint32_t* storage, uint32_t* saved);
};

#endif
