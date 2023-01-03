if(WHOA_SYSTEM_MAC OR WHOA_SYSTEM_LINUX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations -Wno-empty-body")
endif()

set(LUA_SRC_DIR "./src")

add_library(lua-5.1 STATIC
    ${LUA_SRC_DIR}/bit.c
    ${LUA_SRC_DIR}/lapi.c
    ${LUA_SRC_DIR}/lauxlib.c
    ${LUA_SRC_DIR}/lbaselib.c
    ${LUA_SRC_DIR}/lcode.c
    ${LUA_SRC_DIR}/ldblib.c
    ${LUA_SRC_DIR}/ldebug.c
    ${LUA_SRC_DIR}/ldo.c
    ${LUA_SRC_DIR}/ldump.c
    ${LUA_SRC_DIR}/lfunc.c
    ${LUA_SRC_DIR}/lgc.c
    ${LUA_SRC_DIR}/linit.c
    ${LUA_SRC_DIR}/liolib.c
    ${LUA_SRC_DIR}/llex.c
    ${LUA_SRC_DIR}/lmathlib.c
    ${LUA_SRC_DIR}/lmem.c
    ${LUA_SRC_DIR}/loadlib.c
    ${LUA_SRC_DIR}/lobject.c
    ${LUA_SRC_DIR}/lopcodes.c
    ${LUA_SRC_DIR}/loslib.c
    ${LUA_SRC_DIR}/lparser.c
    ${LUA_SRC_DIR}/lstate.c
    ${LUA_SRC_DIR}/lstring.c
    ${LUA_SRC_DIR}/lstrlib.c
    ${LUA_SRC_DIR}/ltable.c
    ${LUA_SRC_DIR}/ltablib.c
    ${LUA_SRC_DIR}/ltm.c
    ${LUA_SRC_DIR}/lua.c
    ${LUA_SRC_DIR}/luac.c
    ${LUA_SRC_DIR}/lundump.c
    ${LUA_SRC_DIR}/lvm.c
    ${LUA_SRC_DIR}/lzio.c
    ${LUA_SRC_DIR}/print.c
)

target_include_directories(lua-5.1
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
