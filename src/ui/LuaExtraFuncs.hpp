#ifndef UI_LUA_EXTRA_FUNCS_HPP
#define UI_LUA_EXTRA_FUNCS_HPP

#include <cstdint>

struct lua_State;

int32_t sub_8168D0(lua_State*);
int32_t sub_816910(lua_State*);
int32_t strtrim(lua_State*);
int32_t strsplit(lua_State*);
int32_t strjoin(lua_State*);
int32_t sub_816C40(lua_State*);
int32_t sub_816D80(lua_State*);
int32_t strlenutf8(lua_State*);
int32_t issecure(lua_State*);
int32_t issecurevariable(lua_State*);
int32_t forceinsecure(lua_State*);
int32_t securecall(lua_State*);
int32_t hooksecurefunc(lua_State*);
int32_t debugload(lua_State*);
int32_t debuginfo(lua_State*);
int32_t debugprint(lua_State*);
int32_t debugdump(lua_State*);
int32_t debugbreak(lua_State*);
int32_t debughook(lua_State*);
int32_t debugtimestamp(lua_State*);
int32_t debugprofilestart(lua_State*);
int32_t debugprofilestop(lua_State*);
int32_t seterrorhandler(lua_State*);
int32_t geterrorhandler(lua_State*);
int32_t os_date(lua_State*);
int32_t os_time(lua_State*);
int32_t os_difftime(lua_State*);
int32_t debugstack(lua_State*);
int32_t debuglocals(lua_State*);
int32_t scrub(lua_State*);

#endif
