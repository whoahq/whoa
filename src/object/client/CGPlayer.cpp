#include "object/client/CGPlayer.hpp"
#include "object/client/CGUnit.hpp"

uint32_t CGPlayer::TotalFields() {
    return CGUnit::TotalFields() + 1178;
}

uint32_t CGPlayer::TotalRemoteFields() {
    return CGUnit::TotalFields() + 176;
}

uint32_t CGPlayer::TotalFieldsSaved() {
    return CGUnit::TotalFieldsSaved() + 1043;
}

uint32_t CGPlayer::TotalRemoteFieldsSaved() {
    return CGUnit::TotalFieldsSaved() + 173;
}
