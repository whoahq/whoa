file(GLOB GX_SOURCES
    "*.cpp"
    "blp/*.cpp"
    "buffer/*.cpp"
    "font/*.cpp"
    "shader/*.cpp"
    "texture/*.cpp"
)

if(WHOA_SYSTEM_WIN)
    file(GLOB WIN_SOURCES
        "d3d/*.cpp"
        "win/*.cpp"
    )

    list(APPEND GX_SOURCES ${WIN_SOURCES})
endif()

if(WHOA_SYSTEM_MAC)
    file(GLOB MAC_SOURCES
        "gll/*.cpp"
        "gll/*.mm"
        "mac/*.cpp"
    )

    set_source_files_properties(${MAC_SOURCES}
        PROPERTIES COMPILE_FLAGS "-x objective-c++"
    )

    list(APPEND GX_SOURCES ${MAC_SOURCES})
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

if(WHOA_SYSTEM_MAC)
    target_link_libraries(gx
        PRIVATE
            "-framework AppKit"
            "-framework OpenGL"
    )
endif()
