#ifndef OBJECT_CLIENT_PLAYER_C_HPP
#define OBJECT_CLIENT_PLAYER_C_HPP

#include <cstdint>

class CreatureModelDataRec;

uint32_t Player_C_GetDisplayId(uint32_t race, uint32_t sex);

const CreatureModelDataRec* Player_C_GetModelName(uint32_t race, uint32_t sex);

#endif
