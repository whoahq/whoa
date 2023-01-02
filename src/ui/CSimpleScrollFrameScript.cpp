#include "ui/CSimpleScrollFrameScript.hpp"
#include "gx/Coordinate.hpp"
#include "ui/CSimpleScrollFrame.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t CSimpleScrollFrame_SetScrollChild(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleScrollFrame_GetScrollChild(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleScrollFrame_SetHorizontalScroll(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CSimpleScrollFrame_SetVerticalScroll(lua_State* L) {
    auto type = CSimpleScrollFrame::GetObjectType();
    auto scrollFrame = static_cast<CSimpleScrollFrame*>(FrameScript_GetObjectThis(L, type));

    if (!scrollFrame->ProtectedFunctionsAllowed()) {
        // TODO handle check

        return 0;
    }

    if (!lua_isnumber(L, 2)) {
        luaL_error(L, "Usage: %s:SetVerticalScroll(offset)", scrollFrame->GetDisplayName());
    }

    float offset = lua_tonumber(L, 2);
    float ndcOffset = offset / (CoordinateGetAspectCompensation() * 1024.0f);
    float ddcOffset = NDCToDDCWidth(ndcOffset);

    scrollFrame->SetVerticalScroll(ddcOffset);

    return 0;
}

int32_t CSimpleScrollFrame_GetHorizontalScroll(lua_State* L) {
    auto type = CSimpleScrollFrame::GetObjectType();
    auto scrollFrame = static_cast<CSimpleScrollFrame*>(FrameScript_GetObjectThis(L, type));

    float ddcOffset = CoordinateGetAspectCompensation() * 1024.0f * scrollFrame->m_scrollOffset.x;
    float ndcOffset = DDCToNDCWidth(ddcOffset);

    lua_pushnumber(L, ndcOffset);

    return 1;
}

int32_t CSimpleScrollFrame_GetVerticalScroll(lua_State* L) {
    auto type = CSimpleScrollFrame::GetObjectType();
    auto scrollFrame = static_cast<CSimpleScrollFrame*>(FrameScript_GetObjectThis(L, type));

    float ddcOffset = CoordinateGetAspectCompensation() * 1024.0f * scrollFrame->m_scrollOffset.y;
    float ndcOffset = DDCToNDCWidth(ddcOffset);

    lua_pushnumber(L, ndcOffset);

    return 1;
}

int32_t CSimpleScrollFrame_GetHorizontalScrollRange(lua_State* L) {
    auto type = CSimpleScrollFrame::GetObjectType();
    auto scrollFrame = static_cast<CSimpleScrollFrame*>(FrameScript_GetObjectThis(L, type));

    float ddcRange = CoordinateGetAspectCompensation() * 1024.0f * scrollFrame->m_scrollRange.x;
    float ndcRange = DDCToNDCWidth(ddcRange);

    lua_pushnumber(L, ndcRange);

    return 1;
}

int32_t CSimpleScrollFrame_GetVerticalScrollRange(lua_State* L) {
    auto type = CSimpleScrollFrame::GetObjectType();
    auto scrollFrame = static_cast<CSimpleScrollFrame*>(FrameScript_GetObjectThis(L, type));

    float ddcRange = CoordinateGetAspectCompensation() * 1024.0f * scrollFrame->m_scrollRange.y;
    float ndcRange = DDCToNDCWidth(ddcRange);

    lua_pushnumber(L, ndcRange);

    return 1;
}

int32_t CSimpleScrollFrame_UpdateScrollChildRect(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

FrameScript_Method SimpleScrollFrameMethods[NUM_SIMPLE_SCROLL_FRAME_SCRIPT_METHODS] = {
    { "SetScrollChild",             &CSimpleScrollFrame_SetScrollChild },
    { "GetScrollChild",             &CSimpleScrollFrame_GetScrollChild },
    { "SetHorizontalScroll",        &CSimpleScrollFrame_SetHorizontalScroll },
    { "SetVerticalScroll",          &CSimpleScrollFrame_SetVerticalScroll },
    { "GetHorizontalScroll",        &CSimpleScrollFrame_GetHorizontalScroll },
    { "GetVerticalScroll",          &CSimpleScrollFrame_GetVerticalScroll },
    { "GetHorizontalScrollRange",   &CSimpleScrollFrame_GetHorizontalScrollRange },
    { "GetVerticalScrollRange",     &CSimpleScrollFrame_GetVerticalScrollRange },
    { "UpdateScrollChildRect",      &CSimpleScrollFrame_UpdateScrollChildRect }
};
