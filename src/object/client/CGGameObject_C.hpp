#ifndef OBJECT_CLIENT_CG_GAME_OBJECT_C_HPP
#define OBJECT_CLIENT_CG_GAME_OBJECT_C_HPP

#include "object/client/CClientObjCreate.hpp"
#include "object/client/CGGameObject.hpp"
#include "object/client/CGObject_C.hpp"

class CGGameObject_C : public CGObject_C, public CGGameObject {
    public:
        // Virtual public member functions
        virtual ~CGGameObject_C();

        // Public member functions
        CGGameObject_C(uint32_t time, CClientObjCreate& objCreate);
        void PostInit(uint32_t time, const CClientObjCreate& init, bool a4);
        void SetStorage(uint32_t* storage, uint32_t* saved);
};

#endif
