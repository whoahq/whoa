#include "ui/CSimpleModelScript.hpp"
#include "ui/CSimpleModel.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

int32_t CSimpleModel_SetModel(lua_State* L) {
    auto type = CSimpleModel::GetObjectType();
    auto model = static_cast<CSimpleModel*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isstring(L, 2)) {
        return luaL_error(L, "Usage: %s:SetModel(\"file\")", model->GetDisplayName());
    }

    const char* file = lua_tolstring(L, 2, 0);

    model->SetModel(file);

    if (!model->m_model) {
        return luaL_error(L, "Invalid model file: %s", file);
    }

    return 0;
}

int32_t CSimpleModel_GetModel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_ClearModel(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_SetPosition(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_SetFacing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_SetScale(lua_State* L) {
    auto type = CSimpleModel::GetObjectType();
    auto model = static_cast<CSimpleModel*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isnumber(L, 2)) {
        return luaL_error(L, "Usage: %s:SetScale(scale)", model->GetDisplayName());
    }

    float scale = lua_tonumber(L, 2);
    model->SetScale(scale);

    return 0;
}

int32_t CSimpleModel_SetSequence(lua_State* L) {
    auto type = CSimpleModel::GetObjectType();
    auto model = static_cast<CSimpleModel*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isnumber(L, 2)) {
        return luaL_error(L, "Usage: %s:SetSequence(sequence)", model->GetDisplayName());
    }

    uint32_t sequence = lua_tonumber(L, 2);

    if (sequence >= 506) {
        return luaL_error(L, "Error: %s:SetSequence(sequence) exceeds valid range of 0 - %d", model->GetDisplayName(), 506);
    }

    model->SetSequence(sequence);

    return 0;
}

int32_t CSimpleModel_SetSequenceTime(lua_State* L) {
    auto type = CSimpleModel::GetObjectType();
    auto model = static_cast<CSimpleModel*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isnumber(L, 2) || !lua_isnumber(L, 3)) {
        return luaL_error(L, "Usage: %s:SetSequenceTime(sequence, time)", model->GetDisplayName());
    }

    uint32_t sequence = lua_tonumber(L, 2);
    int32_t time = lua_tonumber(L, 3);
    model->SetSequenceTime(sequence, time);

    return 0;
}

int32_t CSimpleModel_SetCamera(lua_State* L) {
    auto type = CSimpleModel::GetObjectType();
    auto model = static_cast<CSimpleModel*>(FrameScript_GetObjectThis(L, type));

    if (!lua_isnumber(L, 2)) {
        return luaL_error(L, "Usage: %s:SetCamera(index)", model->GetDisplayName());
    }

    int32_t index = lua_tonumber(L, 2);
    model->SetCameraByIndex(index);

    return 0;
}

int32_t CSimpleModel_SetLight(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_GetLight(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_GetPosition(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_GetFacing(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_GetScale(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_AdvanceTime(lua_State* L) {
    auto type = CSimpleModel::GetObjectType();
    auto model = static_cast<CSimpleModel*>(FrameScript_GetObjectThis(L, type));

    // TODO GxDefaultApi();

    return 0;
}

int32_t CSimpleModel_ReplaceIconTexture(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_SetFogColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_GetFogColor(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_SetFogNear(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_GetFogNear(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_SetFogFar(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_GetFogFar(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_ClearFog(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t CSimpleModel_SetGlow(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

FrameScript_Method SimpleModelMethods[NUM_SIMPLE_MODEL_SCRIPT_METHODS] = {
    { "SetModel",           &CSimpleModel_SetModel },
    { "GetModel",           &CSimpleModel_GetModel },
    { "ClearModel",         &CSimpleModel_ClearModel },
    { "SetPosition",        &CSimpleModel_SetPosition },
    { "SetFacing",          &CSimpleModel_SetFacing },
    { "SetModelScale",      &CSimpleModel_SetScale },
    { "SetSequence",        &CSimpleModel_SetSequence },
    { "SetSequenceTime",    &CSimpleModel_SetSequenceTime },
    { "SetCamera",          &CSimpleModel_SetCamera },
    { "SetLight",           &CSimpleModel_SetLight },
    { "GetLight",           &CSimpleModel_GetLight },
    { "GetPosition",        &CSimpleModel_GetPosition },
    { "GetFacing",          &CSimpleModel_GetFacing },
    { "GetModelScale",      &CSimpleModel_GetScale },
    { "AdvanceTime",        &CSimpleModel_AdvanceTime },
    { "ReplaceIconTexture", &CSimpleModel_ReplaceIconTexture },
    { "SetFogColor",        &CSimpleModel_SetFogColor },
    { "GetFogColor",        &CSimpleModel_GetFogColor },
    { "SetFogNear",         &CSimpleModel_SetFogNear },
    { "GetFogNear",         &CSimpleModel_GetFogNear },
    { "SetFogFar",          &CSimpleModel_SetFogFar },
    { "GetFogFar",          &CSimpleModel_GetFogFar },
    { "ClearFog",           &CSimpleModel_ClearFog },
    { "SetGlow",            &CSimpleModel_SetGlow }
};
