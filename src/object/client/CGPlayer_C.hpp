#ifndef OBJECT_CLIENT_CG_PLAYER_C_HPP
#define OBJECT_CLIENT_CG_PLAYER_C_HPP

#include "object/client/CClientObjCreate.hpp"
#include "object/client/CGPlayer.hpp"
#include "object/client/CGUnit_C.hpp"
#include <cstdint>

class CreatureModelDataRec;

class CGPlayer_C : public CGUnit_C, public CGPlayer {
    public:
        // Public member functions
        CGPlayer_C(uint32_t time, CClientObjCreate& objCreate);
        void SetStorage(uint32_t* storage, uint32_t* saved);
};

uint32_t Player_C_GetDisplayId(uint32_t race, uint32_t sex);

const CreatureModelDataRec* Player_C_GetModelName(uint32_t race, uint32_t sex);

#endif
