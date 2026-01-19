#ifndef OBJECT_CLIENT_CG_CORPSE_C_HPP
#define OBJECT_CLIENT_CG_CORPSE_C_HPP

#include "object/client/CClientObjCreate.hpp"
#include "object/client/CGCorpse.hpp"
#include "object/client/CGObject_C.hpp"

class CGCorpse_C : public CGObject_C, public CGCorpse {
    public:
        // Virtual public member functions
        virtual ~CGCorpse_C();

        // Public member functions
        CGCorpse_C(uint32_t time, CClientObjCreate& objCreate);
        void SetStorage(uint32_t* storage, uint32_t* saved);
};

#endif
