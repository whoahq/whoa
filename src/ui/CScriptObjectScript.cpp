#include "ui/CScriptObjectScript.hpp"
#include "ui/CScriptObject.hpp"
#include "util/Lua.hpp"
#include <cstdint>

int32_t CScriptObject_GetObjectType(lua_State* L) {
    auto object = static_cast<CScriptObject*>(FrameScript_GetObjectThis(L, CScriptObject::GetObjectType()));
    auto type = object->GetObjectTypeName();

    lua_pushstring(L, type);

    return 1;
}

int32_t CScriptObject_IsObjectType(lua_State* L) {
    auto object = static_cast<CScriptObject*>(FrameScript_GetObjectThis(L, CScriptObject::GetObjectType()));

    if (!lua_isstring(L, 2)) {
        auto name = object->GetDisplayName();
        return luaL_error(L, "Usage: %s:IsObjectType(\"type\")", name);
    }

    auto type = lua_tolstring(L, 2, nullptr);

    if (object->IsA(type)) {
        lua_pushnumber(L, 1.0);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

int32_t CScriptObject_GetName(lua_State* L) {
    auto object = static_cast<CScriptObject*>(FrameScript_GetObjectThis(L, CScriptObject::GetObjectType()));
    auto name = object->GetName();

    if (name && *name) {
        lua_pushstring(L, name);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

int32_t CScriptObject_GetParent(lua_State* L) {
    auto object = static_cast<CScriptObject*>(FrameScript_GetObjectThis(L, CScriptObject::GetObjectType()));
    CScriptObject* parent = object->GetScriptObjectParent();

    if (parent) {
        if (!parent->lua_registered) {
            parent->RegisterScriptObject(nullptr);
        }

        lua_rawgeti(L, LUA_REGISTRYINDEX, parent->lua_objectRef);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

FrameScript_Method ScriptObjectMethods[NUM_SCRIPT_OBJECT_SCRIPT_METHODS] = {
    { "GetObjectType",  &CScriptObject_GetObjectType },
    { "IsObjectType",   &CScriptObject_IsObjectType },
    { "GetName",        &CScriptObject_GetName },
    { "GetParent",      &CScriptObject_GetParent }
};
