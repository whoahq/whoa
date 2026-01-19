#ifndef OBJECT_CLIENT_CG_CONTAINER_C_HPP
#define OBJECT_CLIENT_CG_CONTAINER_C_HPP

#include "object/client/CClientObjCreate.hpp"
#include "object/client/CGContainer.hpp"
#include "object/client/CGItem_C.hpp"

class CGContainer_C : public CGItem_C, public CGContainer {
    public:
        // Virtual public member functions
        virtual ~CGContainer_C();

        // Public member functions
        CGContainer_C(uint32_t time, CClientObjCreate& objCreate);
        void SetStorage(uint32_t* storage, uint32_t* saved);
};

#endif
