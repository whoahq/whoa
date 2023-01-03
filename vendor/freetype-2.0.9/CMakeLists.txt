if(WHOA_SYSTEM_MAC OR WHOA_SYSTEM_LINUX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations -Wno-empty-body")
endif()

set(FREETYPE_SRC_DIR "./src")
set(FREETYPE_INCLUDE_DIR "./include")

add_library(freetype-2.0 STATIC
    ${FREETYPE_SRC_DIR}/autohint/ahoptim.c
    ${FREETYPE_SRC_DIR}/autohint/autohint.c
    ${FREETYPE_SRC_DIR}/base/ftbase.c
    ${FREETYPE_SRC_DIR}/base/ftbbox.c
    ${FREETYPE_SRC_DIR}/base/ftextend.c
    ${FREETYPE_SRC_DIR}/base/ftglyph.c
    ${FREETYPE_SRC_DIR}/base/ftinit.c
    ${FREETYPE_SRC_DIR}/base/ftmm.c
    ${FREETYPE_SRC_DIR}/base/ftsynth.c
    ${FREETYPE_SRC_DIR}/base/ftsystem.c
    ${FREETYPE_SRC_DIR}/cid/cidgload.c
    ${FREETYPE_SRC_DIR}/cid/cidload.c
    ${FREETYPE_SRC_DIR}/cid/cidobjs.c
    ${FREETYPE_SRC_DIR}/cid/cidparse.c
    ${FREETYPE_SRC_DIR}/cid/cidriver.c
    ${FREETYPE_SRC_DIR}/cff/cff.c
    ${FREETYPE_SRC_DIR}/pcf/pcf.c
    ${FREETYPE_SRC_DIR}/psaux/psaux.c
    ${FREETYPE_SRC_DIR}/pshinter/pshinter.c
    ${FREETYPE_SRC_DIR}/psnames/psnames.c
    ${FREETYPE_SRC_DIR}/raster/raster.c
    ${FREETYPE_SRC_DIR}/sfnt/sfnt.c
    ${FREETYPE_SRC_DIR}/smooth/smooth.c
    ${FREETYPE_SRC_DIR}/truetype/truetype.c
    ${FREETYPE_SRC_DIR}/type1/type1.c
    ${FREETYPE_SRC_DIR}/winfonts/winfnt.c
)

target_include_directories(freetype-2.0
    PUBLIC
        ${FREETYPE_INCLUDE_DIR}
)
