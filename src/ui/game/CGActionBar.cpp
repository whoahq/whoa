#include "ui/game/CGActionBar.hpp"

uint32_t CGActionBar::s_bonusBarOffset;
uint32_t CGActionBar::s_currentPage;
uint32_t CGActionBar::s_tempPageActiveFlags;

uint32_t CGActionBar::GetBonusBarOffset() {
    return CGActionBar::s_bonusBarOffset;
}
