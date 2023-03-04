file(GLOB PRIVATE_SOURCES "*.cpp")

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
