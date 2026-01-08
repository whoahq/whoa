if(WHOA_SYSTEM_MAC OR WHOA_SYSTEM_LINUX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-non-prototype")
endif()

set(ZLIB_SRC_DIR "./")

add_library(zlib-1.2 STATIC
    ${ZLIB_SRC_DIR}/adler32.c
    ${ZLIB_SRC_DIR}/compress.c
    ${ZLIB_SRC_DIR}/crc32.c
    ${ZLIB_SRC_DIR}/deflate.c
    ${ZLIB_SRC_DIR}/gzio.c
    ${ZLIB_SRC_DIR}/inflate.c
    ${ZLIB_SRC_DIR}/trees.c
    ${ZLIB_SRC_DIR}/uncompr.c
    ${ZLIB_SRC_DIR}/zutil.c
)

target_include_directories(zlib-1.2
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
