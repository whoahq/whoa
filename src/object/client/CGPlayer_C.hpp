#ifndef OBJECT_CLIENT_CG_PLAYER_C_HPP
#define OBJECT_CLIENT_CG_PLAYER_C_HPP

#include "object/client/CClientObjCreate.hpp"
#include "object/client/CGPlayer.hpp"
#include "object/client/CGUnit_C.hpp"
#include <cstdint>

class CreatureModelDataRec;

class CGPlayer_C : public CGUnit_C, public CGPlayer {
    public:
        // Virtual public member functions
        virtual ~CGPlayer_C();

        // Public member functions
        CGPlayer_C(uint32_t time, CClientObjCreate& objCreate);
        uint32_t GetNextLevelXP() const;
        uint32_t GetXP() const;
        void PostInit(uint32_t time, const CClientObjCreate& init, bool a4);
        void PostInitActivePlayer();
        void SetStorage(uint32_t* storage, uint32_t* saved);
        void UpdatePartyMemberState();
};

uint32_t Player_C_GetDisplayId(uint32_t race, uint32_t sex);

const CreatureModelDataRec* Player_C_GetModelName(uint32_t race, uint32_t sex);

#endif
