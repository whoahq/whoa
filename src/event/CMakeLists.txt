file(GLOB PRIVATE_SOURCES "*.cpp")

if(WHOA_SYSTEM_MAC)
    file(GLOB MAC_SOURCES
        "mac/*.cpp"
        "mac/*.mm"
    )
    list(APPEND PRIVATE_SOURCES ${MAC_SOURCES})
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
        gx
    PUBLIC
        common
        storm
        tempest
)
