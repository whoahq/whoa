file(GLOB GX_SOURCES
    "*.cpp"
    "buffer/*.cpp"
    "font/*.cpp"
    "shader/*.cpp"
    "texture/*.cpp"
)

if(WHOA_SYSTEM_MAC)
    file(GLOB GLL_SOURCES "gll/*.cpp" "gll/*.mm")
    set_source_files_properties(${GLL_SOURCES}
        PROPERTIES COMPILE_FLAGS "-x objective-c++"
    )
    list(APPEND GX_SOURCES ${GLL_SOURCES})
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
        freetype-2.0
        storm
        tempest
)

if(WHOA_SYSTEM_MAC)
    target_link_libraries(gx
        PRIVATE
            "-framework AppKit"
            "-framework OpenGL"
    )
endif()
