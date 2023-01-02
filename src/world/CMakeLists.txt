file(GLOB PRIVATE_SOURCES "*.cpp")

add_library(world STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(world
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(world
    PRIVATE
        gx
        model
)
