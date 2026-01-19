#ifndef OBJECT_CLIENT_CG_ITEM_C_HPP
#define OBJECT_CLIENT_CG_ITEM_C_HPP

#include "object/client/CClientObjCreate.hpp"
#include "object/client/CGObject_C.hpp"
#include "object/client/CGItem.hpp"

class CGItem_C : public CGObject_C, public CGItem {
    public:
        // Virtual public member functions
        virtual ~CGItem_C();

        // Public member functions
        CGItem_C(uint32_t time, CClientObjCreate& objCreate);
        void SetStorage(uint32_t* storage, uint32_t* saved);
};

#endif
