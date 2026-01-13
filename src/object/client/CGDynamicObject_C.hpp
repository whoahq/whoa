#ifndef OBJECT_CLIENT_CG_DYNAMIC_OBJECT_C_HPP
#define OBJECT_CLIENT_CG_DYNAMIC_OBJECT_C_HPP

#include "object/client/CClientObjCreate.hpp"
#include "object/client/CGDynamicObject.hpp"
#include "object/client/CGObject_C.hpp"

class CGDynamicObject_C : public CGObject_C, public CGDynamicObject {
    public:
        // Public member functions
        CGDynamicObject_C(uint32_t time, CClientObjCreate& objCreate);
        void SetStorage(uint32_t* storage, uint32_t* saved);
};

#endif
