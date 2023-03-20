file(GLOB PRIVATE_SOURCES
    "*.cpp"
    "gui/*.cpp"
)

if(WHOA_SYSTEM_WIN)
    file(GLOB WIN_SOURCES
        "gui/win/*.cpp"
    )
    list(APPEND PRIVATE_SOURCES ${WIN_SOURCES})
endif()

if(WHOA_SYSTEM_MAC)
    file(GLOB MAC_SOURCES
        "gui/mac/*.cpp"
    )
    list(APPEND PRIVATE_SOURCES ${MAC_SOURCES})
endif()

if(WHOA_SYSTEM_LINUX)
    file(GLOB LINUX_SOURCES
        "gui/linux/*.cpp"
    )
    list(APPEND PRIVATE_SOURCES ${LINUX_SOURCES})
endif()

add_library(client STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(client
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(client
    PRIVATE
        async
        db
        event
        gx
        model
        net
        ui
        util
        world
    PUBLIC
        bc
        common
        storm
)
