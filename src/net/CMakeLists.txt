file(GLOB PRIVATE_SOURCES
    "*.cpp"
    "connection/*.cpp"
    "grunt/*.cpp"
    "login/*.cpp"
    "srp/*.cpp"
)

if(WHOA_SYSTEM_WIN)
    file(GLOB WINSOCK_SOURCES
        "connection/winsock/*.cpp"
    )
    list(APPEND PRIVATE_SOURCES ${WINSOCK_SOURCES})
endif()

if(WHOA_SYSTEM_MAC OR WHOA_SYSTEM_LINUX)
    file(GLOB BSD_SOURCES
        "connection/bsd/*.cpp"
    )
    list(APPEND PRIVATE_SOURCES ${BSD_SOURCES})
endif()

add_library(net STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(net
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(net
    PRIVATE
        client
        event
    PUBLIC
        common
        storm
)

if(WHOA_SYSTEM_WIN)
    target_link_libraries(net
        PUBLIC
            ws2_32
            wsock32
    )
endif()
