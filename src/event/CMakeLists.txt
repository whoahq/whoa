file(GLOB PRIVATE_SOURCES "*.cpp")

if(WHOA_SYSTEM_WIN)
    file(GLOB WIN_SOURCES
        "win/*.cpp"
    )
    list(APPEND PRIVATE_SOURCES ${WIN_SOURCES})
endif()

if(WHOA_SYSTEM_MAC)
    file(GLOB MAC_SOURCES
        "mac/*.cpp"
        "mac/*.mm"
    )
    list(APPEND PRIVATE_SOURCES ${MAC_SOURCES})
endif()

if(WHOA_SYSTEM_LINUX)
    file(GLOB LINUX_SOURCES
        "linux/*.cpp"
    )
    list(APPEND PRIVATE_SOURCES ${LINUX_SOURCES})
endif()

if(WHOA_SYSTEM_WEB)
    file(GLOB WEB_SOURCES
        "web/*.cpp"
    )
    list(APPEND PRIVATE_SOURCES ${WEB_SOURCES})
endif()

add_library(event STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(event
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(event
    PRIVATE
        client
        gx
    PUBLIC
        common
        storm
        tempest
)
