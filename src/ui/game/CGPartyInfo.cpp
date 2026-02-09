#include "ui/game/CGPartyInfo.hpp"

WOWGUID CGPartyInfo::m_members[4];

uint32_t CGPartyInfo::NumMembers() {
    uint32_t count = 0;

    for (auto& member : CGPartyInfo::m_members) {
        if (member != 0) {
            count++;
        }
    }

    return count;
}
