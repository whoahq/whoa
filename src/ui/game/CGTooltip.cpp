#include "ui/game/CGTooltip.hpp"

CSimpleFrame* CGTooltip::Create(CSimpleFrame* parent) {
    // TODO use CDataAllocator

    return STORM_NEW(CGTooltip)(parent);
}

CGTooltip::CGTooltip(CSimpleFrame* parent) : CSimpleFrame(parent) {
    // TODO
}
