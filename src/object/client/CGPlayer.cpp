#include "object/client/CGPlayer.hpp"
#include "object/client/CGUnit.hpp"

uint32_t CGPlayer::TotalFieldsSaved() {
    return CGUnit::TotalFieldsSaved() + 173;
}
