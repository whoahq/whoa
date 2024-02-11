file(GLOB GX_SOURCES
    "*.cpp"
    "buffer/*.cpp"
    "font/*.cpp"
    "shader/*.cpp"
    "texture/*.cpp"
)

if(WHOA_SYSTEM_WIN)
    file(GLOB D3D_SOURCES "d3d/*.cpp")
    list(APPEND GX_SOURCES ${D3D_SOURCES})
endif()

if(WHOA_SYSTEM_MAC)
    file(GLOB GLL_SOURCES "gll/*.cpp" "gll/*.mm")
    set_source_files_properties(${GLL_SOURCES}
        PROPERTIES COMPILE_FLAGS "-x objective-c++"
    )
    list(APPEND GX_SOURCES ${GLL_SOURCES})
endif()

# Build OpenGL/SDL graphics device on Windows and Linux
if(WHOA_SYSTEM_WIN OR WHOA_SYSTEM_LINUX)
    file(GLOB GLSDL_SOURCES "glsdl/*.cpp")
    list(APPEND GX_SOURCES ${GLSDL_SOURCES})
endif()

add_library(gx STATIC ${GX_SOURCES})

target_include_directories(gx
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(gx
    PRIVATE
        event
        math
        model
        ui
        util
    PUBLIC
        bc
        freetype-2.0
        storm
        tempest
)

if(WHOA_SYSTEM_WIN)
    target_link_libraries(gx
        PRIVATE
            d3d9.lib
    )
endif()

# Link SDL3 and GLEW for Windows and Linux
if (WHOA_SYSTEM_WIN OR WHOA_SYSTEM_LINUX)
    target_link_libraries(gx
        PRIVATE
            SDL3::SDL3-static
            libglew_static
        )
endif()

if(WHOA_SYSTEM_MAC)
    target_link_libraries(gx
        PRIVATE
            "-framework AppKit"
            "-framework OpenGL"
    )
endif()
