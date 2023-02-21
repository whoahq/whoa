#include "ui/LuaExtraFuncs.hpp"
#include "ui/FrameScript.hpp"
#include "ui/FrameScriptInternal.hpp"
#include "util/Lua.hpp"
#include "util/Unimplemented.hpp"
#include <cstdint>

luaL_Reg FrameScriptInternal::extra_funcs[31] = {
    { "setglobal", &sub_8168D0 },
    { "getglobal", &sub_816910 },
    { "strtrim", &strtrim },
    { "strsplit", &strsplit },
    { "strjoin", &strjoin },
    { "strreplace", &sub_816C40 },
    { "strconcat", &sub_816D80 },
    { "strlenutf8", &strlenutf8 },
    { "issecure", &issecure },
    { "issecurevariable", &issecurevariable },
    { "forceinsecure", &forceinsecure },
    { "securecall", &securecall },
    { "hooksecurefunc", &hooksecurefunc },
    { "debugload", &debugload },
    { "debuginfo", &debuginfo },
    { "debugprint", &debugprint },
    { "debugdump", &debugdump },
    { "debugbreak", &debugbreak },
    { "debughook", &debughook },
    { "debugtimestamp", &debugtimestamp },
    { "debugprofilestart", &debugprofilestart },
    { "debugprofilestop", &debugprofilestop },
    { "seterrorhandler", &seterrorhandler },
    { "geterrorhandler", &geterrorhandler },
    { "date", &os_date },
    { "time", &os_time },
    { "difftime", &os_difftime },
    { "debugstack", &debugstack },
    { "debuglocals", &debuglocals },
    { "scrub", &scrub },
    { nullptr, nullptr }
};

int32_t sub_8168D0(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t sub_816910(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t strtrim(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t strsplit(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t strjoin(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t sub_816C40(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t sub_816D80(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t strlenutf8(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t issecure(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t issecurevariable(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t forceinsecure(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t securecall(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t hooksecurefunc(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t debugload(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t debuginfo(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t debugprint(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t debugdump(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t debugbreak(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t debughook(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t debugtimestamp(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t debugprofilestart(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t debugprofilestop(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t seterrorhandler(lua_State* L) {
    if (lua_type(L, 1) != LUA_TFUNCTION) {
        luaL_error(L, "Usage: seterrorhandler(errfunc)");
        return 0;
    }

    if (FrameScript::s_errorHandlerFun != -1) {
        luaL_unref(L, LUA_REGISTRYINDEX, FrameScript::s_errorHandlerFun);
    }

    FrameScript::s_errorHandlerFun = luaL_ref(L, LUA_REGISTRYINDEX);

    return 0;
}

int32_t geterrorhandler(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t os_date(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t os_time(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t os_difftime(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t debugstack(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t debuglocals(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}

int32_t scrub(lua_State* L) {
    WHOA_UNIMPLEMENTED(0);
}
