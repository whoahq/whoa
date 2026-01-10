#include "object/client/CGPlayer.hpp"
#include "object/client/CGUnit.hpp"

uint32_t CGPlayer::TotalFieldsSaved() {
    return CGUnit::TotalFieldsSaved() + 1043;
}

uint32_t CGPlayer::TotalRemoteFieldsSaved() {
    return CGUnit::TotalFieldsSaved() + 173;
}
