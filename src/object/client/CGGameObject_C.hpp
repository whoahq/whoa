#ifndef OBJECT_CLIENT_CG_GAME_OBJECT_C_HPP
#define OBJECT_CLIENT_CG_GAME_OBJECT_C_HPP

#include "object/client/CGGameObject.hpp"
#include "object/client/CGObject_C.hpp"

class CGGameObject_C : public CGObject_C, public CGGameObject {
    public:
        // Public member functions
        void SetStorage(uint32_t* storage, uint32_t* saved);
};

#endif
