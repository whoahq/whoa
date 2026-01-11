#ifndef OBJECT_CLIENT_CG_CORPSE_C_HPP
#define OBJECT_CLIENT_CG_CORPSE_C_HPP

#include "object/client/CGCorpse.hpp"
#include "object/client/CGObject_C.hpp"

class CGCorpse_C : public CGObject_C, public CGCorpse {
    public:
        // Public member functions
        void SetStorage(uint32_t* storage, uint32_t* saved);
};

#endif
