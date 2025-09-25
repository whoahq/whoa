file(GLOB PRIVATE_SOURCES "*.cpp")

add_library(ui STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(ui
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(ui
    PRIVATE
        client
        console
        db
        event
        glue
        gx
        math
        model
        net
        util
    PUBLIC
        common
        storm
        tempest
)
