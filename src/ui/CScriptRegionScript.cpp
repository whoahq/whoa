#include "ui/CScriptRegionScript.hpp"
#include "gx/Coordinate.hpp"
#include "ui/FrameScript_Object.hpp"
#include "ui/CScriptRegion.hpp"
#include "ui/CSimpleTop.hpp"
#include "util/Lua.hpp"
#include "util/StringTo.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>
#include <tempest/Rect.hpp>

int32_t CScriptRegion_IsProtected(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_CanChangeProtectedState(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_SetParent(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_GetRect(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_GetCenter(lua_State* L) {
    int32_t type = CScriptRegion::GetObjectType();
    auto region = static_cast<CScriptRegion*>(FrameScript_GetObjectThis(L, type));

    if (region->IsResizePending()) {
        region->Resize(1);
    }

    CRect rect;

    if (region->GetRect(&rect)) {
        float width = rect.maxX - rect.minX;
        float v5 = CoordinateGetAspectCompensation() * 1024.0f * ((width * 0.5f + rect.minX) / region->m_layoutScale);
        float v6 = DDCToNDCWidth(v5);
        lua_pushnumber(L, v6);

        float height = rect.maxY - rect.minY;
        float v7 = CoordinateGetAspectCompensation() * 1024.0f * ((height * 0.5f + rect.minY) / region->m_layoutScale);
        float v8 = DDCToNDCWidth(v7);
        lua_pushnumber(L, v8);
    } else {
        lua_pushnil(L);
        lua_pushnil(L);
    }

    return 2;
}

int32_t CScriptRegion_GetLeft(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_GetRight(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_GetTop(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_GetBottom(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_GetWidth(lua_State* L) {
    int32_t type = CScriptRegion::GetObjectType();
    auto region = static_cast<CScriptRegion*>(FrameScript_GetObjectThis(L, type));

    float width = region->GetWidth();

    if (width == 0.0 && !StringToBOOL(L, 2, 0)) {
        if (region->IsResizePending()) {
            region->Resize(1);
        }

        CRect rect = { 0.0, 0.0, 0.0, 0.0 };

        if (region->GetRect(&rect)) {
            width = (rect.maxX - rect.minX) / region->m_layoutScale;
        }
    }

    float ddcWidth = CoordinateGetAspectCompensation() * 1024.0 * width;
    float ndcWidth = DDCToNDCWidth(ddcWidth);
    lua_pushnumber(L, ndcWidth);

    return 1;
}

int32_t CScriptRegion_SetWidth(lua_State* L) {
    int32_t type = CScriptRegion::GetObjectType();
    auto region = static_cast<CScriptRegion*>(FrameScript_GetObjectThis(L, type));

    if (!region->ProtectedFunctionsAllowed()) {
        // TODO
        // - disallowed logic

        return 0;
    }

    if (!lua_isnumber(L, 2)) {
        return luaL_error(L, "Usage: %s:SetWidth(width)", region->GetDisplayName());
    }

    float width = lua_tonumber(L, 2);
    float ndcWidth = width / (CoordinateGetAspectCompensation() * 1024.0f);
    float ddcWidth = NDCToDDCWidth(ndcWidth);

    region->SetWidth(ddcWidth);

    return 0;
}

int32_t CScriptRegion_GetHeight(lua_State* L) {
    int32_t type = CScriptRegion::GetObjectType();
    auto region = static_cast<CScriptRegion*>(FrameScript_GetObjectThis(L, type));

    float height = region->GetHeight();

    if (height == 0.0f && !StringToBOOL(L, 2, 0)) {
        if (region->IsResizePending()) {
            region->Resize(1);
        }

        CRect rect = { 0.0f, 0.0f, 0.0f, 0.0f };

        if (region->GetRect(&rect)) {
            height = (rect.maxY - rect.minY) / region->m_layoutScale;
        }
    }

    float ddcHeight = CoordinateGetAspectCompensation() * 1024.0f * height;
    float ndcHeight = DDCToNDCWidth(ddcHeight);
    lua_pushnumber(L, ndcHeight);

    return 1;
}

int32_t CScriptRegion_SetHeight(lua_State* L) {
    int32_t type = CScriptRegion::GetObjectType();
    auto region = static_cast<CScriptRegion*>(FrameScript_GetObjectThis(L, type));

    if (!region->ProtectedFunctionsAllowed()) {
        // TODO
        // - disallowed logic

        return 0;
    }

    if (!lua_isnumber(L, 2)) {
        return luaL_error(L, "Usage: %s:SetHeight(height)", region->GetDisplayName());
    }

    float height = lua_tonumber(L, 2);
    float ndcHeight = height / (CoordinateGetAspectCompensation() * 1024.0f);
    float ddcHeight = NDCToDDCWidth(ndcHeight);

    region->SetHeight(ddcHeight);

    return 0;
}

int32_t CScriptRegion_SetSize(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_GetSize(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_GetNumPoints(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_GetPoint(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_SetPoint(lua_State* L) {
    int32_t type = CScriptRegion::GetObjectType();
    auto region = static_cast<CScriptRegion*>(FrameScript_GetObjectThis(L, type));

    if (!region->ProtectedFunctionsAllowed()) {
        // TODO
        // - disallowed logic

        return 0;
    }

    if (!lua_isstring(L, 2)) {
        return luaL_error(L, "Usage: %s:SetPoint(\"point\" [, region or nil] [, \"relativePoint\"] [, offsetX, offsetY])", region->GetDisplayName());
    }

    auto relative = region->GetLayoutParent();

    const char* pointStr = lua_tolstring(L, 2, 0);
    FRAMEPOINT point;

    if (!StringToFramePoint(pointStr, point)) {
        return luaL_error(L, "%s:SetPoint(): Unknown region point", region->GetDisplayName());
    }

    int32_t argsIndex = 3;

    if (lua_type(L, 3) == LUA_TSTRING) {
        const char* name = lua_tolstring(L, 3, 0);
        relative = region->GetLayoutFrameByName(name);

        argsIndex++;
    } else if (lua_type(L, 3) == LUA_TTABLE) {
        lua_rawgeti(L, 3, 0);

        auto r = reinterpret_cast<CScriptRegion*>(lua_touserdata(L, -1));
        relative = r ? static_cast<CLayoutFrame*>(r) : nullptr;

        lua_settop(L, -2);

        argsIndex++;
    } else if (lua_type(L, 3) == LUA_TNIL) {
        relative = CSimpleTop::s_instance;

        argsIndex++;
    }

    if (!relative) {
        const char* name = lua_tolstring(L, 3, 0);
        return luaL_error(L, "%s:SetPoint(): Couldn't find region named '%s'", region->GetDisplayName(), name);
    }

    if (relative == region) {
        return luaL_error(L, "%s:SetPoint(): trying to anchor to itself", region->GetDisplayName());
    }

    if (relative->IsResizeDependency(region)) {
        return luaL_error(L, "%s:SetPoint(): %s is dependent on this", region->GetDisplayName(), static_cast<CScriptRegion*>(relative)->GetDisplayName());
    }

    FRAMEPOINT relativePoint = point;

    if (lua_type(L, argsIndex) == LUA_TSTRING) {
        const char* relativePointStr = lua_tolstring(L, argsIndex, 0);

        if (!StringToFramePoint(relativePointStr, relativePoint)) {
            return luaL_error(L, "%s:SetPoint(): Unknown region point", region->GetDisplayName());
        }

        argsIndex++;
    }

    float offsetX = 0.0f;
    float offsetY = 0.0f;

    if (lua_isnumber(L, argsIndex) && lua_isnumber(L, argsIndex + 1)) {
        float x = lua_tonumber(L, argsIndex);
        float ndcX = x / (CoordinateGetAspectCompensation() * 1024.0f);
        float ddcX = NDCToDDCWidth(ndcX);

        float y = lua_tonumber(L, argsIndex + 1);
        float ndcY = y / (CoordinateGetAspectCompensation() * 1024.0f);
        float ddcY = NDCToDDCWidth(ndcY);

        offsetX = ddcX;
        offsetY = ddcY;
    }

    region->SetPoint(point, relative, relativePoint, offsetX, offsetY, 1);

    return 0;
}

int32_t CScriptRegion_SetAllPoints(lua_State* L) {
    int32_t type = CScriptRegion::GetObjectType();
    auto region = static_cast<CScriptRegion*>(FrameScript_GetObjectThis(L, type));

    if (!region->ProtectedFunctionsAllowed()) {
        // TODO
        // - disallowed logic

        return 0;
    }

    auto relative = region->GetLayoutParent();

    if (lua_isstring(L, 2)) {
        const char* name = lua_tolstring(L, 2, 0);
        relative = region->GetLayoutFrameByName(name);
    } else if (lua_type(L, 2) == LUA_TTABLE) {
        lua_rawgeti(L, 2, 0);

        auto r = reinterpret_cast<CScriptRegion*>(lua_touserdata(L, -1));
        relative = r ? static_cast<CLayoutFrame*>(r) : nullptr;

        lua_settop(L, -2);
    } else if (lua_type(L, 2) == LUA_TNIL) {
        relative = CSimpleTop::s_instance;
    }

    if (!relative) {
        const char* name = lua_tolstring(L, 2, 0);
        return luaL_error(L, "%s:SetAllPoints(): Couldn't find region named '%s'", region->GetDisplayName(), name);
    }

    if (relative == region) {
        return luaL_error(L, "%s:SetAllPoints(): trying to anchor to itself", region->GetDisplayName());
    }

    if (relative->IsResizeDependency(region)) {
        return luaL_error(L, "%s:SetAllPoints(): %s is dependent on this", region->GetDisplayName(), static_cast<CScriptRegion*>(relative)->GetDisplayName());
    }

    region->SetAllPoints(relative, 1);

    return 0;
}

int32_t CScriptRegion_ClearAllPoints(lua_State* L) {
    int32_t type = CScriptRegion::GetObjectType();
    auto region = static_cast<CScriptRegion*>(FrameScript_GetObjectThis(L, type));

    if (region->ProtectedFunctionsAllowed()) {
        region->ClearAllPoints();
    } else {
        // TODO
        // void* v3 = CSimpleTop::s_instance->Function4692;

        // if (v3) {
        //     v3(object);
        // }
    }

    return 0;
}

int32_t CScriptRegion_CreateAnimationGroup(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_GetAnimationGroups(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_StopAnimating(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_IsDragging(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

int32_t CScriptRegion_IsMouseOver(lua_State* L) {
    WHOA_UNIMPLEMENTED();
}

FrameScript_Method ScriptRegionMethods[NUM_SCRIPT_REGION_SCRIPT_METHODS] = {
    { "IsProtected",                &CScriptRegion_IsProtected },
    { "CanChangeProtectedState",    &CScriptRegion_CanChangeProtectedState },
    { "SetParent",                  &CScriptRegion_SetParent },
    { "GetRect",                    &CScriptRegion_GetRect },
    { "GetCenter",                  &CScriptRegion_GetCenter },
    { "GetLeft",                    &CScriptRegion_GetLeft },
    { "GetRight",                   &CScriptRegion_GetRight },
    { "GetTop",                     &CScriptRegion_GetTop },
    { "GetBottom",                  &CScriptRegion_GetBottom },
    { "GetWidth",                   &CScriptRegion_GetWidth },
    { "SetWidth",                   &CScriptRegion_SetWidth },
    { "GetHeight",                  &CScriptRegion_GetHeight },
    { "SetHeight",                  &CScriptRegion_SetHeight },
    { "SetSize",                    &CScriptRegion_SetSize },
    { "GetSize",                    &CScriptRegion_GetSize },
    { "GetNumPoints",               &CScriptRegion_GetNumPoints },
    { "GetPoint",                   &CScriptRegion_GetPoint },
    { "SetPoint",                   &CScriptRegion_SetPoint },
    { "SetAllPoints",               &CScriptRegion_SetAllPoints },
    { "ClearAllPoints",             &CScriptRegion_ClearAllPoints },
    { "CreateAnimationGroup",       &CScriptRegion_CreateAnimationGroup },
    { "GetAnimationGroups",         &CScriptRegion_GetAnimationGroups },
    { "StopAnimating",              &CScriptRegion_StopAnimating },
    { "IsDragging",                 &CScriptRegion_IsDragging },
    { "IsMouseOver",                &CScriptRegion_IsMouseOver }
};
