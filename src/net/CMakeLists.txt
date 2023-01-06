file(GLOB PRIVATE_SOURCES "*.cpp" "connection/*.cpp" "grunt/*.cpp" "login/*.cpp" "srp/*.cpp")

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
