#include "object/client/CGPlayer.hpp"
#include "object/client/CGUnit.hpp"

uint32_t CGPlayer::GetBaseOffset() {
    return CGUnit::TotalFields();
}

uint32_t CGPlayer::GetBaseOffsetSaved() {
    return CGUnit::TotalFieldsSaved();
}

uint32_t CGPlayer::GetDataSize() {
    return CGPlayer::TotalFields() * sizeof(uint32_t);
}

uint32_t CGPlayer::GetDataSizeSaved() {
    return CGPlayer::TotalFieldsSaved() * sizeof(uint32_t);
}

uint32_t CGPlayer::GetRemoteDataSize() {
    return CGPlayer::TotalRemoteFields() * sizeof(uint32_t);
}

uint32_t CGPlayer::GetRemoteDataSizeSaved() {
    return CGPlayer::TotalRemoteFieldsSaved() * sizeof(uint32_t);
}

uint32_t CGPlayer::TotalFields() {
    return CGPlayer::GetBaseOffset() + 1178;
}

uint32_t CGPlayer::TotalRemoteFields() {
    return CGPlayer::GetBaseOffset() + 176;
}

uint32_t CGPlayer::TotalFieldsSaved() {
    return CGPlayer::GetBaseOffsetSaved() + 1043;
}

uint32_t CGPlayer::TotalRemoteFieldsSaved() {
    return CGPlayer::GetBaseOffsetSaved() + 173;
}

uint32_t CGPlayer::GetMoney() const {
    return this->Player()->coinage;
}

uint32_t CGPlayer::GetNextLevelXP() const {
    return this->Player()->nextLevelXP;
}

uint32_t CGPlayer::GetXP() const {
    return this->Player()->xp;
}

CGPlayerData* CGPlayer::Player() const {
    return this->m_player;
}
