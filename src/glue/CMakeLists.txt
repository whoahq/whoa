file(GLOB PRIVATE_SOURCES "*.cpp")

add_library(glue STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(glue
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(glue
    PRIVATE
        client
        component
        console
        db
        event
        gx
        model
        net
        sound
        ui
        util
    PUBLIC
        storm
)
