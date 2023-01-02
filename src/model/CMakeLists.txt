file(GLOB PRIVATE_SOURCES "*.cpp")

add_library(model STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(model
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(model
    PRIVATE
        async
        gx
        math
        util
    PUBLIC
        common
        storm
        tempest
)
