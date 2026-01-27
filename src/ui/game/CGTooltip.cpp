#include "ui/game/CGTooltip.hpp"

int32_t CGTooltip::s_objectType;

CSimpleFrame* CGTooltip::Create(CSimpleFrame* parent) {
    // TODO use CDataAllocator

    return STORM_NEW(CGTooltip)(parent);
}

int32_t CGTooltip::GetObjectType() {
    if (!CGTooltip::s_objectType) {
        CGTooltip::s_objectType = ++FrameScript_Object::s_objectTypes;
    }

    return CGTooltip::s_objectType;
}

CGTooltip::CGTooltip(CSimpleFrame* parent) : CSimpleFrame(parent) {
    // TODO
}
